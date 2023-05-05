#include "Atlas.h"
#include <thread>
#include "util/UUIDGenerator.h"
#include "EventHandling.h"
#include <iostream>
#include "util/MapUtils.h"

Atlas::Atlas(bool isServer) : isServer(isServer) {
    eventState = new Event::EventState(10000);
    ecs = new ECS::EntityComponentSystem();
    assetPack = new Asset::AssetPack();
    window = new Render::Window();
    input = new Input::InputState();
    networkState = new Network::NetworkState();
}

Atlas::~Atlas() {
    delete eventState;
    delete ecs;
    delete assetPack;
    delete window;
    delete networkState;
}

void Atlas::init() {
    Render::initWindow(window);
    Input::initInputState(input, window->glfwWindow);
    ECS::initComponentTypes(ecs);

    ECS::Entity* camera = new ECS::Entity();
    camera->id = 0;
    ECS::Component* component = new ECS::Component();
    component->type = ECS::COMPONENT_TYPE_SPATIAL_3D;
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[component->type];
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_POSITION, {0, -2, -5});
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION, {0, 0, 0});
    ECS::setField_f32(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_SCALE, 1.0f);
    camera->components[component->type] = component;
    ecs->entities[camera->id] = camera;
}

static void networkControllerReadEventLoop(Event::EventState* eventState, Network::NetworkState* networkState, bool isServer) {
    if (isServer) {
        Network::startServer(networkState);
    } else {
        Network::startClient(networkState);
    }
    while (true) {
        Network::receiveEventMsg(networkState);
        Network::EventMsg* eventMsg = &(networkState->eventMsgIn);
        if (eventMsg->numEvents == 0) {
            continue;
        }
        for (ui64 i = 0; i < eventMsg->numEvents; i++) {
            Event::Event* event = eventState->eventPool.waitForObject();
            Event::copyEvent(&eventMsg->events[i], event);
            eventState->eventQueue.push(event);
        }
        eventMsg->numEvents = 0;
    }
}

