#include "Frustum.h"
#include "util/MathUtils.h"


void Frustum::createCameraFrustum(const Matrix4f& view, const Matrix4f& projection, Frustum& result) {
    Matrix4f viewProjection{};
    MathUtils::multiplyMatrix(view, projection, viewProjection);

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
    MathUtils::normalizePlane(result.leftPlane);
    MathUtils::normalizePlane(result.rightPlane);
    MathUtils::normalizePlane(result.bottomPlane);
    MathUtils::normalizePlane(result.topPlane);
    MathUtils::normalizePlane(result.nearPlane);
    MathUtils::normalizePlane(result.farPlane);
}

bool Frustum::isPointInFrustum(const Frustum& frustum, const Vector3f& point) {
    return MathUtils::distanceToPlane(frustum.leftPlane, point) > 0.0f &&
            MathUtils::distanceToPlane(frustum.rightPlane, point) > 0.0f &&
            MathUtils::distanceToPlane(frustum.bottomPlane, point) > 0.0f &&
            MathUtils::distanceToPlane(frustum.topPlane, point) > 0.0f &&
            MathUtils::distanceToPlane(frustum.nearPlane, point) > 0.0f &&
            MathUtils::distanceToPlane(frustum.farPlane, point) > 0.0f;
}

bool Frustum::isSphereInFrustum(const Frustum& frustum, const Vector3f& center, f32 radius) {
    return MathUtils::distanceToPlane(frustum.leftPlane, center) >= -radius &&
           MathUtils::distanceToPlane(frustum.rightPlane, center) >= -radius &&
           MathUtils::distanceToPlane(frustum.bottomPlane, center) >= -radius &&
           MathUtils::distanceToPlane(frustum.topPlane, center) >= -radius &&
           MathUtils::distanceToPlane(frustum.nearPlane, center) >= -radius &&
           MathUtils::distanceToPlane(frustum.farPlane, center) >= -radius;
}