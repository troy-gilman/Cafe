#include "MathUtils.h"
#include "glm/gtc/matrix_transform.hpp"


glm::f32mat4 MathUtils::createProjectionMatrix(f32 fov, f32 nearPlane, f32 farPlane, f32 aspectRatio) {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}
