#include "Asset.h"

#include "../util/StringUtils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <cfloat>

void processMesh(aiMesh* mesh, const aiScene* scene, Asset::MeshAsset* asset) {
    for (ui32 i = 0; i < mesh->mNumVertices; i++) {

    }
}

void processNode(aiNode* node, const aiScene* scene, Asset::MeshAsset* asset) {
    // Process all the node's meshes (if any)
    for (ui32 i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, asset);
    }
    // Then do the same for each of its children
    for (ui32 i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, asset);
    }
}

UUID Asset::loadMeshAsset(AssetPack& assetPack, const char* filePath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
        return false;
    }

    // If there are multiple meshes in the file, we only load the first one
    //processNode(scene->mRootNode, scene, asset);
    aiMesh* mesh = scene->mMeshes[0];

    ui32 verticesBufferSize = mesh->mNumVertices * 3;
    ui32 texCoordsBufferSize = mesh->mNumVertices * 2;
    ui32 normalsBufferSize = mesh->mNumVertices * 3;
    ui32 indicesBufferSize = mesh->mNumFaces * 3;

    f32* vertices = new f32[verticesBufferSize];
    f32* texCoords = new f32[texCoordsBufferSize];
    f32* normals = new f32[normalsBufferSize];
    ui32* indices = new ui32[indicesBufferSize];

    AABB::AABB aabb = {
            Vector3f{FLT_MAX, FLT_MAX, FLT_MAX},
            Vector3f{FLT_MIN, FLT_MIN, FLT_MIN}
    };

    for (ui32 i = 0; i < mesh->mNumVertices; i++) {
        // Update minAABB
        if (mesh->mVertices[i].x < aabb.min.x) {
            aabb.min.x = mesh->mVertices[i].x;
        }
        if (mesh->mVertices[i].y < aabb.min.y) {
            aabb.min.y = mesh->mVertices[i].y;
        }
        if (mesh->mVertices[i].z < aabb.min.z) {
            aabb.min.z = mesh->mVertices[i].z;
        }

        // Update maxAABB
        if (mesh->mVertices[i].x > aabb.max.x) {
            aabb.max.x = mesh->mVertices[i].x;
        }
        if (mesh->mVertices[i].y > aabb.max.y) {
            aabb.max.y = mesh->mVertices[i].y;
        }
        if (mesh->mVertices[i].z > aabb.max.z) {
            aabb.max.z = mesh->mVertices[i].z;
        }

        // Copy the vertex data
        vertices[i * 3] = mesh->mVertices[i].x;
        vertices[i * 3 + 1] = mesh->mVertices[i].y;
        vertices[i * 3 + 2] = mesh->mVertices[i].z;

        // Copy the texture coordinate data
        texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
        texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;

        // Copy the normal data
        normals[i * 3] = mesh->mNormals[i].x;
        normals[i * 3 + 1] = mesh->mNormals[i].y;
        normals[i * 3 + 2] = mesh->mNormals[i].z;
    }

    for (ui32 i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (ui32 j = 0; j < 3; j++) {
            indices[i * 3 + j] = face.mIndices[j];
        }
    }

    // Create the VAO
    ui32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create the VBO
    ui32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (verticesBufferSize * sizeof(f32)), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create the TBO
    ui32 tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (texCoordsBufferSize * sizeof(f32)), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Create the NBO
    ui32 nbo;
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (normalsBufferSize * sizeof(f32)), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ui32 ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (i64) (indicesBufferSize * sizeof(ui32)), indices, GL_STATIC_DRAW);

    AABB::loadAABBMesh(aabb.min, aabb.max, aabb.mesh);

    // Create the mesh asset
    MeshAsset* asset = new MeshAsset();
    StringUtils::copyStringToBuffer(asset->filePath, filePath, CHAR_BUFFER_SIZE);
    asset->vao = vao;
    asset->vbo = vbo;
    asset->tbo = tbo;
    asset->nbo = nbo;
    asset->ibo = ibo;
    asset->numIndices = indicesBufferSize;
    asset->aabb = aabb;

    // Add the mesh asset to the asset pack
    UUID assetId = assetPack.nextMeshAssetId;
    asset->assetId = assetId;
    assetPack.meshAssets[assetId] = asset;
    assetPack.numMeshAssets++;
    assetPack.nextMeshAssetId++;
    return assetId;
}