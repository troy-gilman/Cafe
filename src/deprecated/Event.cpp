#include "Event.h"


void Event::copyEvent(Event *src, Event *target) {
    memcpy(src, target, sizeof(Event));
}

void Event::resetEvent(Event* event) {
    event->eventId = 0;
    event->eventType = NONE;
    event->isProcessed = false;
    event->success = false;

    memcpy(&event->field1, nullptr, sizeof(FieldUnion));
    memcpy(&event->field2, nullptr, sizeof(FieldUnion));
    memcpy(&event->field3, nullptr, sizeof(FieldUnion));
    memcpy(&event->field4, nullptr, sizeof(FieldUnion));
}