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
    static const f32 FAR_PLANE = 300.0f;
    static const char* WINDOW_TITLE = "Cafe";

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
        i32 maxGroups;
        i32 maxEntities;
        i32* renderOrderArray;
        UUID* meshIdArray;
        UUID* materialIdArray;
        i32* numEntitiesArray;
        UUID* groupTable;
    };

    static const i32 MAX_NUM_LIGHTS = 4;

    struct LightData {
        bool needsUpdate;
        i32 numLights;
        i32 maxLights;
        Vector3f* lightPositions;
        Vector3f* lightColors;
        Vector3f* lightAttenuations;
    };

    struct RenderState {
        Window window;
        EntityAssetGroupTable entityAssetGroupTable;
        LightData lightData;
    };

    void initRenderState(RenderState& renderState);
    void initEntityAssetGroupTable(EntityAssetGroupTable& entityAssetGroupTable);
    void initLightData(LightData& lightData);
    void prepareRenderState(RenderState& renderState, const ECS::EntityComponentSystem& ecs, const Frustum& cameraFrustum);
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
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Matrix4f value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, glm::f32mat4 value);
}