#include "Asset.h"

#include "../util/StringUtils.h"
#include "../util/UUIDGenerator.h"
#include <vector>
#include <iostream>

bool Asset::loadMeshAsset(MeshAsset* asset, const char* filePath) {
    // Open the file
    FILE* file = fopen(filePath, "r");
    if (file == nullptr) {
        return false;
    }

    std::vector<Vector3f> vertices;
    std::vector<Vector2f> uvs;
    std::vector<Vector3f> normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

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
            Vector3f vertex = { 0, 0, 0 };
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        } else if (strcmp(line, "vt") == 0) {
            // Read the uv
            Vector2f uv = { 0, 0 };
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uvs.push_back(uv);
        } else if (strcmp(line, "vn") == 0) {
            // Read the normal
            Vector3f normal = { 0, 0, 0 };
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        } else if (strcmp(line, "f") == 0) {
            // Read the face
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                std::cout << "File can't be read by our simple parser. Try exporting with other options\n";
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    // Create the buffers
    float* vertexBuffer = new float[vertexIndices.size() * 3];
    float* uvBuffer = new float[uvIndices.size() * 2];
    float* normalBuffer = new float[normalIndices.size() * 3];

    // For each vertex of each triangle
    for (ui64 i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        Vector3f vertex = vertices[vertexIndex - 1];
        vertexBuffer[i * 3] = vertex.x;
        vertexBuffer[i * 3 + 1] = vertex.y;
        vertexBuffer[i * 3 + 2] = vertex.z;
    }

    // For each uv of each triangle
    for (ui64 i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        Vector2f uv = uvs[uvIndex - 1];
        uvBuffer[i * 2] = uv.x;
        uvBuffer[i * 2 + 1] = uv.y;
    }

    // For each normal of each triangle
    for (ui64 i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        Vector3f normal = normals[normalIndex - 1];
        normalBuffer[i * 3] = normal.x;
        normalBuffer[i * 3 + 1] = normal.y;
        normalBuffer[i * 3 + 2] = normal.z;
    }

    // Create the VAO
    ui32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create the VBO
    ui32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexIndices.size() * 3 * sizeof(float), vertexBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create the TBO
    ui32 tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, uvIndices.size() * 2 * sizeof(float), uvBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create the NBO
    ui32 nbo;
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normalIndices.size() * 3 * sizeof(float), normalBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Set the mesh asset properties
    asset->assetId = UUIDGenerator::getInstance()->generateUUID();
    StringUtils::copyStringToBuffer(filePath, asset->filePath, CHAR_BUFFER_SIZE);
    asset->vao = vao;
    asset->vbo = vbo;
    asset->tbo = tbo;
    asset->nbo = nbo;
    asset->vertexCount = vertexIndices.size();

    return true;
}