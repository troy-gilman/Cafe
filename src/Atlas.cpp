#include "Atlas.h"

#include <iostream>
#include "util/MapUtils.h"

Atlas::Atlas() {
    ecs = new ECS::EntityComponentSystem();
    assetPack = new Asset::AssetPack();
    window = new Render::Window();
    input = new Input::InputState();
}

Atlas::~Atlas() {
    delete ecs;
    delete assetPack;
    delete window;
    delete input;
}

void Atlas::init() {
    Render::initWindow(window);
    Input::initInputState(input, window->glfwWindow);
    ECS::initEntityComponentSystem(ecs);

    ECS::Entity* camera = new ECS::Entity();
    camera->id = ecs->nextEntityId;
    ECS::Component* component = new ECS::Component();
    component->type = ECS::COMPONENT_TYPE_SPATIAL_3D;
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[component->type];
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_POSITION, {0, -2, -5});
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION, {0, 0, 0});
    ECS::setField_f32(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_SCALE, 1.0f);
    camera->components[component->type] = component;
    ecs->entities[camera->id] = camera;
    ecs->numEntities++;
    ecs->nextEntityId++;
}

void Atlas::start() {
    bool done = false;
    int eventsProcessed = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {

    }
}

// Does not work properly
//void handleCameraThirdPerson(Entity::EntityState* entityState, Input::InputState* input) {
//    Entity::Entity* camera = entityState->entities[1];
//
//    if (input->keys[GLFW_KEY_W]) {
//        camera->spatial3D_Position.z += 0.1f;
//    }
//    if (input->keys[GLFW_KEY_S]) {
//        camera->spatial3D_Position.z -= 0.1f;
//    }
//    if (input->keys[GLFW_KEY_A]) {
//        camera->spatial3D_Position.x += 0.1f;
//    }
//    if (input->keys[GLFW_KEY_D]) {
//        camera->spatial3D_Position.x -= 0.1f;
//    }
//    if (input->keys[GLFW_KEY_SPACE]) {
//        camera->spatial3D_Position.y -= 0.1f;
//    }
//    if (input->keys[GLFW_KEY_LEFT_SHIFT]) {
//        camera->spatial3D_Position.y += 0.1f;
//    }
//
//    f32 dx = (f32) (input->mouseX - input->oldMouseX);
//    f32 dy = (f32) (input->mouseY - input->oldMouseY);
//
//    f32 mouseSensitivity = 0.15f;
//    f32 verticalViewAngle = 90.0f;
//
//    if (camera->camera_VerticalAngle - dy * mouseSensitivity > verticalViewAngle ||
//        camera->camera_VerticalAngle - dy * mouseSensitivity < -verticalViewAngle) {
//        camera->camera_VerticalAngle = camera->spatial3D_Rotation.x;
//    } else {
//        camera->camera_VerticalAngle -= dy * mouseSensitivity;
//    }
//
//    f32 horizontalDistance = (f32) cos(camera->camera_VerticalAngle * M_PI / 180.0f);
//    f32 verticalDistance = (f32) sin(camera->camera_VerticalAngle * M_PI / 180.0f);
//
//    f32 objectRotationY = camera->spatial3D_Rotation.y;
//    f32 xOffset = (f32) (horizontalDistance * sin(objectRotationY * M_PI / 180.0f));
//    f32 zOffset = (f32) (horizontalDistance * cos(objectRotationY * M_PI / 180.0f));
//
//    camera->spatial3D_Position.x -= xOffset;
//    camera->spatial3D_Position.y += verticalDistance;
//    camera->spatial3D_Position.z -= zOffset;
//
//    camera->spatial3D_Rotation = { camera->camera_VerticalAngle, 180 - objectRotationY, 0.0f };
//}

void handleCameraFirstPerson(ECS::EntityComponentSystem* ecs, Input::InputState* input, f32 lastFrameTimeMs) {
    ECS::Entity* camera = ecs->entities[0];
    ECS::Component* spatial3d = camera->components[ECS::COMPONENT_TYPE_SPATIAL_3D];
    if (spatial3d == nullptr) {
        return;
    }
    ECS::ComponentInfo* spatial3dInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];
    if (spatial3dInfo == nullptr) {
        return;
    }

    Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
    Vector3f rotation = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION);

    f32 moveSpeed = 0.01f * lastFrameTimeMs;
    f32 mouseSensitivity = 0.2f;
    f32 verticalViewRange = 90.0f;

    f32 x = (f32) sin((180 - rotation.y) * M_PI / 180.0f) * moveSpeed;
    f32 z = (f32) cos((180 - rotation.y) * M_PI / 180.0f) * moveSpeed;

    if (input->keys[GLFW_KEY_W]) {
        position.x += -x;
        position.z += -z;
    }
    if (input->keys[GLFW_KEY_S]) {
        position.x += x;
        position.z += z;
    }
    if (input->keys[GLFW_KEY_A]) {
        position.x += -z;
        position.z += x;
    }
    if (input->keys[GLFW_KEY_D]) {
        position.x += z;
        position.z += -x;
    }
    if (input->keys[GLFW_KEY_SPACE]) {
        position.y -= moveSpeed;
    }
    if (input->keys[GLFW_KEY_LEFT_SHIFT]) {
        position.y += moveSpeed;
    }

    f32 dx = (f32) (input->mouseX - input->oldMouseX);
    f32 dy = (f32) (input->mouseY - input->oldMouseY);

    f32 newRotationX = rotation.x - dy * mouseSensitivity;
    if (newRotationX > verticalViewRange || newRotationX < - verticalViewRange) {
        dy = 0;
    }

    rotation.x += dy * mouseSensitivity;
    rotation.y += dx * mouseSensitivity;
    rotation.x = (f32) fmod(rotation.x, 360.0f);
    rotation.y = (f32) fmod(rotation.y, 360.0f);

    ECS::setField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION, position);
    ECS::setField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION, rotation);
}

