#include "Cafe.h"

#include <iostream>
#include "util/MapUtils.h"

void Cafe::init(EngineState* engine) {
    Render::initWindow(engine->renderState.window);
    Input::initInputState(engine->input, engine->renderState.window.glfwWindow);
    ECS::initEntityComponentSystem(engine->ecs);
}

void Cafe::destroy(EngineState* engine) {
    Render::closeWindow(engine->renderState.window);
}

void Cafe::update(EngineState* engine) {
    Render::updateWindow(engine->renderState.window);
    Input::updateInputState(engine->input);
    Trait::handleController1p_Move(engine->ecs, engine->input, engine->renderState.window.lastFrameTimeMs);
}

void Cafe::render(EngineState* engine) {
    Render::render(engine->renderState, engine->assetPack, engine->ecs);
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


