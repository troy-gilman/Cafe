#include <cstring>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Atlas.hpp"
#include "Render.hpp"

int main(int argc, char* argv[]) {

    Render::Window* window = new Render::Window();

    Render::init(window);

    while (!glfwWindowShouldClose(window->glfwWindow)) {
        // Render here

        // Swap front and back buffers
        glfwSwapBuffers(window->glfwWindow);

        // Poll for events
        glfwPollEvents();
    }

    Render::close(window);




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