#include <iostream>
#include "Atlas.h"
#include "asset/Asset.h"



int main(int argc, char* argv[]) {
    Atlas atlas;
    atlas.init();

    Asset::TextureAsset* textureAsset = new Asset::TextureAsset();
    Asset::loadTextureAsset(textureAsset, "resources/models/tree/tree.png");
    atlas.addTextureToAssetPack(textureAsset);

    Asset::ShaderAsset* shaderAsset = new Asset::ShaderAsset();
    Asset::loadShaderAsset(shaderAsset, "resources/shaders/mainVertex.glsl", "resources/shaders/mainFragment.glsl");
    atlas.addShaderToAssetPack(shaderAsset);

    Asset::MaterialAsset* materialAsset = new Asset::MaterialAsset();
    Asset::loadMaterialAsset(materialAsset, shaderAsset->assetId, textureAsset->assetId, 0);
    atlas.addMaterialToAssetPack(materialAsset);

    Asset::MeshAsset* meshAsset = new Asset::MeshAsset();
    Asset::loadMeshAsset(meshAsset, "resources/models/tree/tree.obj");
    atlas.addMeshToAssetPack(meshAsset);

    {   // Camera
        UUID entityId = atlas.createEntity();
        atlas.addSpatial3dComponentToEntity(entityId, {0, -2, -5}, {0, 0, 0}, 1);
        atlas.addCameraComponentToEntity(entityId, 0.0f, 0.0f);
    }

    {   // Light
        UUID entityId = atlas.createEntity();
        atlas.addSpatial3dComponentToEntity(entityId, {100.0f, 100.0f, 100.0f}, {0, 0, 0}, 1);
        atlas.addLightComponentToEntity(entityId, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f});
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 100; j++) {
            UUID entityId = atlas.createEntity();
            atlas.addSpatial3dComponentToEntity(entityId, {5.0f * i, 0, 5.0f*j}, {0, 0, 0}, 1);
            atlas.addRenderable3dComponentToEntity(entityId, meshAsset->assetId, materialAsset->assetId, 0);
        }
    }

    atlas.render();
    return 0;
}