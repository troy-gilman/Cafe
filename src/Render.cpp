#include "Render.h"
#include <iostream>
#include "util/MathUtils.h"
#include "util/TimeUtils.h"
#include <cstring>

void Render::initRenderState(RenderState& renderState) {
    initWindow(renderState.window);
    initEntityAssetGroupTable(renderState.entityAssetGroupTable);
    initLightData(renderState.lightData);
}

void Render::initEntityAssetGroupTable(EntityAssetGroupTable& entityAssetGroupTable) {
    // Delete old data
    delete entityAssetGroupTable.renderOrderArray;
    delete entityAssetGroupTable.meshIdArray;
    delete entityAssetGroupTable.materialIdArray;
    delete entityAssetGroupTable.numEntitiesArray;
    delete entityAssetGroupTable.groupTable;

    // Initialize new data
    entityAssetGroupTable.needsUpdate = true;
    entityAssetGroupTable.numGroups = 0;
    entityAssetGroupTable.maxGroups = 64;
    entityAssetGroupTable.maxEntities = 0;
    entityAssetGroupTable.renderOrderArray = new i32[entityAssetGroupTable.maxGroups];
    entityAssetGroupTable.meshIdArray = new UUID[entityAssetGroupTable.maxGroups];
    entityAssetGroupTable.materialIdArray = new UUID[entityAssetGroupTable.maxGroups];
    entityAssetGroupTable.numEntitiesArray = new i32[entityAssetGroupTable.maxGroups];
    entityAssetGroupTable.groupTable = nullptr;
    memset(entityAssetGroupTable.renderOrderArray, 0, sizeof(i32) * entityAssetGroupTable.maxGroups);
    memset(entityAssetGroupTable.meshIdArray, 0, sizeof(UUID) * entityAssetGroupTable.maxGroups);
    memset(entityAssetGroupTable.materialIdArray, 0, sizeof(UUID) * entityAssetGroupTable.maxGroups);
    memset(entityAssetGroupTable.numEntitiesArray, 0, sizeof(i32) * entityAssetGroupTable.maxGroups);
}

void Render::initLightData(Render::LightData &lightData) {
    // Delete old data
    delete lightData.lightPositions;
    delete lightData.lightColors;
    delete lightData.lightAttenuations;

    // Initialize new data
    lightData.needsUpdate = true;
    lightData.numLights = 0;
    lightData.maxLights = MAX_NUM_LIGHTS;
    lightData.lightPositions = new Vector3f[lightData.maxLights];
    lightData.lightColors = new Vector3f[lightData.maxLights];
    lightData.lightAttenuations = new Vector3f[lightData.maxLights];
    memset(lightData.lightPositions, 0, sizeof(Vector3f) * lightData.maxLights);
    memset(lightData.lightColors, 0, sizeof(Vector3f) * lightData.maxLights);
    memset(lightData.lightAttenuations, 0, sizeof(Vector3f) * lightData.maxLights);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << message << "\n";
}

void Render::windowResizedCallback(GLFWwindow* window, i32 w, i32 h) {
//    _windowWidth = w*2; // macOS Retina
//    _windowHeight = h*2; // macOS Retina
    _windowWidth = w;
    _windowHeight = h;
    _windowResized = true;
}

void Render::initWindow(Window& window) {
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* glfwWindow = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!glfwWindow) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(glfwWindow);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0); // Unlimited FPS
    //glfwSwapInterval(1); // VSync

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: Unable to initialize GLEW\n";
        return;
    }

    glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, nullptr); // macOS doesn't support this
    glfwSetWindowSizeCallback(glfwWindow, windowResizedCallback);

    window.width = INIT_WIDTH;
    window.height = INIT_HEIGHT;
    window.resized = false;
    window.lastFpsUpdateMs = TimeUtils::getCurrentTimeMillis();
    window.framesSinceLastFpsUpdate = 0;
    window.glfwWindow = glfwWindow;
    window.backgroundColor = {0.2f, 0.3f, 0.3f};
    window.projectionMatrix = MathUtils::createProjectionMatrix(FOV,  NEAR_PLANE, FAR_PLANE, (f32)window.width / (f32)window.height);
}

void Render::updateWindow(Window& window) {
    window.width = _windowWidth;
    window.height = _windowHeight;
    window.resized = _windowResized;
    if (window.resized) {
        glViewport(0, 0, window.width, window.height);
        window.projectionMatrix = MathUtils::createProjectionMatrix(FOV, NEAR_PLANE, FAR_PLANE, (f32)window.width / (f32)window.height);
        _windowResized = false;
    }

    std::chrono::milliseconds currentTime = TimeUtils::getCurrentTimeMillis();

    // Update FPS counter
    window.framesSinceLastFpsUpdate++;
    if (currentTime >= window.lastFpsUpdateMs + std::chrono::milliseconds(1000)) {
        std::string title = std::string(WINDOW_TITLE) + " |FPS: " + std::to_string(window.framesSinceLastFpsUpdate) + "|";
        glfwSetWindowTitle(window.glfwWindow, title.c_str());
        window.lastFpsUpdateMs = currentTime;
        window.framesSinceLastFpsUpdate = 0;
    }

    // Update last frame time
    window.lastFrameTimeMs = (f32) (currentTime - window.lastWindowUpdateMs).count();
    window.lastWindowUpdateMs = currentTime;
}

void Render::closeWindow(const Window& window) {
    glfwDestroyWindow(window.glfwWindow);
    glfwTerminate();
}

bool Render::shouldCloseWindow(const Window& window) {
    return glfwWindowShouldClose(window.glfwWindow);
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
    setUniform(shaderAsset, uniform, textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureAsset->textureId);
}

void Render::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, i32 value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1i((i32) uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, f32 value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1f((i32) uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, bool value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform1i((i32) uniformLocation, value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector2f value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform2f((i32) uniformLocation, value.x, value.y);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform3f((i32) uniformLocation, value.x, value.y, value.z);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f* values, i32 count) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniform3fv((i32) uniformLocation, count, (f32*) values);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Matrix4f value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniformMatrix4fv((i32) uniformLocation, 1, GL_FALSE, (f32*) &value);
}

void Render::setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, glm::f32mat4 value) {
    ui32 uniformLocation = shaderAsset->uniformLocations[uniform];
    glUniformMatrix4fv((i32) uniformLocation, 1, GL_FALSE, &value[0][0]);
}
