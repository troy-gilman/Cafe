#include "Atlas.h"

#include <iostream>
#include "util/MapUtils.h"

void Atlas::init(EngineState* engineState) {
    Render::initWindow(engineState->renderState.window);
    Input::initInputState(engineState->input, engineState->renderState.window.glfwWindow);
    ECS::initEntityComponentSystem(engineState->ecs);
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

void handleCameraFirstPerson(ECS::EntityComponentSystem& ecs, const Input::InputState& input, f32 lastFrameTimeMs) {
    UUID cameraEntityId = 0;
    if (!ecs.activeComponents[ECS::COMPONENT_TYPE_SPATIAL_3D][cameraEntityId]) return;
    ECS::Component& spatial3d = ecs.components[ECS::COMPONENT_TYPE_SPATIAL_3D][cameraEntityId];
    const ECS::ComponentInfo& spatial3dInfo = ecs.componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];

    Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
    Vector3f rotation = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION);

    f32 moveSpeed = 0.01f * lastFrameTimeMs;
    f32 mouseSensitivity = 0.2f;
    f32 verticalViewRange = 90.0f;

    f32 x = (f32) sin((180 - rotation.y) * M_PI / 180.0f) * moveSpeed;
    f32 z = (f32) cos((180 - rotation.y) * M_PI / 180.0f) * moveSpeed;

    if (input.keys[GLFW_KEY_W]) {
        position.x += -x;
        position.z += -z;
    }
    if (input.keys[GLFW_KEY_S]) {
        position.x += x;
        position.z += z;
    }
    if (input.keys[GLFW_KEY_A]) {
        position.x += -z;
        position.z += x;
    }
    if (input.keys[GLFW_KEY_D]) {
        position.x += z;
        position.z += -x;
    }
    if (input.keys[GLFW_KEY_SPACE]) {
        position.y -= moveSpeed;
    }
    if (input.keys[GLFW_KEY_LEFT_SHIFT]) {
        position.y += moveSpeed;
    }

    f32 dx = (f32) (input.mouseX - input.oldMouseX);
    f32 dy = (f32) (input.mouseY - input.oldMouseY);

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

void Atlas::render(EngineState* engineState) {
    Render::Window& window = engineState->renderState.window;
    Input::InputState& input = engineState->input;
    while (!Render::shouldCloseWindow(window)) {
        Render::updateWindow(window);
        if (window.resized) {
            glViewport(0, 0, window.width, window.height);
            window.resized = false;
        }
        if (input.keys[GLFW_KEY_ESCAPE]) {
            Render::closeWindow(window);
            return;
        }
        if (input.mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
            glfwSetInputMode(window.glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        handleCameraFirstPerson(engineState->ecs, input, window.lastFrameTimeMs);
        Render::render(engineState->renderState, engineState->assetPack, engineState->ecs);
        Input::updateInputState(input);
    }
    Render::closeWindow(window);
}
