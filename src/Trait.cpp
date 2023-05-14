#include "Cafe.h"

using namespace Cafe;

void Trait::handleController1p_Move(ECS::EntityComponentSystem& ecs, const Input::InputState& input, f32 lastFrameTimeMs) {
    const ECS::ComponentInfo& controller1pInfo = ecs.componentTypes[ECS::COMPONENT_TYPE_CONTROLLER_1P];
    const ECS::ComponentInfo& spatial3dInfo = ecs.componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];

    for (UUID entityId = 0; entityId < ECS::MAX_ENTITIES; entityId++) {
        if (!ecs.activeComponents[ECS::COMPONENT_TYPE_CONTROLLER_1P][entityId]) return;
        ECS::Component& controller1p = ecs.components[ECS::COMPONENT_TYPE_CONTROLLER_1P][entityId];

        if (!ecs.activeComponents[ECS::COMPONENT_TYPE_SPATIAL_3D][entityId]) return;
        ECS::Component& spatial3d = ecs.components[ECS::COMPONENT_TYPE_SPATIAL_3D][entityId];

        f32 moveSpeed = ECS::getField_f32(controller1p, controller1pInfo, ECS::Controller1p::FIELD_INDEX_MOVE_SPEED);
        f32 mouseSensitivity = ECS::getField_f32(controller1p, controller1pInfo, ECS::Controller1p::FIELD_INDEX_MOUSE_SENSITIVITY);
        Vector2f verticalViewRange = ECS::getField_Vector2f(controller1p, controller1pInfo, ECS::Controller1p::FIELD_INDEX_VERTICAL_VIEW_RANGE);
        f32 adjustedMoveSpeed = moveSpeed * lastFrameTimeMs;

        Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
        Vector3f rotation = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION);

        f32 x = (f32) sin((180 - rotation.y) * M_PI / 180.0f) * adjustedMoveSpeed;
        f32 z = (f32) cos((180 - rotation.y) * M_PI / 180.0f) * adjustedMoveSpeed;

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
            position.y -= adjustedMoveSpeed;
        }
        if (input.keys[GLFW_KEY_LEFT_SHIFT]) {
            position.y += adjustedMoveSpeed;
        }

        f32 dx = (f32) (input.mouseX - input.oldMouseX);
        f32 dy = (f32) (input.mouseY - input.oldMouseY);

        f32 newRotationX = rotation.x + dy * mouseSensitivity;
        if (newRotationX >= verticalViewRange.x) {
            rotation.x = verticalViewRange.x;
        } else if (newRotationX <= verticalViewRange.y) {
            rotation.x = verticalViewRange.y;
        } else {
            rotation.x = newRotationX;
        }

        rotation.y += dx * mouseSensitivity;
        rotation.x = (f32) fmod(rotation.x, 360.0f);
        rotation.y = (f32) fmod(rotation.y, 360.0f);

        ECS::setField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION, position);
        ECS::setField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION, rotation);
    }
}