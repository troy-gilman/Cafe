#pragma once

#include <cstring>
#include "Entity.h"
#include "util/Types.h"
#include "util/ThreadSafeQueue.h"
#include "util/ObjectPool.h"


namespace Event {

    enum EventType {
        NONE,
        ENTITY_CREATE,
        ENTITY_DESTROY,
        ENTITY_UPDATE_CAMERA_DISTANCE_FROM_TARGET,
        ENTITY_GET_CAMERA_DISTANCE_FROM_TARGET,
        ENTITY_UPDATE_CAMERA_VERTICAL_ANGLE,
        ENTITY_GET_CAMERA_VERTICAL_ANGLE,
        ENTITY_UPDATE_LIGHT_COLOR,
        ENTITY_GET_LIGHT_COLOR,
        ENTITY_UPDATE_LIGHT_ATTENUATION,
        ENTITY_GET_LIGHT_ATTENUATION,
        ENTITY_UPDATE_RENDERABLE3D_MESH_ASSET_ID,
        ENTITY_GET_RENDERABLE3D_MESH_ASSET_ID,
        ENTITY_UPDATE_RENDERABLE3D_MATERIAL_ASSET_ID,
        ENTITY_GET_RENDERABLE3D_MATERIAL_ASSET_ID,
        ENTITY_UPDATE_RENDERABLE3D_TEXTURE_ATLAS_INDEX,
        ENTITY_GET_RENDERABLE3D_TEXTURE_ATLAS_INDEX,
        ENTITY_UPDATE_SPATIAL3D_POSITION,
        ENTITY_GET_SPATIAL3D_POSITION,
        ENTITY_UPDATE_SPATIAL3D_ROTATION,
        ENTITY_GET_SPATIAL3D_ROTATION,
        ENTITY_UPDATE_SPATIAL3D_SCALE,
        ENTITY_GET_SPATIAL3D_SCALE,
    };

    struct Event {
        UUID eventId;
        int eventType;
        bool isProcessed;
        bool success;

        UUID dataId;
        float dataFloat;
        int dataInt;
        bool dataBool;
        CharBuffer dataString;
        Vector3f dataVector3f;
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

