#pragma once

#include <unordered_map>
#include "Entity.hpp"
#include "Types.hpp"


struct State {
    std::unordered_map<UUID, Entity> entities;

    State() {
        entities.reserve(1024);
    }
};