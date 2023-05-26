#include "AABBUtils.h"

void AABBUtils::loadAABBMesh(const Vector3f& min, const Vector3f& max, AABBMesh& result) {
    AABBMeshData meshData{};
    generateAABBMeshData(min, max, 0.02f, meshData);

    ui32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    ui32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (meshData.verticesCount * sizeof(Vector3f)), meshData.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ui32 ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (i64) (meshData.indicesCount * sizeof(ui32)), meshData.indices, GL_STATIC_DRAW);

    delete meshData.vertices;
    delete meshData.indices;

    result.vao = vao;
    result.vbo = vbo;
    result.ibo = ibo;
    result.numIndices = meshData.indicesCount;
}

void AABBUtils::bindAABBMesh(const AABBMesh& aabbMesh) {
    glBindVertexArray(aabbMesh.vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aabbMesh.ibo);
}

void AABBUtils::unbindAABBMesh() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

// Generates the vertices and indices for a cuboid with the given min and max points.
// The vertices and indices arrays must be allocated before calling this function.
// The vertices array must have at least 8 elements.
// The indices array must have at least 36 elements.
void generateCuboid(
        const Vector3f& min,
        const Vector3f& max,
        Vector3f* vertices,
        i32 verticesStartIndex,
        ui32* indices,
        i32 indicesStartIndex) {

    vertices[verticesStartIndex + 0] = {min.x, min.y, min.z};
    vertices[verticesStartIndex + 1] = {max.x, min.y, min.z};
    vertices[verticesStartIndex + 2] = {max.x, max.y, min.z};
    vertices[verticesStartIndex + 3] = {min.x, max.y, min.z};
    vertices[verticesStartIndex + 4] = {min.x, min.y, max.z};
    vertices[verticesStartIndex + 5] = {max.x, min.y, max.z};
    vertices[verticesStartIndex + 6] = {max.x, max.y, max.z};
    vertices[verticesStartIndex + 7] = {min.x, max.y, max.z};

    // Front face
    indices[indicesStartIndex + 0] = verticesStartIndex + 2;
    indices[indicesStartIndex + 1] = verticesStartIndex + 1;
    indices[indicesStartIndex + 2] = verticesStartIndex + 0;
    indices[indicesStartIndex + 3] = verticesStartIndex + 2;
    indices[indicesStartIndex + 4] = verticesStartIndex + 0;
    indices[indicesStartIndex + 5] = verticesStartIndex + 3;

    // Back face
    indices[indicesStartIndex + 6] = verticesStartIndex + 4;
    indices[indicesStartIndex + 7] = verticesStartIndex + 5;
    indices[indicesStartIndex + 8] = verticesStartIndex + 6;
    indices[indicesStartIndex + 9] = verticesStartIndex + 7;
    indices[indicesStartIndex + 10] = verticesStartIndex + 4;
    indices[indicesStartIndex + 11] = verticesStartIndex + 6;

    // Left face
    indices[indicesStartIndex + 12] = verticesStartIndex + 4;
    indices[indicesStartIndex + 13] = verticesStartIndex + 3;
    indices[indicesStartIndex + 14] = verticesStartIndex + 0;
    indices[indicesStartIndex + 15] = verticesStartIndex + 3;
    indices[indicesStartIndex + 16] = verticesStartIndex + 4;
    indices[indicesStartIndex + 17] = verticesStartIndex + 7;

    // Right face
    indices[indicesStartIndex + 18] = verticesStartIndex + 1;
    indices[indicesStartIndex + 19] = verticesStartIndex + 2;
    indices[indicesStartIndex + 20] = verticesStartIndex + 5;
    indices[indicesStartIndex + 21] = verticesStartIndex + 6;
    indices[indicesStartIndex + 22] = verticesStartIndex + 5;
    indices[indicesStartIndex + 23] = verticesStartIndex + 2;

    // Top face
    indices[indicesStartIndex + 24] = verticesStartIndex + 3;
    indices[indicesStartIndex + 25] = verticesStartIndex + 6;
    indices[indicesStartIndex + 26] = verticesStartIndex + 2;
    indices[indicesStartIndex + 27] = verticesStartIndex + 3;
    indices[indicesStartIndex + 28] = verticesStartIndex + 7;
    indices[indicesStartIndex + 29] = verticesStartIndex + 6;

    // Bottom face
    indices[indicesStartIndex + 30] = verticesStartIndex + 1;
    indices[indicesStartIndex + 31] = verticesStartIndex + 5;
    indices[indicesStartIndex + 32] = verticesStartIndex + 0;
    indices[indicesStartIndex + 33] = verticesStartIndex + 5;
    indices[indicesStartIndex + 34] = verticesStartIndex + 4;
    indices[indicesStartIndex + 35] = verticesStartIndex + 0;
}

