#include "Atlas.hpp"
#include <thread>
#include "UUIDGenerator.hpp"
#include <iostream>

Atlas::Atlas(bool isServer) : isServer(isServer) {
    eventPool = new ObjectPool<Event>(10000);
    eventHandler = new EventHandler(eventPool);
    state = new State();
    networkState = new Network::State();
}

Atlas::~Atlas() {
    delete eventPool;
    delete eventHandler;
    delete state;
    delete networkState;
}

static void networkControllerReadEventLoop(ThreadSafeQueue<Event*>* eventQueue, ObjectPool<Event>* eventPool, Network::State* networkState, bool isServer) {
    if (isServer) {
        Network::startServer(networkState);
    } else {
        Network::startClient(networkState);
    }
    while (true) {
        Network::receiveEventMsg(networkState);
        Network::EventMsg* eventMsg = &(networkState->eventMsgIn);
        if (eventMsg->numEvents == 0) {
            continue;
        }
        for (int i = 0; i < eventMsg->numEvents; i++) {
            Event* event = eventPool->waitForObject();
            event->copy(&eventMsg->events[i]);
            eventQueue->push(event);
        }
        //std::cout << "Received " << eventMsg->numEvents << " events" << std::endl;
        eventMsg->numEvents = 0;
    }
}

static void eventCreatorLoop(ThreadSafeQueue<Event*>* eventQueue, ObjectPool<Event>* eventPool, bool isServer, bool* done) {
    if (isServer) {
        *done = true;
        return;
    }
    UUIDGenerator uuidGenerator;
    int numEvents = 10000;
    for (int i = 0; i < numEvents; i++) {
        Event* event = eventPool->waitForObject();
        event->reset();

        EventType eventType = static_cast<EventType>(i % 24);
        event->eventType = eventType;

        UUID id = uuidGenerator.generateUUID();
        event->dataId = id;

        eventQueue->push(event);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Done creating events" << std::endl;
    *done = true;
}

void Atlas::start() {
    bool done = false;
    std::thread eventReaderThread(networkControllerReadEventLoop, &eventQueue, eventPool, networkState, isServer);
    std::thread eventCreatorThread(eventCreatorLoop, &eventQueue, eventPool, isServer, &done);
    int eventsProcessed = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        // Process events in our queue
        if (!eventQueue.empty()) {
            Event* event = eventQueue.pop();
            eventHandler->handleEvent(state, event);
            if (event->success) {
                eventsProcessed++;
            }
            eventPool->returnObject(event);
            if (eventsProcessed % 1000 == 0) {
                std::cout << "Events successfully processed: " << eventsProcessed << std::endl;
            }
        }
        // Handle events that were processed
        if (eventHandler->hasProcessedEvent()) {
            Event* event = eventHandler->popProcessedEvent();
            if (!isServer) {
                Network::EventMsg* eventMsg = &(networkState->eventMsgOut);
                int numEvents = eventMsg->numEvents;
                eventMsg->events[numEvents].copy(event);
                eventMsg->numEvents++;
                if (eventMsg->numEvents == Network::MAX_EVENTS_PER_MSG) {
                    //std::cout << "Sending " << eventMsg->numEvents << " events" << std::endl;
                    Network::sendEventMsg(networkState);
                }
            }
            eventPool->returnObject(event);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        bool shouldExit = !isServer || (isServer && elapsed.count() > 60);
        if (done && eventQueue.empty() && shouldExit) {
            eventReaderThread.detach();
            eventCreatorThread.join();
            std::cout << "Successfully processed " << eventsProcessed << " events" << std::endl;
            return;
        }
    }
}

