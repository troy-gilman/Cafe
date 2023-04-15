#pragma once

#include <queue>
#include <atomic>
#include "Event.hpp"
#include "util/ThreadSafeQueue.hpp"
#include "Network.hpp"

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