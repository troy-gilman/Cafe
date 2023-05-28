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

void Geometry::loadFrustumMesh(const Frustum &frustum) {
    Geometry::GeometryMeshData frustumMeshData{};
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

    // Store the points in the vertices array
    frustumMeshData.vertices[0] = ntl;
    frustumMeshData.vertices[1] = ntr;
    frustumMeshData.vertices[2] = nbl;
    frustumMeshData.vertices[3] = nbr;
    frustumMeshData.vertices[4] = ftl;
    frustumMeshData.vertices[5] = ftr;
    frustumMeshData.vertices[6] = fbl;
    frustumMeshData.vertices[7] = fbr;

    Geometry::initQuadPrismMeshDataIndices(frustumMeshData);
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

void Geometry::loadAABBMesh(AABB& aabb, f32 lineThickness) {
    Geometry::GeometryMeshData aabbMeshData{};
    const Vector3f& min = aabb.min;
    const Vector3f& max = aabb.max;

    i32 verticesPerCuboid = Geometry::QUAD_PRISM_MESH_NUM_VERTICES;
    i32 indicesPerCuboid = Geometry::QUAD_PRISM_MESH_NUM_INDICES;
    i32 cuboidCount = 12;

    aabbMeshData.numVertices = verticesPerCuboid * cuboidCount;
    aabbMeshData.numIndices = indicesPerCuboid * cuboidCount;

    GeometryMeshData lineMeshData{};

    // VERTICAL LINES BEGIN
    initQuadPrismMeshData({min.x, min.y, min.z}, {min.x + lineThickness, max.y, min.z + lineThickness}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 0, lineMeshData.indices, aabbMeshData.indices, 0);

    initQuadPrismMeshData({max.x - lineThickness, min.y, max.z - lineThickness}, {max.x, max.y, max.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, indicesPerCuboid);

    initQuadPrismMeshData({max.x - lineThickness, min.y, min.z + lineThickness}, {max.x, max.y, min.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 2*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 2*indicesPerCuboid);

    initQuadPrismMeshData({min.x + lineThickness, min.y, max.z - lineThickness}, {min.x, max.y, max.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 3*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 3*indicesPerCuboid);
    // VERTICAL LINES END

    // BOTTOM HORIZONTAL LINES BEGIN
    initQuadPrismMeshData({min.x, min.y, min.z}, {min.x + lineThickness, min.y + lineThickness, max.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 4*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 4*indicesPerCuboid);

    initQuadPrismMeshData({min.x + lineThickness, min.y, min.z}, {max.x - lineThickness, min.y + lineThickness, min.z + lineThickness}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 5*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 5*indicesPerCuboid);

    initQuadPrismMeshData({min.x + lineThickness, min.y, min.z}, {max.x - lineThickness, min.y + lineThickness, min.z + lineThickness}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 6*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 6*indicesPerCuboid);

    initQuadPrismMeshData({min.x + lineThickness, min.y, max.z - lineThickness}, {max.x - lineThickness, min.y + lineThickness, max.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 7*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 7*indicesPerCuboid);
    // BOTTOM HORIZONTAL LINES END

    // TOP HORIZONTAL LINES BEGIN
    initQuadPrismMeshData({min.x, max.y - lineThickness, min.z}, {min.x + lineThickness, max.y, max.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 8*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 8*indicesPerCuboid);

    initQuadPrismMeshData({max.x - lineThickness, max.y - lineThickness, min.z}, {max.x, max.y, max.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 9*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 9*indicesPerCuboid);

    initQuadPrismMeshData({min.x + lineThickness, max.y - lineThickness, min.z}, {max.x - lineThickness, max.y, min.z + lineThickness}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 10*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 10*indicesPerCuboid);

    initQuadPrismMeshData({min.x + lineThickness, max.y - lineThickness, max.z - lineThickness}, {max.x - lineThickness, max.y, max.z}, lineMeshData);
    copyMeshDataWithOffsets(lineMeshData.vertices, aabbMeshData.vertices, 11*verticesPerCuboid, lineMeshData.indices, aabbMeshData.indices, 11*indicesPerCuboid);
    // TOP HORIZONTAL LINES END

    loadMesh(aabbMeshData, aabb.mesh);

//    // Vertical lines
//    generateCuboid({min.x, min.y, min.z}, {min.x + lineThickness, max.y, min.z + lineThickness}, vertices, 0, indices, 0);
//    generateCuboid({max.x - lineThickness, min.y, max.z - lineThickness}, {max.x, max.y, max.z}, vertices, verticesPerCuboid, indices, indicesPerCuboid);
//    generateCuboid({max.x - lineThickness, min.y, min.z + lineThickness}, {max.x, max.y, min.z}, vertices, 2*verticesPerCuboid, indices, 2*indicesPerCuboid);
//    generateCuboid({min.x + lineThickness, min.y, max.z - lineThickness}, {min.x, max.y, max.z}, vertices, 3*verticesPerCuboid, indices, 3*indicesPerCuboid);
//
//    // Horizontal lines on the bottom
//    generateCuboid({min.x, min.y, min.z}, {min.x + lineThickness, min.y + lineThickness, max.z}, vertices, 4*verticesPerCuboid, indices, 4*indicesPerCuboid);
//    generateCuboid({max.x - lineThickness, min.y, min.z}, {max.x, min.y + lineThickness, max.z}, vertices, 5*verticesPerCuboid, indices, 5*indicesPerCuboid);
//    generateCuboid({min.x + lineThickness, min.y, min.z}, {max.x - lineThickness, min.y + lineThickness, min.z + lineThickness}, vertices, 6*verticesPerCuboid, indices, 6*indicesPerCuboid);
//    generateCuboid({min.x + lineThickness, min.y, max.z - lineThickness}, {max.x - lineThickness, min.y + lineThickness, max.z}, vertices, 7*verticesPerCuboid, indices, 7*indicesPerCuboid);
//
//    // Horizontal lines on the top
//    generateCuboid({min.x, max.y - lineThickness, min.z}, {min.x + lineThickness, max.y, max.z}, result.vertices, 8*verticesPerCuboid, result.indices, 8*indicesPerCuboid);
//    generateCuboid({max.x - lineThickness, max.y - lineThickness, min.z}, {max.x, max.y, max.z}, result.vertices, 9*verticesPerCuboid, result.indices, 9*indicesPerCuboid);
//    generateCuboid({min.x + lineThickness, max.y - lineThickness, min.z}, {max.x - lineThickness, max.y, min.z + lineThickness}, result.vertices, 10*verticesPerCuboid, result.indices, 10*indicesPerCuboid);
//    generateCuboid({min.x + lineThickness, max.y - lineThickness, max.z - lineThickness}, {max.x - lineThickness, max.y, max.z}, result.vertices, 11*verticesPerCuboid, result.indices, 11*indicesPerCuboid);
}