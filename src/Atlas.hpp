#pragma once

#include <queue>
#include <atomic>
#include "Event.hpp"
#include "EventHandler.hpp"
#include "ThreadSafeQueue.hpp"
#include "Network.hpp"

class Atlas {
private:
    bool isServer;
    ObjectPool<Event>* eventPool;
    EventHandler* eventHandler;
    ThreadSafeQueue<Event*> eventQueue;
    State* state;
    Network::State* networkState;

public:
    Atlas(bool isServer);
    ~Atlas();
    void start();



};