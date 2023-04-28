#include "glm/glm.hpp"
#include "Types.h"

namespace MathUtils {
    glm::f32mat4 createProjectionMatrix(f32 fov, f32 nearPlane, f32 farPlane, f32 aspectRatio);
}