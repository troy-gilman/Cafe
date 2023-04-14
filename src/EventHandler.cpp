#include "EventHandler.hpp"

EventHandler::EventHandler(ObjectPool<Event>* eventPool) {
    this->eventPool = eventPool;
}

EventHandler::~EventHandler() {

}

bool EventHandler::hasProcessedEvent() const {
    return !processedEvents.empty();
}

size_t EventHandler::getNumProcessedEvents() const {
    return processedEvents.size();
}

Event* EventHandler::popProcessedEvent() {
    if (processedEvents.empty()) {
        return nullptr;
    }
    Event* event = processedEvents.front();
    processedEvents.pop();
    return event;
}

void EventHandler::handleEvent(State* state, Event* event) {
    switch (event->eventType) {
        case EventType::NONE:
            break;
        case EventType::ENTITY_CREATE:
            handleEventEntityCreate(state, event);
            break;
        case EventType::ENTITY_DESTROY:
            handleEventEntityDestroy(state, event);
            break;
        case EventType::ENTITY_UPDATE_CAMERA_DISTANCE_FROM_TARGET:
            handleEventEntityUpdateCameraDistanceFromTarget(state, event);
            break;
        case EventType::ENTITY_GET_CAMERA_DISTANCE_FROM_TARGET:
            handleEventEntityGetCameraDistanceFromTarget(state, event);
            break;
        case EventType::ENTITY_UPDATE_CAMERA_VERTICAL_ANGLE:
            handleEventEntityUpdateCameraVerticalAngle(state, event);
            break;
        case EventType::ENTITY_GET_CAMERA_VERTICAL_ANGLE:
            handleEventEntityGetCameraVerticalAngle(state, event);
            break;
        case EventType::ENTITY_UPDATE_LIGHT_COLOR:
            handleEventEntityUpdateLightColor(state, event);
            break;
        case EventType::ENTITY_GET_LIGHT_COLOR:
            handleEventEntityGetLightColor(state, event);
            break;
        case EventType::ENTITY_UPDATE_LIGHT_ATTENUATION:
            handleEventEntityUpdateLightAttenuation(state, event);
            break;
        case EventType::ENTITY_GET_LIGHT_ATTENUATION:
            handleEventEntityGetLightAttenuation(state, event);
            break;
        case EventType::ENTITY_UPDATE_RENDERABLE3D_MESH_ASSET_ID:
            handleEventEntityUpdateRenderable3DMeshAssetID(state, event);
            break;
        case EventType::ENTITY_GET_RENDERABLE3D_MESH_ASSET_ID:
            handleEventEntityGetRenderable3DMeshAssetID(state, event);
            break;
        case EventType::ENTITY_UPDATE_RENDERABLE3D_MATERIAL_ASSET_ID:
            handleEventEntityUpdateRenderable3DMaterialAssetID(state, event);
            break;
        case EventType::ENTITY_GET_RENDERABLE3D_MATERIAL_ASSET_ID:
            handleEventEntityGetRenderable3DMaterialAssetID(state, event);
            break;
        case EventType::ENTITY_UPDATE_RENDERABLE3D_TEXTURE_ATLAS_INDEX:
            handleEventEntityUpdateRenderable3DTextureAtlasIndex(state, event);
            break;
        case EventType::ENTITY_GET_RENDERABLE3D_TEXTURE_ATLAS_INDEX:
            handleEventEntityGetRenderable3DTextureAtlasIndex(state, event);
            break;
        case EventType::ENTITY_UPDATE_SPATIAL3D_POSITION:
            handleEventEntityUpdateSpatial3DPosition(state, event);
            break;
        case EventType::ENTITY_GET_SPATIAL3D_POSITION:
            handleEventEntityGetSpatial3DPosition(state, event);
            break;
        case EventType::ENTITY_UPDATE_SPATIAL3D_ROTATION:
            handleEventEntityUpdateSpatial3DRotation(state, event);
            break;
        case EventType::ENTITY_GET_SPATIAL3D_ROTATION:
            handleEventEntityGetSpatial3DRotation(state, event);
            break;
        case EventType::ENTITY_UPDATE_SPATIAL3D_SCALE:
            handleEventEntityUpdateSpatial3DScale(state, event);
            break;
        case EventType::ENTITY_GET_SPATIAL3D_SCALE:
            handleEventEntityGetSpatial3DScale(state, event);
            break;
    }
    event->isProcessed = true;
    event->success = true;
    if (event->success) {
        Event* eventCopy = eventPool->waitForObject();
        eventCopy->copy(event);
        processedEvents.push(eventCopy);
    }
}

