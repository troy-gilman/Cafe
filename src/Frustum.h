#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "util/Types.h"


namespace Frustum {

    struct Frustum {
        Plane topPlane;
        Plane bottomPlane;
        Plane rightPlane;
        Plane leftPlane;
        Plane farPlane;
        Plane nearPlane;
    };

    void createCameraFrustum(const Matrix4f& view, const Matrix4f& projection, Frustum& result);
    bool isPointInFrustum(const Frustum& frustum, const Vector3f& point);
    bool isSphereInFrustum(const Frustum& frustum, const Vector3f& center, f32 radius);

}

#endif // FRUSTUM_H