#include "Geometry.h"
#include "util/MathUtils.h"

void Geometry::setupQuadPrismMeshData(GeometryMeshData& result) {
    // Front face
    result.indices[0] = 2;
    result.indices[1] = 1;
    result.indices[2] = 0;
    result.indices[3] = 2;
    result.indices[4] = 0;
    result.indices[5] = 3;

    // Back face
    result.indices[6] = 4;
    result.indices[7] = 5;
    result.indices[8] = 6;
    result.indices[9] = 7;
    result.indices[10] = 4;
    result.indices[11] = 6;

    // Left face
    result.indices[12] = 4;
    result.indices[13] = 3;
    result.indices[14] = 0;
    result.indices[15] = 3;
    result.indices[16] = 4;
    result.indices[17] = 7;

    // Right face
    result.indices[18] = 1;
    result.indices[19] = 2;
    result.indices[20] = 5;
    result.indices[21] = 6;
    result.indices[22] = 5;
    result.indices[23] = 2;

    // Top face
    result.indices[24] = 3;
    result.indices[25] = 6;
    result.indices[26] = 2;
    result.indices[27] = 3;
    result.indices[28] = 7;
    result.indices[29] = 6;

    // Bottom face
    result.indices[30] = 1;
    result.indices[31] = 5;
    result.indices[32] = 0;
    result.indices[33] = 5;
    result.indices[34] = 4;
    result.indices[35] = 0;
}

void Geometry::copyMeshDataWithOffsets(const Vector3f* srcVertices, Vector3f* destVertices, ui32 vertexOffset, const ui32* srcIndices, ui32* destIndices, ui32 indexOffset) {
    for (ui32 i = 0; i < QUAD_PRISM_MESH_NUM_VERTICES; i++) {
        destVertices[vertexOffset + i] = srcVertices[i];
    }
    for (ui32 i = 0; i < QUAD_PRISM_MESH_NUM_INDICES; i++) {
        destIndices[indexOffset + i] = srcIndices[i] + vertexOffset;
    }
}

void Geometry::normalizePlane(Plane& plane) {
    const f32 length = sqrtf(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);
    plane.normal.x /= length;
    plane.normal.y /= length;
    plane.normal.z /= length;
    plane.distance /= length;
}

f32 Geometry::distanceToPlane(const Plane& plane, const Vector3f& point) {
    return plane.normal.x * point.x + plane.normal.y * point.y + plane.normal.z * point.z + plane.distance;
}

void Geometry::getPlaneIntersection(const Plane& plane1, const Plane& plane2, const Plane& plane3, Vector3f& result) {
    const f32 denominator = plane1.normal.x * (plane2.normal.y * plane3.normal.z - plane3.normal.y * plane2.normal.z) -
                            plane2.normal.x * (plane1.normal.y * plane3.normal.z - plane3.normal.y * plane1.normal.z) +
                            plane3.normal.x * (plane1.normal.y * plane2.normal.z - plane2.normal.y * plane1.normal.z);
    if (denominator == 0.0f) {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        return;
    }
    const f32 inverseDenominator = 1.0f / denominator;
    result.x = (plane1.distance * (plane2.normal.y * plane3.normal.z - plane3.normal.y * plane2.normal.z) -
                plane2.distance * (plane1.normal.y * plane3.normal.z - plane3.normal.y * plane1.normal.z) +
                plane3.distance * (plane1.normal.y * plane2.normal.z - plane2.normal.y * plane1.normal.z)) * inverseDenominator;
    result.y = (plane1.normal.x * (plane2.distance * plane3.normal.z - plane3.distance * plane2.normal.z) -
                plane2.normal.x * (plane1.distance * plane3.normal.z - plane3.distance * plane1.normal.z) +
                plane3.normal.x * (plane1.distance * plane2.normal.z - plane2.distance * plane1.normal.z)) * inverseDenominator;
    result.z = (plane1.normal.x * (plane2.normal.y * plane3.distance - plane3.normal.y * plane2.distance) -
                plane2.normal.x * (plane1.normal.y * plane3.distance - plane3.normal.y * plane1.distance) +
                plane3.normal.x * (plane1.normal.y * plane2.distance - plane2.normal.y * plane1.distance)) * inverseDenominator;
}

void Geometry::createFrustum(const Matrix4f& view, const Matrix4f& projection, Frustum& result) {
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
    normalizePlane(result.leftPlane);
    normalizePlane(result.rightPlane);
    normalizePlane(result.bottomPlane);
    normalizePlane(result.topPlane);
    normalizePlane(result.nearPlane);
    normalizePlane(result.farPlane);

    loadFrustumMesh(result);
}

void Geometry::getFrustumPoints(const Frustum& frustum, Vector3f* result) {
    Vector3f ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

    // Near plane
    getPlaneIntersection(frustum.leftPlane, frustum.topPlane, frustum.nearPlane, ntl);
    getPlaneIntersection(frustum.rightPlane, frustum.topPlane, frustum.nearPlane, ntr);
    getPlaneIntersection(frustum.leftPlane, frustum.bottomPlane, frustum.nearPlane, nbl);
    getPlaneIntersection(frustum.rightPlane, frustum.bottomPlane, frustum.nearPlane, nbr);

    // Far plane
    getPlaneIntersection(frustum.leftPlane, frustum.topPlane, frustum.farPlane, ftl);
    getPlaneIntersection(frustum.rightPlane, frustum.topPlane, frustum.farPlane, ftr);
    getPlaneIntersection(frustum.leftPlane, frustum.bottomPlane, frustum.farPlane, fbl);
    getPlaneIntersection(frustum.rightPlane, frustum.bottomPlane, frustum.farPlane, fbr);

    // Store the points in the result array
    result[0] = ntl;
    result[1] = ntr;
    result[2] = nbl;
    result[3] = nbr;
    result[4] = ftl;
    result[5] = ftr;
    result[6] = fbl;
    result[7] = fbr;
}

void Geometry::loadFrustumMesh(const Frustum &frustum) {
    Geometry::GeometryMeshData frustumMeshData{};
    getFrustumPoints(frustum, frustumMeshData.vertices);
    Geometry::setupQuadPrismMeshData(frustumMeshData);
    // TODO: Create a mesh from the prismMeshData
}

bool Geometry::isPointInFrustum(const Frustum& frustum, const Vector3f& point) {
    return distanceToPlane(frustum.leftPlane, point) > 0.0f &&
           distanceToPlane(frustum.rightPlane, point) > 0.0f &&
           distanceToPlane(frustum.bottomPlane, point) > 0.0f &&
           distanceToPlane(frustum.topPlane, point) > 0.0f &&
           distanceToPlane(frustum.nearPlane, point) > 0.0f &&
           distanceToPlane(frustum.farPlane, point) > 0.0f;
}

bool Geometry::isSphereInFrustum(const Frustum& frustum, const Vector3f& center, f32 radius) {
    return distanceToPlane(frustum.leftPlane, center) >= -radius &&
           distanceToPlane(frustum.rightPlane, center) >= -radius &&
           distanceToPlane(frustum.bottomPlane, center) >= -radius &&
           distanceToPlane(frustum.topPlane, center) >= -radius &&
           distanceToPlane(frustum.nearPlane, center) >= -radius &&
           distanceToPlane(frustum.farPlane, center) >= -radius;
}