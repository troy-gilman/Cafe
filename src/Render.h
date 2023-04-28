#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "asset/Asset.h"
#include "Entity.h"
#include <vector>
#include "glm/glm.hpp"

namespace Render {

    static const ui32 INIT_WIDTH = 960;
    static const ui32 INIT_HEIGHT = 540;
    static const f32 FOV = 45.0f;
    static const f32 NEAR_PLANE = 0.1f;
    static const f32 FAR_PLANE = 1000.0f;

    struct Window {
        ui32 width;
        ui32 height;
        f64 posX;
        f64 posY;
        GLFWwindow* glfwWindow;
        glm::f32mat4 projectionMatrix;
        Vector3f backgroundColor;
    };

    void render(Window * window, Asset::AssetPack* assetPack, Entity::EntityState* entityState);

    void initWindow(Window* window);
    void closeWindow(Window * window);
    bool shouldCloseWindow(Window * window);
    void enableCulling();
    void disableCulling();
    void bindShader(Asset::ShaderAsset* shaderAsset);
    void unbindShader();
    void bindMesh(Asset::MeshAsset* meshAsset);
    void unbindMesh();
    void bindTexture(Asset::ShaderAsset* shaderAsset, Asset::TextureAsset* textureAsset, Asset::ShaderUniform uniform, i32 textureUnit);
    void unbindTexture();

    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, i32 value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, f32 value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, bool value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector2f value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f* values, i32 count);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, glm::f32mat4 value);
}