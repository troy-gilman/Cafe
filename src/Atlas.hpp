#pragma once

#include <queue>
#include <atomic>
#include "Event.hpp"
#include "NetworkController.hpp"
#include "EventHandler.hpp"
#include "ThreadSafeQueue.hpp"

class Atlas {
private:
    ObjectPool<Event>* eventPool;
    NetworkController* networkController;
    EventHandler* eventHandler;
    State* state;
    ThreadSafeQueue<Event*> eventQueue;
    bool isServer;

public:
    Atlas(bool isServer);
    ~Atlas();
    void start();



};