#include "EventHandling.h"

void handleEventEntityCreate(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    if (entityState->entities.find(id) == entityState->entities.end()) {
        Entity::Entity* entity = new Entity::Entity();
        entity->id = id;
        entityState->entities[id] = entity;
        event->success = true;
    }
}

void handleEventEntityDestroy(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    if (entityState->entities.find(id) != entityState->entities.end()) {
        entityState->entities.erase(id);
        event->success = true;
    }
}

void handleEventEntityUpdateCameraDistanceFromTarget(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->camera_DistanceFromTarget = event->dataFloat;
        event->success = true;
    }
}

void handleEventEntityGetCameraDistanceFromTarget(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataFloat = it->second->camera_DistanceFromTarget;
        event->success = true;
    }
}

void handleEventEntityUpdateCameraVerticalAngle(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->camera_VerticalAngle = event->dataFloat;
        event->success = true;
    }
}

void handleEventEntityGetCameraVerticalAngle(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataFloat = it->second->camera_VerticalAngle;
        event->success = true;
    }
}

void handleEventEntityUpdateLightColor(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->light_Color.x = event->dataVector3f.x;
        it->second->light_Color.y = event->dataVector3f.y;
        it->second->light_Color.z = event->dataVector3f.z;
        event->success = true;
    }
}

void handleEventEntityGetLightColor(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataVector3f.x = it->second->light_Color.x;
        event->dataVector3f.y = it->second->light_Color.y;
        event->dataVector3f.z = it->second->light_Color.z;
        event->success = true;
    }
}

void handleEventEntityUpdateLightAttenuation(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->light_Attenuation.x = event->dataVector3f.x;
        it->second->light_Attenuation.y = event->dataVector3f.y;
        it->second->light_Attenuation.z = event->dataVector3f.z;
        event->success = true;
    }
}

void handleEventEntityGetLightAttenuation(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataVector3f.x = it->second->light_Attenuation.x;
        event->dataVector3f.y = it->second->light_Attenuation.y;
        event->dataVector3f.z = it->second->light_Attenuation.z;
        event->success = true;
    }
}

void handleEventEntityUpdateRenderable3DMeshAssetId(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->renderable3D_MeshAssetId = event->dataId;
        event->success = true;
    }
}

void handleEventEntityGetRenderable3DMeshAssetId(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataId = it->second->renderable3D_MeshAssetId;
        event->success = true;
    }
}

void handleEventEntityUpdateRenderable3DMaterialAssetId(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->renderable3D_MaterialAssetId = event->dataId;
        event->success = true;
    }
}

void handleEventEntityGetRenderable3DMaterialAssetId(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataId = it->second->renderable3D_MaterialAssetId;
        event->success = true;
    }
}

void handleEventEntityUpdateRenderable3DTextureAtlasIndex(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->renderable3D_TextureAtlasIndex = event->dataInt;
        event->success = true;
    }
}

void handleEventEntityGetRenderable3DTextureAtlasIndex(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataInt = it->second->renderable3D_TextureAtlasIndex;
        event->success = true;
    }
}

void handleEventEntityUpdateSpatial3DPosition(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->spatial3D_Position.x = event->dataVector3f.x;
        it->second->spatial3D_Position.y = event->dataVector3f.y;
        it->second->spatial3D_Position.z = event->dataVector3f.z;
        event->success = true;
    }
}

void handleEventEntityGetSpatial3DPosition(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataVector3f.x = it->second->spatial3D_Position.x;
        event->dataVector3f.y = it->second->spatial3D_Position.y;
        event->dataVector3f.z = it->second->spatial3D_Position.z;
        event->success = true;
    }
}

void handleEventEntityUpdateSpatial3DRotation(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->spatial3D_Rotation.x = event->dataVector3f.x;
        it->second->spatial3D_Rotation.y = event->dataVector3f.y;
        it->second->spatial3D_Rotation.z = event->dataVector3f.z;
        event->success = true;
    }
}

