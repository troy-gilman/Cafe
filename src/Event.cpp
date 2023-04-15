#include "Event.hpp"


void Event::copyEvent(Event *src, Event *target) {
    memcpy(src, target, sizeof(Event));
}

void Event::resetEvent(Event* event) {
    event->eventId = 0;
    event->eventType = NONE;
    event->isProcessed = false;
    event->success = false;

    event->dataId = 0;
    event->dataFloat = 0.0f;
    event->dataInt = 0;
    event->dataBool = false;
    memset(event->dataString, 0, sizeof(event->dataString));
    memset(event->dataVector3f, 0, sizeof(event->dataVector3f));
}