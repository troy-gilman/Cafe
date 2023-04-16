#include <cstring>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Atlas.h"
#include "Render.h"
#include "util/Image.h"
#include "util/StringUtils.h"

int main(int argc, char* argv[]) {

    Image::Image* image = new Image::Image();
    const char* fileName = "resources/textures/blendMap.png";
    StringUtils::copyStringToBuffer(fileName, image->filePath, strlen(fileName));
    bool success = Image::loadImage(image);


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