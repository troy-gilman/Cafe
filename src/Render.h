#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "asset/Asset.h"
#include "ECS.h"
#include <vector>
#include "glm/glm.hpp"
#include <chrono>

namespace Render {

    // CONSTANTS
    static const i32 INIT_WIDTH = 960;
    static const i32 INIT_HEIGHT = 540;
    static const f32 FOV = 70.0f;
    static const f32 NEAR_PLANE = 0.1f;
    static const f32 FAR_PLANE = 1000.0f;
    static const char* WINDOW_TITLE = "GLFW Render";

    // GLOBALS
    static i32 _windowWidth;
    static i32 _windowHeight;
    static bool _windowResized;

    // CALLBACKS
    void windowResizedCallback(GLFWwindow* window, i32 w, i32 h);

    struct Window {
        i32 width;
        i32 height;
        bool resized;
        i32 posX;
        i32 posY;
        f32 lastFrameTimeMs;
        std::chrono::milliseconds lastWindowUpdateMs;
        std::chrono::milliseconds lastFpsUpdateMs;
        i32 framesSinceLastFpsUpdate;
        GLFWwindow* glfwWindow;
        glm::f32mat4 projectionMatrix;
        Vector3f backgroundColor;
    };

    struct EntityAssetGroupTable {
        i32 numGroups;
        i32 renderOrder[ECS::MAX_ENTITIES];
        UUID meshIds[ECS::MAX_ENTITIES];
        UUID materialIds[ECS::MAX_ENTITIES];
        i32 numEntries[ECS::MAX_ENTITIES];
        UUID table[ECS::MAX_ENTITIES][ECS::MAX_ENTITIES];
    };

    static const i32 MAX_NUM_LIGHTS = 4;

    struct LightData {
        i32 numLights;
        Vector3f lightPositions[MAX_NUM_LIGHTS];
        Vector3f lightColors[MAX_NUM_LIGHTS];
        Vector3f lightAttenuations[MAX_NUM_LIGHTS];
    };

    struct RenderState {
        Window window;
        EntityAssetGroupTable entityAssetGroupTable;
        LightData lightData;
    };

    void prepareRenderState(RenderState& renderState, const ECS::EntityComponentSystem& ecs);
    void render(RenderState& renderState, const Asset::AssetPack& assetPack, const ECS::EntityComponentSystem& ecs);

    void initWindow(Window& window);
    void updateWindow(Window& window);
    void closeWindow(const Window& window);
    bool shouldCloseWindow(const Window& window);
    void enableCulling();
    void disableCulling();
    void bindShader(Asset::ShaderAsset* shaderAsset);
    void unbindShader();
    void bindMesh(Asset::MeshAsset* meshAsset);
    void unbindMesh();
    void bindTexture(Asset::ShaderAsset* shaderAsset, Asset::TextureAsset* textureAsset, Asset::ShaderUniform uniform, i32 textureUnit);
    void unbindTexture();

    // UNIFORM SETTERS
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, i32 value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, f32 value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, bool value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector2f value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f* values, i32 count);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, glm::f32mat4 value);
}