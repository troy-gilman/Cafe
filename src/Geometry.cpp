#include "Geometry.h"
#include "util/MathUtils.h"

void Geometry::loadMesh(const GeometryMeshData& meshData, GeometryMesh& result) {
    ui32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    ui32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (meshData.numVertices * sizeof(Vector3f)), meshData.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ui32 ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (i64) (meshData.numIndices * sizeof(ui32)), meshData.indices, GL_STATIC_DRAW);

    result.vao = vao;
    result.vbo = vbo;
    result.ibo = ibo;
    result.numIndices = meshData.numIndices;
}

void Geometry::bindMesh(const GeometryMesh& mesh) {
    glBindVertexArray(mesh.vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
}

void Geometry::unbindMesh() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Geometry::initWireframeMeshData(QuadPrism& quadPrism, f32 lineThickness, GeometryMeshData& result) {
    i32 v = Geometry::QUAD_PRISM_MESH_NUM_VERTICES;
    i32 i = Geometry::QUAD_PRISM_MESH_NUM_INDICES;
    f32 lt = lineThickness;

    const Vector3f& p0 = quadPrism.p0;
    const Vector3f& p1 = quadPrism.p1;
    const Vector3f& p2 = quadPrism.p2;
    const Vector3f& p3 = quadPrism.p3;
    const Vector3f& p4 = quadPrism.p4;
    const Vector3f& p5 = quadPrism.p5;
    const Vector3f& p6 = quadPrism.p6;
    const Vector3f& p7 = quadPrism.p7;

    i32 cuboidCount = 12;
    result.numVertices = v * cuboidCount;
    result.numIndices = i * cuboidCount;

    GeometryMeshData lineMeshData{};

    // VERTICAL LINES BEGIN
    initQuadPrismMeshData(p0, {p3.x + lt, p3.y, p3.z + lt}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 0, lineMeshData.indices, result.indices, 0);

    initQuadPrismMeshData({p5.x - lineThickness, p5.y, p5.z - lineThickness}, p6, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, v, lineMeshData.indices, result.indices, i);

    initQuadPrismMeshData({p1.x - lineThickness, p1.y, p1.z + lineThickness}, p2, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 2 * v, lineMeshData.indices, result.indices, 2 * i);

    initQuadPrismMeshData({p4.x + lineThickness, p4.y, p4.z - lineThickness}, p7, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 3 * v, lineMeshData.indices, result.indices, 3 * i);
    // VERTICAL LINES END

    // BOTTOM HORIZONTAL LINES BEGIN
    initQuadPrismMeshData(p0, {p4.x + lineThickness, p4.y + lineThickness, p4.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 4 * v, lineMeshData.indices, result.indices, 4 * i);

    initQuadPrismMeshData({p1.x - lineThickness, p1.y, p1.z}, {p5.x, p5.y + lineThickness, p5.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 5 * v, lineMeshData.indices, result.indices, 5 * i);

    initQuadPrismMeshData({p0.x + lineThickness, p0.y, p0.z}, {p1.x - lineThickness, p1.y + lineThickness, p1.z + lineThickness}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 6 * v, lineMeshData.indices, result.indices, 6 * i);

    initQuadPrismMeshData({p4.x + lineThickness, p4.y, p4.z - lineThickness}, {p5.x - lineThickness, p5.y + lineThickness, p5.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 7 * v, lineMeshData.indices, result.indices, 7 * i);
    // BOTTOM HORIZONTAL LINES END

    // TOP HORIZONTAL LINES BEGIN
    initQuadPrismMeshData({p3.x, p3.y - lineThickness, p3.z}, {p7.x + lineThickness, p7.y, p7.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 8 * v, lineMeshData.indices, result.indices, 8 * i);

    initQuadPrismMeshData({p2.x - lineThickness, p2.y - lineThickness, p2.z}, p6, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 9 * v, lineMeshData.indices, result.indices, 9 * i);

    initQuadPrismMeshData({p3.x + lineThickness, p3.y - lineThickness, p3.z}, {p2.x - lineThickness, p2.y, p2.z + lineThickness}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 10 * v, lineMeshData.indices, result.indices, 10 * i);

    initQuadPrismMeshData({p7.x + lineThickness, p7.y - lineThickness, p7.z - lineThickness}, {p6.x - lineThickness, p6.y, p6.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, result.vertices, 11 * v, lineMeshData.indices, result.indices, 11 * i);
}

void Geometry::initQuadPrismMeshData(const Vector3f& min, const Vector3f& max, GeometryMeshData& result)  {
    result.numVertices = QUAD_PRISM_MESH_NUM_VERTICES;
    result.vertices[0] = {min.x, min.y, min.z};
    result.vertices[1] = {max.x, min.y, min.z};
    result.vertices[2] = {max.x, max.y, min.z};
    result.vertices[3] = {min.x, max.y, min.z};
    result.vertices[4] = {min.x, min.y, max.z};
    result.vertices[5] = {max.x, min.y, max.z};
    result.vertices[6] = {max.x, max.y, max.z};
    result.vertices[7] = {min.x, max.y, max.z};
    Geometry::initQuadPrismMeshDataIndices(result);
}

void Geometry::initQuadPrismMeshDataIndices(GeometryMeshData& result) {
    result.numIndices = QUAD_PRISM_MESH_NUM_INDICES;
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
}

void Geometry::loadFrustumMeshData(const Frustum& frustum, GeometryMeshData& result) {
    QuadPrism prism{};

    // Near plane
    getPlaneIntersection(frustum.leftPlane, frustum.topPlane, frustum.nearPlane, prism.p3);
    getPlaneIntersection(frustum.rightPlane, frustum.topPlane, frustum.nearPlane, prism.p2);
    getPlaneIntersection(frustum.leftPlane, frustum.bottomPlane, frustum.nearPlane, prism.p0);
    getPlaneIntersection(frustum.rightPlane, frustum.bottomPlane, frustum.nearPlane, prism.p1);

    // Far plane
    getPlaneIntersection(frustum.leftPlane, frustum.topPlane, frustum.farPlane, prism.p7);
    getPlaneIntersection(frustum.rightPlane, frustum.topPlane, frustum.farPlane, prism.p6);
    getPlaneIntersection(frustum.leftPlane, frustum.bottomPlane, frustum.farPlane, prism.p4);
    getPlaneIntersection(frustum.rightPlane, frustum.bottomPlane, frustum.farPlane, prism.p5);

    initWireframeMeshData(prism, 0.02f, result);
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

void Geometry::loadAABBMeshData(const AABB& aabb, GeometryMeshData& result) {
    QuadPrism prism {
        aabb.min.x, aabb.min.y, aabb.min.z,
        aabb.max.x, aabb.min.y, aabb.min.z,
        aabb.max.x, aabb.max.y, aabb.min.z,
        aabb.min.x, aabb.max.y, aabb.min.z,
        aabb.min.x, aabb.min.y, aabb.max.z,
        aabb.max.x, aabb.min.y, aabb.max.z,
        aabb.max.x, aabb.max.y, aabb.max.z,
        aabb.min.x, aabb.max.y, aabb.max.z
    };
    initWireframeMeshData(prism, 0.02f, result);
}