#include <iostream>
#include "Atlas.h"
#include "Asset.h"
#include "Render.h"


int main(int argc, char* argv[]) {

    Asset::TextureAsset* asset = new Asset::TextureAsset();
    Asset::loadTextureAsset(asset, "resources/textures/blendMap.png");

    Render::Window* window = new Render::Window();

    Render::initWindow(window);

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