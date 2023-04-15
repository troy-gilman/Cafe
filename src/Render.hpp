#pragma once

#include <GLFW/glfw3.h>

namespace Render {

    struct Window {
        GLFWwindow* glfwWindow;
    };

    void init(Window* window);
    void close(Window* window);
}