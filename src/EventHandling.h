#pragma once

#include "Event.h"
#include "ECS.h"

namespace EventHandling {

    // This is the function that will be called by the main thread to handle events.
    // It will be called in a loop until the program is closed.
    bool handleNextEvent(Event::EventState* eventState, ECS::EntityComponentSystem* ecs);
}