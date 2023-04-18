#include <iostream>
#include "Atlas.h"
#include "Render.h"
#include "asset/Asset.h"



int main(int argc, char* argv[]) {
    Atlas atlas(false);
    atlas.init();

    Asset::TextureAsset* textureAsset = new Asset::TextureAsset();
    Asset::loadTextureAsset(textureAsset, "resources/textures/blendMap.png");
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