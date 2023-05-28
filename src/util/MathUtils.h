#include "glm/glm.hpp"
#include "Types.h"

namespace MathUtils {
    glm::f32mat4 createProjectionMatrix(f32 fov, f32 nearPlane, f32 farPlane, f32 aspectRatio);
    void normalize(Vector3f& vector);
    void transformPoint(const Vector3f& point, const Matrix4f& matrix, Vector3f& result);
    void setIdentity(Matrix4f& matrix);
    void translateMatrix(const Matrix4f& matrix, const Vector3f& translation, Matrix4f& result);
    void rotateMatrix(const Matrix4f &matrix, f32 angleDegrees, const Vector3f &axis, Matrix4f& result);
    void multiplyMatrix(const Matrix4f& left, const Matrix4f& right, Matrix4f& result);
    void createModelMatrix(const Vector3f& position, const Vector3f& rotation, f32 scale, Matrix4f& result);
    void printMatrix(const Matrix4f& matrix);
    void printMatrix(const glm::f32mat4& matrix);
    void glmToMatrix4f(const glm::f32mat4& glmMatrix, Matrix4f& result);
}