void AABBUtils::generateAABBMeshData(const Vector3f& min, const Vector3f& max, f32 lineThickness, AABBMeshData& result) {
    i32 verticesPerCuboid = 8;
    i32 indicesPerCuboid = 36;
    i32 cuboidCount = 12;

    result.verticesCount = verticesPerCuboid * cuboidCount;
    result.indicesCount = indicesPerCuboid * cuboidCount;

    result.vertices = new Vector3f[result.verticesCount];
    result.indices = new ui32[result.indicesCount];

    // Vertical lines
    generateCuboid({min.x, min.y, min.z}, {min.x + lineThickness, max.y, min.z + lineThickness}, result.vertices, 0, result.indices, 0);
    generateCuboid({max.x - lineThickness, min.y, max.z - lineThickness}, {max.x, max.y, max.z}, result.vertices, verticesPerCuboid, result.indices, indicesPerCuboid);
    generateCuboid({max.x - lineThickness, min.y, min.z + lineThickness}, {max.x, max.y, min.z}, result.vertices, 2*verticesPerCuboid, result.indices, 2*indicesPerCuboid);
    generateCuboid({min.x + lineThickness, min.y, max.z - lineThickness}, {min.x, max.y, max.z}, result.vertices, 3*verticesPerCuboid, result.indices, 3*indicesPerCuboid);

    // Horizontal lines on the bottom
    generateCuboid({min.x, min.y, min.z}, {min.x + lineThickness, min.y + lineThickness, max.z}, result.vertices, 4*verticesPerCuboid, result.indices, 4*indicesPerCuboid);
    generateCuboid({max.x - lineThickness, min.y, min.z}, {max.x, min.y + lineThickness, max.z}, result.vertices, 5*verticesPerCuboid, result.indices, 5*indicesPerCuboid);
    generateCuboid({min.x + lineThickness, min.y, min.z}, {max.x - lineThickness, min.y + lineThickness, min.z + lineThickness}, result.vertices, 6*verticesPerCuboid, result.indices, 6*indicesPerCuboid);
    generateCuboid({min.x + lineThickness, min.y, max.z - lineThickness}, {max.x - lineThickness, min.y + lineThickness, max.z}, result.vertices, 7*verticesPerCuboid, result.indices, 7*indicesPerCuboid);

    // Horizontal lines on the top
    generateCuboid({min.x, max.y - lineThickness, min.z}, {min.x + lineThickness, max.y, max.z}, result.vertices, 8*verticesPerCuboid, result.indices, 8*indicesPerCuboid);
    generateCuboid({max.x - lineThickness, max.y - lineThickness, min.z}, {max.x, max.y, max.z}, result.vertices, 9*verticesPerCuboid, result.indices, 9*indicesPerCuboid);
    generateCuboid({min.x + lineThickness, max.y - lineThickness, min.z}, {max.x - lineThickness, max.y, min.z + lineThickness}, result.vertices, 10*verticesPerCuboid, result.indices, 10*indicesPerCuboid);
    generateCuboid({min.x + lineThickness, max.y - lineThickness, max.z - lineThickness}, {max.x - lineThickness, max.y, max.z}, result.vertices, 11*verticesPerCuboid, result.indices, 11*indicesPerCuboid);
}