void handleEventEntityGetSpatial3DRotation(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataVector3f.x = it->second->spatial3D_Rotation.x;
        event->dataVector3f.y = it->second->spatial3D_Rotation.y;
        event->dataVector3f.z = it->second->spatial3D_Rotation.z;
        event->success = true;
    }
}

void handleEventEntityUpdateSpatial3DScale(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        it->second->spatial3D_Scale = event->dataFloat;
        event->success = true;
    }
}

void handleEventEntityGetSpatial3DScale(Event::Event *event, Entity::EntityState *entityState) {
    event->success = false;
    UUID id = event->dataId;
    auto it = entityState->entities.find(id);
    if (it != entityState->entities.end()) {
        event->dataFloat = it->second->spatial3D_Scale;
        event->success = true;
    }
}

bool EventHandling::handleNextEvent(Event::EventState *eventState, Entity::EntityState *entityState) {
    Event::Event *event = eventState->eventQueue.pop();
    switch (event->eventType) {
        case Event::EventType::NONE:
            break;
        case Event::EventType::ENTITY_CREATE:
            handleEventEntityCreate(event, entityState);
            break;
        case Event::EventType::ENTITY_DESTROY:
            handleEventEntityDestroy(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_CAMERA_DISTANCE_FROM_TARGET:
            handleEventEntityUpdateCameraDistanceFromTarget(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_CAMERA_DISTANCE_FROM_TARGET:
            handleEventEntityGetCameraDistanceFromTarget(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_CAMERA_VERTICAL_ANGLE:
            handleEventEntityUpdateCameraVerticalAngle(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_CAMERA_VERTICAL_ANGLE:
            handleEventEntityGetCameraVerticalAngle(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_LIGHT_COLOR:
            handleEventEntityUpdateLightColor(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_LIGHT_COLOR:
            handleEventEntityGetLightColor(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_LIGHT_ATTENUATION:
            handleEventEntityUpdateLightAttenuation(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_LIGHT_ATTENUATION:
            handleEventEntityGetLightAttenuation(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_RENDERABLE3D_MESH_ASSET_ID:
            handleEventEntityUpdateRenderable3DMeshAssetId(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_RENDERABLE3D_MESH_ASSET_ID:
            handleEventEntityGetRenderable3DMeshAssetId(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_RENDERABLE3D_MATERIAL_ASSET_ID:
            handleEventEntityUpdateRenderable3DMaterialAssetId(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_RENDERABLE3D_MATERIAL_ASSET_ID:
            handleEventEntityGetRenderable3DMaterialAssetId(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_RENDERABLE3D_TEXTURE_ATLAS_INDEX:
            handleEventEntityUpdateRenderable3DTextureAtlasIndex(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_RENDERABLE3D_TEXTURE_ATLAS_INDEX:
            handleEventEntityGetRenderable3DTextureAtlasIndex(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_SPATIAL3D_POSITION:
            handleEventEntityUpdateSpatial3DPosition(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_SPATIAL3D_POSITION:
            handleEventEntityGetSpatial3DPosition(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_SPATIAL3D_ROTATION:
            handleEventEntityUpdateSpatial3DRotation(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_SPATIAL3D_ROTATION:
            handleEventEntityGetSpatial3DRotation(event, entityState);
            break;
        case Event::EventType::ENTITY_UPDATE_SPATIAL3D_SCALE:
            handleEventEntityUpdateSpatial3DScale(event, entityState);
            break;
        case Event::EventType::ENTITY_GET_SPATIAL3D_SCALE:
            handleEventEntityGetSpatial3DScale(event, entityState);
            break;
        default:
            event->success = false;
            break;
    }

    event->isProcessed = true;
    event->success = true;
    bool success = event->success;

    if (event->success) {
        eventState->eventQueueProcessed.push(event);
    } else {
        eventState->eventPool.returnObject(event);
    }
    return success;
}