void Atlas::render() {
    while (!Render::shouldCloseWindow(window)) {
        Render::updateWindow(window);
        if (window->resized) {
            glViewport(0, 0, window->width, window->height);
            window->resized = false;
        }
        if (input->keys[GLFW_KEY_ESCAPE]) {
            Render::closeWindow(window);
            return;
        }
        if (input->mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
            glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        handleCameraFirstPerson(ecs, input, window->lastFrameTimeMs);
        Input::updateInputState(input);
        Render::render(window, assetPack, ecs);
    }
    Render::closeWindow(window);
}

bool Atlas::addMeshToAssetPack(Asset::MeshAsset* asset) {
    if (!asset) return false;
    UUID assetId = assetPack->nextMeshAssetId;
    asset->assetId = assetId;
    assetPack->meshAssets[assetId] = asset;
    assetPack->numMeshAssets++;
    assetPack->nextMeshAssetId++;
    return true;
}

bool Atlas::addMaterialToAssetPack(Asset::MaterialAsset* asset) {
    if (!asset) return false;
    UUID assetId = assetPack->nextMaterialAssetId;
    asset->assetId = assetId;
    assetPack->materialAssets[assetId] = asset;
    assetPack->numMaterialAssets++;
    assetPack->nextMaterialAssetId++;
    return true;
}
bool Atlas::addTextureToAssetPack(Asset::TextureAsset* asset) {
    if (!asset) return false;
    UUID assetId = assetPack->nextTextureAssetId;
    asset->assetId = assetId;
    assetPack->textureAssets[assetId] = asset;
    assetPack->numTextureAssets++;
    assetPack->nextTextureAssetId++;
    return true;
}

bool Atlas::addShaderToAssetPack(Asset::ShaderAsset* asset) {
    if (!asset) return false;
    UUID assetId = assetPack->nextShaderAssetId;
    asset->assetId = assetId;
    assetPack->shaderAssets[assetId] = asset;
    assetPack->numTextureAssets++;
    assetPack->nextShaderAssetId++;
    return true;
}

UUID Atlas::createEntity() {
    UUID entityId = ecs->nextEntityId;
    if (entityId == -1) return -1;
    ECS::Entity* entity = new ECS::Entity();
    entity->id = entityId;
    ecs->entities[entityId] = entity;
    ecs->numEntities++;
    i32 nextEntityId = entityId;
    do {
        nextEntityId = (nextEntityId + 1) % ECS::MAX_ENTITIES;
        if (nextEntityId == ecs->nextEntityId) {
            nextEntityId = -1;
            break;
        }
    } while (ecs->entities[nextEntityId] != nullptr);
    ecs->nextEntityId = nextEntityId;
    return entityId;
}

bool Atlas::addSpatial3dComponentToEntity(UUID entityId, Vector3f position, Vector3f rotation, f32 scale) {
    if (entityId < 0 || entityId >= ECS::MAX_ENTITIES) return false;
    ECS::Entity* entity = ecs->entities[entityId];
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];
    if (entity == nullptr || componentInfo == nullptr) return false;
    ECS::Component* component = new ECS::Component();
    component->type = ECS::COMPONENT_TYPE_SPATIAL_3D;
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_POSITION, position);
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION, rotation);
    ECS::setField_f32(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_SCALE, scale);
    entity->components[component->type] = component;
    return true;
}

bool Atlas::addRenderable3dComponentToEntity(UUID entityId, UUID meshAssetId, UUID materialAssetId, i32 textureAtlasIndex) {
    if (entityId < 0 || entityId >= ECS::MAX_ENTITIES) return false;
    ECS::Entity* entity = ecs->entities[entityId];
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_RENDERABLE_3D];
    if (entity == nullptr || componentInfo == nullptr) return false;
    ECS::Component* component = new ECS::Component();
    component->type = ECS::COMPONENT_TYPE_RENDERABLE_3D;
    ECS::setField_i32(component, componentInfo, ECS::Renderable3d::FIELD_INDEX_MESH_ASSET_ID, meshAssetId);
    ECS::setField_i32(component, componentInfo, ECS::Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID, materialAssetId);
    ECS::setField_i32(component, componentInfo, ECS::Renderable3d::FIELD_INDEX_TEXTURE_ATLAS_INDEX, textureAtlasIndex);
    entity->components[component->type] = component;
    return true;
}



