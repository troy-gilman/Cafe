#include "MathUtils.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cstring>
#include <iostream>


glm::f32mat4 MathUtils::createProjectionMatrix(f32 fov, f32 nearPlane, f32 farPlane, f32 aspectRatio) {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void MathUtils::normalize(Vector3f &vector) {
    const f32 length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    vector.x /= length;
    vector.y /= length;
    vector.z /= length;
}

void MathUtils::transformPoint(const Vector3f& point, const Matrix4f& matrix, Vector3f& result) {
    result.x = point.x * matrix.data[0][0] + point.y * matrix.data[1][0] + point.z * matrix.data[2][0] + matrix.data[3][0];
    result.y = point.x * matrix.data[0][1] + point.y * matrix.data[1][1] + point.z * matrix.data[2][1] + matrix.data[3][1];
    result.z = point.x * matrix.data[0][2] + point.y * matrix.data[1][2] + point.z * matrix.data[2][2] + matrix.data[3][2];
}

void MathUtils::createCameraFrustum(const Matrix4f& view, const Matrix4f& projection, Frustum& result) {
    Matrix4f viewProjection{};
    multiplyMatrix(view, projection, viewProjection);

    result.leftPlane.normal.x = viewProjection.data[0][3] + viewProjection.data[0][0];
    result.leftPlane.normal.y = viewProjection.data[1][3] + viewProjection.data[1][0];
    result.leftPlane.normal.z = viewProjection.data[2][3] + viewProjection.data[2][0];
    result.leftPlane.distance = viewProjection.data[3][3] + viewProjection.data[3][0];

    result.rightPlane.normal.x = viewProjection.data[0][3] - viewProjection.data[0][0];
    result.rightPlane.normal.y = viewProjection.data[1][3] - viewProjection.data[1][0];
    result.rightPlane.normal.z = viewProjection.data[2][3] - viewProjection.data[2][0];
    result.rightPlane.distance = viewProjection.data[3][3] - viewProjection.data[3][0];

    result.bottomPlane.normal.x = viewProjection.data[0][3] + viewProjection.data[0][1];
    result.bottomPlane.normal.y = viewProjection.data[1][3] + viewProjection.data[1][1];
    result.bottomPlane.normal.z = viewProjection.data[2][3] + viewProjection.data[2][1];
    result.bottomPlane.distance = viewProjection.data[3][3] + viewProjection.data[3][1];

    result.topPlane.normal.x = viewProjection.data[0][3] - viewProjection.data[0][1];
    result.topPlane.normal.y = viewProjection.data[1][3] - viewProjection.data[1][1];
    result.topPlane.normal.z = viewProjection.data[2][3] - viewProjection.data[2][1];
    result.topPlane.distance = viewProjection.data[3][3] - viewProjection.data[3][1];

    result.nearPlane.normal.x = viewProjection.data[0][2];
    result.nearPlane.normal.y = viewProjection.data[1][2];
    result.nearPlane.normal.z = viewProjection.data[2][2];
    result.nearPlane.distance = viewProjection.data[3][2];

    result.farPlane.normal.x = viewProjection.data[0][3] - viewProjection.data[0][2];
    result.farPlane.normal.y = viewProjection.data[1][3] - viewProjection.data[1][2];
    result.farPlane.normal.z = viewProjection.data[2][3] - viewProjection.data[2][2];
    result.farPlane.distance = viewProjection.data[3][3] - viewProjection.data[3][2];

    // Normalize the plane coefficients
    normalizePlane(result.leftPlane);
    normalizePlane(result.rightPlane);
    normalizePlane(result.bottomPlane);
    normalizePlane(result.topPlane);
    normalizePlane(result.nearPlane);
    normalizePlane(result.farPlane);
}

void MathUtils::normalizePlane(Plane& plane) {
    const f32 length = sqrtf(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);
    plane.normal.x /= length;
    plane.normal.y /= length;
    plane.normal.z /= length;
    plane.distance /= length;
}

f32 MathUtils::distanceToPlane(const Plane& plane, const Vector3f& point) {
    return plane.normal.x * point.x + plane.normal.y * point.y + plane.normal.z * point.z + plane.distance;
}

bool MathUtils::isPointInFrustum(const Frustum& frustum, const Vector3f& point) {
    return distanceToPlane(frustum.leftPlane, point) > 0.0f &&
           distanceToPlane(frustum.rightPlane, point) > 0.0f &&
           distanceToPlane(frustum.bottomPlane, point) > 0.0f &&
           distanceToPlane(frustum.topPlane, point) > 0.0f &&
           distanceToPlane(frustum.nearPlane, point) > 0.0f &&
           distanceToPlane(frustum.farPlane, point) > 0.0f;
}

bool MathUtils::isSphereInFrustum(const Frustum& frustum, const Vector3f& center, f32 radius) {
    return MathUtils::distanceToPlane(frustum.leftPlane, center) >= -radius &&
            MathUtils::distanceToPlane(frustum.rightPlane, center) >= -radius &&
            MathUtils::distanceToPlane(frustum.bottomPlane, center) >= -radius &&
            MathUtils::distanceToPlane(frustum.topPlane, center) >= -radius &&
            MathUtils::distanceToPlane(frustum.nearPlane, center) >= -radius &&
            MathUtils::distanceToPlane(frustum.farPlane, center) >= -radius;
}

void MathUtils::setIdentity(Matrix4f& matrix) {
    memset(matrix.data, 0, sizeof(matrix.data));
    matrix.data[0][0] = 1.0f;
    matrix.data[1][1] = 1.0f;
    matrix.data[2][2] = 1.0f;
    matrix.data[3][3] = 1.0f;
}

void MathUtils::translateMatrix(const Matrix4f& matrix, const Vector3f& translation, Matrix4f& result) {
    result = matrix;
    result.data[3][0] = matrix.data[0][0] * translation.x + matrix.data[1][0] * translation.y + matrix.data[2][0] * translation.z + matrix.data[3][0];
    result.data[3][1] = matrix.data[0][1] * translation.x + matrix.data[1][1] * translation.y + matrix.data[2][1] * translation.z + matrix.data[3][1];
    result.data[3][2] = matrix.data[0][2] * translation.x + matrix.data[1][2] * translation.y + matrix.data[2][2] * translation.z + matrix.data[3][2];
}

void MathUtils::rotateMatrix(const Matrix4f &matrix, f32 angleDegrees, const Vector3f &axis, Matrix4f& result) {
    Matrix4f rotationMatrix;
    setIdentity(rotationMatrix);

    f32 angle = (f32) (angleDegrees * M_PI / 180.0);
    f32 cos = (f32) cosf(angle);
    f32 sin = (f32) sinf(angle);
    f32 C = 1.0f - cos;

    rotationMatrix.data[0][0] = cos + axis.x * axis.x * C;
    rotationMatrix.data[0][1] = axis.y * axis.x * C + axis.z * sin;
    rotationMatrix.data[0][2] = axis.z * axis.x * C - axis.y * sin;

    rotationMatrix.data[1][0] = axis.x * axis.y * C - axis.z * sin;
    rotationMatrix.data[1][1] = cos + axis.y * axis.y * C;
    rotationMatrix.data[1][2] = axis.z * axis.y * C + axis.x * sin;

    rotationMatrix.data[2][0] = axis.x * axis.z * C + axis.y * sin;
    rotationMatrix.data[2][1] = axis.y * axis.z * C - axis.x * sin;
    rotationMatrix.data[2][2] = cos + axis.z * axis.z * C;

    multiplyMatrix(rotationMatrix, matrix, result);
}

void MathUtils::multiplyMatrix(const Matrix4f &left, const Matrix4f &right, Matrix4f &result) {
    Matrix4f temp{};
    for (ui32 i = 0; i < 4; i++) {
        for (ui32 j = 0; j < 4; j++) {
            temp.data[i][j] = left.data[i][0] * right.data[0][j] +
                                left.data[i][1] * right.data[1][j] +
                                left.data[i][2] * right.data[2][j] +
                                left.data[i][3] * right.data[3][j];
        }
    }
    result = temp;
}

void MathUtils::createModelMatrix(const Vector3f& position, const Vector3f& rotation, f32 scale, Matrix4f& result) {
    MathUtils::setIdentity(result);
    MathUtils::rotateMatrix(result, rotation.x, {1.0f, 0, 0}, result);
    MathUtils::rotateMatrix(result, rotation.y, {0, 1.0f, 0}, result);
    MathUtils::rotateMatrix(result, rotation.z, {0, 0, 1.0f}, result);
    MathUtils::translateMatrix(result, position, result);
}

void MathUtils::printMatrix(const Matrix4f &matrix) {
    std::cout << "Matrix: " << std::endl;
    for (ui32 i = 0; i < 4; i++) {
        std::cout << "[ ";
        for (ui32 j = 0; j < 4; j++) {
            std::cout << matrix.data[i][j] << " ";
        }
        std::cout << "]" << std::endl;
    }
}

void MathUtils::printMatrix(const glm::f32mat4& matrix) {
    std::cout << "GLM Matrix: " << std::endl;
    for (ui32 i = 0; i < 4; i++) {
        std::cout << "[ ";
        for (ui32 j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "]" << std::endl;
    }
}

void MathUtils::glmToMatrix4f(const glm::f32mat4 &glmMatrix, Matrix4f &result) {
    for (ui32 i = 0; i < 4; i++) {
        for (ui32 j = 0; j < 4; j++) {
            result.data[i][j] = glmMatrix[i][j];
        }
    }
}
