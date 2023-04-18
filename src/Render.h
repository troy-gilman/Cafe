#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "asset/Asset.h"
#include <vector>
#include "glm/glm.hpp"

namespace Render {

    static const int INIT_WIDTH = 960;
    static const int INIT_HEIGHT = 540;

    struct Window {
        size_t width;
        size_t height;
        GLFWwindow* glfwWindow;
    };

    void render(Window * window, Asset::AssetPack* assetPack);

    void initWindow(Window* window);
    void closeWindow(Window * window);
    bool shouldCloseWindow(Window * window);
    void enableCulling();
    void disableCulling();
    void bindShader(Asset::ShaderAsset* shaderAsset);
    void unbindShader();

    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, int value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, float value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, bool value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector2f value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, Vector3f value);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, std::vector<Vector3f> value, int count);
    void setUniform(Asset::ShaderAsset* shaderAsset, Asset::ShaderUniform uniform, glm::f32mat4 value);
}