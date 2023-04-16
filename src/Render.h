#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Render {

    static const int INIT_WIDTH = 960;
    static const int INIT_HEIGHT = 540;

    struct Window {
        size_t width;
        size_t height;
        GLFWwindow* glfwWindow;
    };

    void initWindow(Window* window);
    void closeWindow(Window * window);
    bool shouldCloseWindow(Window * window);
    void render(Window * window);
}