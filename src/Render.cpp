#include "Render.hpp"


void Render::init(Window* window) {
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->glfwWindow = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "GLFW Render", NULL, NULL);
    if (!window->glfwWindow) {
        glfwTerminate();
        return;
    }
    window->width = INIT_WIDTH;
    window->height = INIT_HEIGHT;

    glfwMakeContextCurrent(window->glfwWindow);
    glfwSwapInterval(1);
}

void Render::close(Window* window) {
    glfwDestroyWindow(window->glfwWindow);
    glfwTerminate();
}
