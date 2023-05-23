#include "Event.h"


void Event::copyEvent(Event *src, Event *target) {
    memcpy(src, target, sizeof(Event));
}

void Event::resetEvent(Event* event) {
    event->eventId = 0;
    event->eventType = NONE;
    event->isProcessed = false;
    event->success = false;

    memset(&event->field1, 0, sizeof(FieldUnion));
    memset(&event->field2, 0, sizeof(FieldUnion));
    memset(&event->field3, 0, sizeof(FieldUnion));
    memset(&event->field4, 0, sizeof(FieldUnion));
}