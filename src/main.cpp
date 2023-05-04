#include <iostream>
#include "Atlas.h"
#include "Render.h"
#include "asset/Asset.h"



int main(int argc, char* argv[]) {
    Atlas atlas(false);
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

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10; j++) {
            UUID entityId = atlas.createEntity();
            atlas.addSpatial3dComponentToEntity(entityId, {5.0f * i, 0, 5.0f*j}, {0, 0, 0}, 1);
            atlas.addRenderable3dComponentToEntity(entityId, meshAsset->assetId, materialAsset->assetId, 0);
        }
    }



    atlas.render();




//    bool isServer = argc > 1 && strcmp(argv[1], "server") == 0;
//    Atlas atlas(isServer);
//
//    if (isServer) {
//        std::cout << "Server started" << std::endl;
//        atlas.start();
//    } else {
//        std::cout << "Client started" << std::endl;
//        atlas.start();
//    }
//
//    return 0;
}