static void eventCreatorLoop(Event::EventState* eventState, bool isServer, bool* done) {
    if (isServer) {
        *done = true;
        return;
    }
    UUIDGenerator* uuidGenerator = UUIDGenerator::getInstance();
    ui64 numEvents = 10000;
    for (ui64 i = 0; i < numEvents; i++) {
        Event::Event* event = eventState->eventPool.waitForObject();
        Event::resetEvent(event);

        Event::EventType eventType = static_cast<Event::EventType>(i % 6);
        event->eventType = eventType;

        UUID id = uuidGenerator->generateUUID();
        event->field1.field_UUID = id;

        eventState->eventQueue.push(event);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Done creating events" << std::endl;
    *done = true;
}

void Atlas::start() {
    bool done = false;
    std::thread eventReaderThread(networkControllerReadEventLoop, eventState, networkState, isServer);
    std::thread eventCreatorThread(eventCreatorLoop, eventState, isServer, &done);
    int eventsProcessed = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        // Process events in our queue
        if (!eventState->eventQueue.empty()) {
            bool success = EventHandling::handleNextEvent(eventState, ecs);
            if (success) {
                eventsProcessed++;
            }
            if (eventsProcessed % 1000 == 0) {
                std::cout << "Events successfully processed: " << eventsProcessed << std::endl;
            }
        }
        // Handle events that were processed
        if (!eventState->eventQueueProcessed.empty()) {
            Event::Event* event = eventState->eventQueueProcessed.pop();
            if (!isServer) {
                Network::EventMsg* eventMsg = &(networkState->eventMsgOut);
                int numEvents = eventMsg->numEvents;
                Event::copyEvent(event, &eventMsg->events[numEvents]);
                eventMsg->numEvents++;
                if (eventMsg->numEvents == Network::MAX_EVENTS_PER_MSG) {
                    Network::sendEventMsg(networkState);
                }
            }
            eventState->eventPool.returnObject(event);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        bool shouldExit = !isServer || (isServer && elapsed.count() > 30);
        if (done && eventState->eventQueue.empty() && shouldExit) {
            eventReaderThread.detach();
            eventCreatorThread.join();
            std::cout << "Successfully processed " << eventsProcessed << " events" << std::endl;
            return;
        }
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
    ECS::Component* spatial3d = MapUtils::getValueOrNullPtr(camera->components, ECS::COMPONENT_TYPE_SPATIAL_3D);
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
    if (!asset) {
        return false;
    }
    if (assetPack->meshAssets.find(asset->assetId) != assetPack->meshAssets.end()) {
        return false;
    }
    assetPack->meshAssets[asset->assetId] = asset;
    return true;
}

bool Atlas::addMaterialToAssetPack(Asset::MaterialAsset* asset) {
    if (!asset) {
        return false;
    }
    if (assetPack->meshAssets.find(asset->assetId) != assetPack->meshAssets.end()) {
        return false;
    }
    assetPack->materialAssets[asset->assetId] = asset;
    return true;
}
bool Atlas::addTextureToAssetPack(Asset::TextureAsset* asset) {
    if (!asset) {
        return false;
    }
    if (assetPack->meshAssets.find(asset->assetId) != assetPack->meshAssets.end()) {
        return false;
    }
    assetPack->textureAssets[asset->assetId] = asset;
    return true;
}

bool Atlas::addShaderToAssetPack(Asset::ShaderAsset* asset) {
    if (!asset) {
        return false;
    }
    if (assetPack->meshAssets.find(asset->assetId) != assetPack->meshAssets.end()) {
        return false;
    }
    assetPack->shaderAssets[asset->assetId] = asset;
    return true;
}

UUID Atlas::createEntity() {
    Event::Event* event = eventState->eventPool.waitForObject();
    event->eventType = Event::EventType::ENTITY_CREATE;
    UUID uuid = (i32) ecs->entities.size();
    event->field1.field_UUID = uuid;
    eventState->eventQueue.push(event);
    EventHandling::handleNextEvent(eventState, ecs);
    if (event->success) {
        return uuid;
    } else {
        return -1;
    }
}

bool Atlas::addComponentToEntity(UUID entityId, i32 componentType) {
    Event::Event* event = eventState->eventPool.waitForObject();
    event->eventType = Event::EventType::ENTITY_ADD_COMPONENT;
    event->field1.field_UUID = entityId;
    event->field2.field_Integer = componentType;
    eventState->eventQueue.push(event);
    EventHandling::handleNextEvent(eventState, ecs);
    return event->success;
}

bool Atlas::setComponentField(UUID entityId, i32 componentType, i32 fieldIndex, FieldUnion field) {
    Event::Event* event = eventState->eventPool.waitForObject();
    event->eventType = Event::EventType::COMPONENT_SET_FIELD;
    event->field1.field_UUID = entityId;
    event->field2.field_Integer = componentType;
    event->field3.field_Integer = fieldIndex;
    event->field4 = field;
    eventState->eventQueue.push(event);
    EventHandling::handleNextEvent(eventState, ecs);
    return event->success;
}

bool Atlas::addSpatial3dComponentToEntity(UUID entityId, Vector3f position, Vector3f rotation, f32 scale) {
    ECS::Entity* entity = ecs->entities[entityId];
    ECS::Component* component = new ECS::Component();
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];
    component->type = ECS::COMPONENT_TYPE_SPATIAL_3D;
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_POSITION, position);
    ECS::setField_Vector3f(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION, rotation);
    ECS::setField_f32(component, componentInfo, ECS::Spatial3d::FIELD_INDEX_SCALE, scale);
    entity->components[component->type] = component;
    return true;
}

bool Atlas::addRenderable3dComponentToEntity(UUID entityId, UUID meshAssetId, UUID materialAssetId, i32 textureAtlasIndex) {
    ECS::Entity* entity = ecs->entities[entityId];
    ECS::Component* component = new ECS::Component();
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_RENDERABLE_3D];
    component->type = ECS::COMPONENT_TYPE_RENDERABLE_3D;
    ECS::setField_i32(component, componentInfo, ECS::Renderable3d::FIELD_INDEX_MESH_ASSET_ID, meshAssetId);
    ECS::setField_i32(component, componentInfo, ECS::Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID, materialAssetId);
    ECS::setField_i32(component, componentInfo, ECS::Renderable3d::FIELD_INDEX_TEXTURE_ATLAS_INDEX, textureAtlasIndex);
    entity->components[component->type] = component;
    return true;
}



