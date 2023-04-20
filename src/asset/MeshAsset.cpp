#include "Asset.h"

#include "../util/StringUtils.h"
#include "../util/UUIDGenerator.h"
#include <vector>
#include <iostream>

struct Vertex {
    Vector3f position;
    i64 uvIndex;
    i64 normalIndex;
    Vertex* duplicateVertex = nullptr;
    ui64 index;

    Vertex(Vector3f position, ui64 index) {
        this->position = position;
        this->uvIndex = -1;
        this->normalIndex = -1;
        this->duplicateVertex = nullptr;
        this->index = index;
    }
};

void dealWithAlreadyProcessedVertex(Vertex* previousVertex, i64 newUvIndex, i64 newNormalIndex, std::vector<Vertex*>& vertices, std::vector<i32>& indices) {
    if (previousVertex->uvIndex == newUvIndex && previousVertex->normalIndex == newNormalIndex) {
        indices.push_back((i32) previousVertex->index);
    } else if (previousVertex->duplicateVertex != nullptr) {
        dealWithAlreadyProcessedVertex(previousVertex->duplicateVertex, newUvIndex, newNormalIndex, vertices, indices);
    } else {
        Vertex* duplicateVertex = new Vertex(previousVertex->position, vertices.size());
        duplicateVertex->uvIndex = newUvIndex;
        duplicateVertex->normalIndex = newNormalIndex;
        previousVertex->duplicateVertex = duplicateVertex;
        vertices.push_back(duplicateVertex);
        indices.push_back((i32) duplicateVertex->index);
    }
}

void processVertex(i32 vertexIndex, i32 uvIndex, i32 normalIndex, std::vector<Vertex*>& vertices, std::vector<i32>& indices) {
    Vertex* currentVertex = vertices[vertexIndex - 1];
    if (currentVertex->uvIndex == -1 || currentVertex->normalIndex == -1) {
        currentVertex->uvIndex = uvIndex - 1;
        currentVertex->normalIndex = normalIndex - 1;
        indices.push_back(vertexIndex - 1);
    } else {
        dealWithAlreadyProcessedVertex(currentVertex, uvIndex, normalIndex, vertices, indices);
    }
}

bool Asset::loadMeshAsset(MeshAsset* asset, const char* filePath) {
    // Open the file
    FILE* file = fopen(filePath, "r");
    if (file == nullptr) {
        return false;
    }

    std::vector<Vertex*> vertices;
    std::vector<Vector2f> uvs;
    std::vector<Vector3f> normals;
    std::vector<i32> indices;

    bool done = false;
    while (!done) {
        // Read the first word of the line
        char line[128];
        int result = fscanf(file, "%s", line);

        if (result == EOF) {
            // EOF = End Of File. Quit the loop.
            done = true;
            continue;
        } else if (strcmp(line, "v") == 0) {
            // Read the vertex
            Vector3f position = { 0, 0, 0 };
            int matches = fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
            if (matches != 3) {
                std::cout << "File can't be read by our simple parser. Try exporting with other options\n";
                return false;
            }
            Vertex* vertex = new Vertex(position, vertices.size());
            vertices.push_back(vertex);
        } else if (strcmp(line, "vt") == 0) {
            // Read the uv
            Vector2f uv = { 0, 0 };
            int matches = fscanf(file, "%f %f\n", &uv.x, &uv.y);
            if (matches != 2) {
                std::cout << "File can't be read by our simple parser. Try exporting with other options\n";
                return false;
            }
            uvs.push_back(uv);
        } else if (strcmp(line, "vn") == 0) {
            // Read the normal
            Vector3f normal = { 0, 0, 0 };
            int matches = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            if (matches != 3) {
                std::cout << "File can't be read by our simple parser. Try exporting with other options\n";
                return false;
            }
            normals.push_back(normal);
        } else if (strcmp(line, "f") == 0) {
            // Read the face
            i32 vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                std::cout << "File can't be read by our simple parser. Try exporting with other options\n";
                return false;
            }
            processVertex(vertexIndex[0], uvIndex[0], normalIndex[0], vertices, indices);
            processVertex(vertexIndex[1], uvIndex[1], normalIndex[1], vertices, indices);
            processVertex(vertexIndex[2], uvIndex[2], normalIndex[2], vertices, indices);
        }
    }

    // Remove the unused vertices
    for (Vertex* vertex : vertices) {
        if (vertex->uvIndex == -1 || vertex->normalIndex == -1) {
            vertex->uvIndex = 0;
            vertex->normalIndex = 0;
        }
    }

    // Create the buffers
    f32* vertexBuffer = new f32[vertices.size() * 3];
    f32* uvBuffer = new f32[vertices.size() * 2];
    f32* normalBuffer = new f32[vertices.size() * 3];
    i32* indexBuffer = new i32[indices.size()];

    for (i32 i = 0; i < vertices.size(); i++) {
        Vertex* currentVertex = vertices[i];
        Vector3f position = currentVertex->position;
        Vector2f uv = uvs[currentVertex->uvIndex];
        Vector3f normal = normals[currentVertex->normalIndex];
        vertexBuffer[i * 3] = position.x;
        vertexBuffer[i * 3 + 1] = position.y;
        vertexBuffer[i * 3 + 2] = position.z;
        uvBuffer[i * 2] = uv.x;
        uvBuffer[i * 2 + 1] = 1 - uv.y;
        normalBuffer[i * 3] = normal.x;
        normalBuffer[i * 3 + 1] = normal.y;
        normalBuffer[i * 3 + 2] = normal.z;
        delete currentVertex;
    }
    for (i32 i = 0; i < indices.size(); i++) {
        indexBuffer[i] = indices[i];
    }

    // Create the VAO
    ui32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create the VBO
    ui32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (vertices.size() * 3 * sizeof(f32)), vertexBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create the TBO
    ui32 tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (vertices.size() * 2 * sizeof(f32)), uvBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create the NBO
    ui32 nbo;
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (vertices.size() * 3 * sizeof(f32)), normalBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ui32 ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (i64) (indices.size() * sizeof(i32)), indexBuffer, GL_STATIC_DRAW);


    // Set the mesh asset properties
    asset->assetId = UUIDGenerator::getInstance()->generateUUID();
    StringUtils::copyStringToBuffer(filePath, asset->filePath, CHAR_BUFFER_SIZE);
    asset->vao = vao;
    asset->vbo = vbo;
    asset->tbo = tbo;
    asset->nbo = nbo;
    asset->ibo = ibo;
    asset->numIndices = indices.size();

    return true;
}