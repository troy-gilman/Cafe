#include <iostream>
#include "Atlas.h"
#include "Render.h"
#include "asset/Asset.h"



int main(int argc, char* argv[]) {
    Render::Window* window = new Render::Window();
    Render::initWindow(window);

    Asset::TextureAsset* textureAsset = new Asset::TextureAsset();
    Asset::loadTextureAsset(textureAsset, "resources/textures/blendMap.png");

    Asset::ShaderAsset* shaderAsset = new Asset::ShaderAsset();
    Asset::loadShaderAsset(shaderAsset, "resources/shaders/mainVertex.glsl", "resources/shaders/mainFragment.glsl");

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