#include "Render.h"
#include <iostream>

void Render::initWindow(Window* window) {
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

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: Unable to initialize GLEW\n";
        return;
    }
}

void Render::closeWindow(Window* window) {
    glfwDestroyWindow(window->glfwWindow);
    glfwTerminate();
}

bool Render::shouldCloseWindow(Window* window) {
    return glfwWindowShouldClose(window->glfwWindow);
}

void Render::render(Window* window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window->glfwWindow);
    glfwPollEvents();
}