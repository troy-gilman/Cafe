#pragma once

#include <GLFW/glfw3.h>

namespace Render {

    static const int INIT_WIDTH = 960;
    static const int INIT_HEIGHT = 540;

    struct Window {
        int width;
        int height;
        GLFWwindow* glfwWindow;
    };

    void init(Window* window);
    void close(Window* window);
}