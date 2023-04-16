#pragma once

#include <queue>
#include <atomic>
#include "Event.h"
#include "util/ThreadSafeQueue.h"
#include "Network.h"

class Atlas {
private:
    bool isServer;
    Event::EventState* eventState;
    Entity::EntityState* entityState;
    Network::NetworkState* networkState;

public:
    Atlas(bool isServer);
    ~Atlas();
    void start();
};