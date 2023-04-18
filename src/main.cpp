#include <iostream>
#include "Atlas.h"
#include "Render.h"
#include "asset/Asset.h"



int main(int argc, char* argv[]) {
    Render::Window* window = new Render::Window();
    Render::initWindow(window);

    Asset::AssetPack* assetPack = new Asset::AssetPack();

    Asset::TextureAsset* textureAsset = new Asset::TextureAsset();
    Asset::loadTextureAsset(textureAsset, "resources/textures/blendMap.png");
    Asset::addTextureToAssetPack(assetPack, textureAsset);

    Asset::ShaderAsset* shaderAsset = new Asset::ShaderAsset();
    Asset::loadShaderAsset(shaderAsset, "resources/shaders/mainVertex.glsl", "resources/shaders/mainFragment.glsl");
    Asset::addShaderToAssetPack(assetPack, shaderAsset);

    Asset::MaterialAsset* materialAsset = new Asset::MaterialAsset();
    Asset::loadMaterialAsset(materialAsset, shaderAsset->assetId, textureAsset->assetId, 0);
    Asset::addMaterialToAssetPack(assetPack, materialAsset);

    Asset::MeshAsset* meshAsset = new Asset::MeshAsset();
    Asset::loadMeshAsset(meshAsset, "resources/models/tree/tree.obj");
    Asset::addMeshToAssetPack(assetPack, meshAsset);

    while (!Render::shouldCloseWindow(window)) {
       Render::render(window);
    }

    Render::closeWindow(window);




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