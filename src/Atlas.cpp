#include "Atlas.h"
#include <thread>
#include "util/UUIDGenerator.h"
#include "EventHandling.h"
#include <iostream>

Atlas::Atlas(bool isServer) : isServer(isServer) {
    eventState = new Event::EventState(10000);
    entityState = new Entity::EntityState();
    networkState = new Network::NetworkState();
}

Atlas::~Atlas() {
    delete eventState;
    delete entityState;
    delete networkState;
}

static void networkControllerReadEventLoop(Event::EventState* eventState, Network::NetworkState* networkState, bool isServer) {
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
            Event::Event* event = eventState->eventPool.waitForObject();
            Event::copyEvent(&eventMsg->events[i], event);
            eventState->eventQueue.push(event);
        }
        eventMsg->numEvents = 0;
    }
}

static void eventCreatorLoop(Event::EventState* eventState, bool isServer, bool* done) {
    if (isServer) {
        *done = true;
        return;
    }
    UUIDGenerator uuidGenerator;
    int numEvents = 10000;
    for (int i = 0; i < numEvents; i++) {
        Event::Event* event = eventState->eventPool.waitForObject();
        Event::resetEvent(event);

        Event::EventType eventType = static_cast<Event::EventType>(i % 24);
        event->eventType = eventType;

        UUID id = uuidGenerator.generateUUID();
        event->dataId = id;

        eventState->eventQueue.push(event);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Done creating events" << std::endl;
    *done = true;
}

void Atlas::start() {
    bool done = false;
    std::thread eventReaderThread(networkControllerReadEventLoop, eventState, networkState, isServer);
    std::thread eventCreatorThread(eventCreatorLoop, eventState, isServer, &done);
    int eventsProcessed = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        // Process events in our queue
        if (!eventState->eventQueue.empty()) {
            bool success = EventHandling::handleNextEvent(eventState, entityState);
            if (success) {
                eventsProcessed++;
            }
            if (eventsProcessed % 1000 == 0) {
                std::cout << "Events successfully processed: " << eventsProcessed << std::endl;
            }
        }
        // Handle events that were processed
        if (!eventState->eventQueueProcessed.empty()) {
            Event::Event* event = eventState->eventQueueProcessed.pop();
            if (!isServer) {
                Network::EventMsg* eventMsg = &(networkState->eventMsgOut);
                int numEvents = eventMsg->numEvents;
                Event::copyEvent(event, &eventMsg->events[numEvents]);
                eventMsg->numEvents++;
                if (eventMsg->numEvents == Network::MAX_EVENTS_PER_MSG) {
                    Network::sendEventMsg(networkState);
                }
            }
            eventState->eventPool.returnObject(event);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        bool shouldExit = !isServer || (isServer && elapsed.count() > 30);
        if (done && eventState->eventQueue.empty() && shouldExit) {
            eventReaderThread.detach();
            eventCreatorThread.join();
            std::cout << "Successfully processed " << eventsProcessed << " events" << std::endl;
            return;
        }
    }
}

