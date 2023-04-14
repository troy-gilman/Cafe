#include <cstring>
#include <iostream>

#include "Atlas.hpp"

int main(int argc, char* argv[]) {
    bool isServer = argc > 1 && strcmp(argv[1], "server") == 0;
    Atlas atlas(isServer);

    if (isServer) {
        std::cout << "Server started" << std::endl;
        atlas.start();
    } else {
        std::cout << "Client started" << std::endl;
        atlas.start();
    }

    return 0;
}