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

void Render::enableCulling() {
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

void Render::disableCulling() {
    glDisable(GL_CULL_FACE);
}

void Render::bindShader(Asset::ShaderAsset* shaderAsset) {
    glUseProgram(shaderAsset->programId);
}

void Render::unbindShader() {
    glUseProgram(0);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, int value) {
    int uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1i(uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, float value) {
    int uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1f(uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, bool value) {
    int uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1i(uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector2f value) {
    int uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform2f(uniformLocation, value.x, value.y);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f value) {
    int uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, std::vector<Vector3f> value, int count) {
    int uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform3fv(uniformLocation, count, (float*)value.data());
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, glm::f32mat4 value) {
    int uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &value[0][0]);
}
