#pragma once

#include <cstring>
#include "Entity.hpp"
#include "Types.hpp"

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

    Event() {
        reset();
    }

    void reset() {
        eventId = 0;
        eventType = NONE;
        isProcessed = false;
        success = false;

        dataId = 0;
        dataFloat = 0.0f;
        dataInt = 0;
        dataBool = false;
        memset(dataString, 0, sizeof(dataString));
        memset(dataVector3f, 0, sizeof(dataVector3f));
    }

    void copy(Event* other) {
        memcpy(this, other, sizeof(Event));
    }
};
