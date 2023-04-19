#include "Render.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << message << "\n";
}

void Render::initWindow(Window* window) {
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* glfwWindow = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "GLFW Render", NULL, NULL);
    if (!glfwWindow) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: Unable to initialize GLEW\n";
        return;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    window->width = INIT_WIDTH;
    window->height = INIT_HEIGHT;
    window->glfwWindow = glfwWindow;
    window->backgroundColor = {0.2f, 0.3f, 0.3f};
    window->projectionMatrix = glm::perspective(glm::radians(45.0f), (f32)INIT_WIDTH / (f32)INIT_HEIGHT, 0.1f, 100.0f);
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

void Render::bindMesh(Asset::MeshAsset* meshAsset) {
    glBindVertexArray(meshAsset->vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshAsset->ibo);
}

void Render::unbindMesh() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Render::bindTexture(Asset::ShaderAsset* shaderAsset, Asset::TextureAsset* textureAsset, Asset::ShaderUniform uniform, i32 textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureAsset->textureId);
    setUniform(shaderAsset, uniform, textureUnit);
}

void Render::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, i32 value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1i(uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, f32 value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1f(uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, bool value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1i(uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector2f value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform2f(uniformLocation, value.x, value.y);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, std::vector<Vector3f> value, i32 count) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform3fv(uniformLocation, count, (float*)value.data());
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, glm::f32mat4 value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &value[0][0]);
}
