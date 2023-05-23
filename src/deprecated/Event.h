#pragma once

#include <cstring>
#include "../ECS.h"
#include "../util/Types.h"
#include "../util/ThreadSafeQueue.h"
#include "../util/ObjectPool.h"


namespace Event {

    enum EventType {
        NONE,
        ENTITY_CREATE,
        ENTITY_DESTROY,
        ENTITY_ADD_COMPONENT,
        ENTITY_REMOVE_COMPONENT,
        COMPONENT_SET_FIELD,
        COMPONENT_GET_FIELD,
    };

    struct Event {
        UUID eventId;
        i32 eventType;
        bool isProcessed;
        bool success;

        FieldUnion field1;
        FieldUnion field2;
        FieldUnion field3;
        FieldUnion field4;
    };

    struct EventState {
        ObjectPool<Event> eventPool;
        ThreadSafeQueue<Event*> eventQueue;
        ThreadSafeQueue<Event*> eventQueueProcessed;

        EventState(int eventsAvailable) : eventPool(eventsAvailable) {}
    };

    void copyEvent(Event* src, Event* target);
    void resetEvent(Event* event);
}