void EventHandler::handleEventEntityCreate(State* state, Event* event) {
    if (state->entities.find(event->dataId) == state->entities.end()) {
        Entity entity;
        entity.id = event->dataId;
        state->entities[entity.id] = entity;
        event->success = true;
    }
}

void EventHandler::handleEventEntityDestroy(State* state, Event* event) {
    if (state->entities.find(event->dataId) != state->entities.end()) {
        state->entities.erase(event->dataId);
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateCameraDistanceFromTarget(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.camera_DistanceFromTarget = event->dataFloat;
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetCameraDistanceFromTarget(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataFloat = it->second.camera_DistanceFromTarget;
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateCameraVerticalAngle(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.camera_VerticalAngle = event->dataFloat;
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetCameraVerticalAngle(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataFloat = it->second.camera_VerticalAngle;
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateLightColor(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.light_Color[0] = event->dataVector3f[0];
        it->second.light_Color[1] = event->dataVector3f[1];
        it->second.light_Color[2] = event->dataVector3f[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetLightColor(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataVector3f[0] = it->second.light_Color[0];
        event->dataVector3f[1] = it->second.light_Color[1];
        event->dataVector3f[2] = it->second.light_Color[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateLightAttenuation(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.light_Attenuation[0] = event->dataVector3f[0];
        it->second.light_Attenuation[1] = event->dataVector3f[1];
        it->second.light_Attenuation[2] = event->dataVector3f[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetLightAttenuation(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataVector3f[0] = it->second.light_Attenuation[0];
        event->dataVector3f[1] = it->second.light_Attenuation[1];
        event->dataVector3f[2] = it->second.light_Attenuation[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateRenderable3DMeshAssetID(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.renderable3D_MeshAssetId = event->dataId;
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetRenderable3DMeshAssetID(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataId = it->second.renderable3D_MeshAssetId;
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateRenderable3DMaterialAssetID(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.renderable3D_MaterialAssetId = event->dataId;
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetRenderable3DMaterialAssetID(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataId = it->second.renderable3D_MaterialAssetId;
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateRenderable3DTextureAtlasIndex(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.renderable3D_TextureAtlasIndex = event->dataInt;
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetRenderable3DTextureAtlasIndex(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataInt = it->second.renderable3D_TextureAtlasIndex;
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateSpatial3DPosition(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.spatial3D_Position[0] = event->dataVector3f[0];
        it->second.spatial3D_Position[1] = event->dataVector3f[1];
        it->second.spatial3D_Position[2] = event->dataVector3f[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetSpatial3DPosition(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataVector3f[0] = it->second.spatial3D_Position[0];
        event->dataVector3f[1] = it->second.spatial3D_Position[1];
        event->dataVector3f[2] = it->second.spatial3D_Position[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateSpatial3DRotation(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.spatial3D_Rotation[0] = event->dataVector3f[0];
        it->second.spatial3D_Rotation[1] = event->dataVector3f[1];
        it->second.spatial3D_Rotation[2] = event->dataVector3f[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetSpatial3DRotation(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataVector3f[0] = it->second.spatial3D_Rotation[0];
        event->dataVector3f[1] = it->second.spatial3D_Rotation[1];
        event->dataVector3f[2] = it->second.spatial3D_Rotation[2];
        event->success = true;
    }
}

void EventHandler::handleEventEntityUpdateSpatial3DScale(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        it->second.spatial3D_Scale = event->dataFloat;
        event->success = true;
    }
}

void EventHandler::handleEventEntityGetSpatial3DScale(State* state, Event* event) {
    auto it = state->entities.find(event->dataId);
    if (it != state->entities.end()) {
        event->dataFloat = it->second.spatial3D_Scale;
        event->success = true;
    }
}
