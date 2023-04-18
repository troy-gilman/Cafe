#include "Render.h"

void Render::render(Window* window, Asset::AssetPack* assetPack) {
    Vector3f skyColor = {0.2f, 0.3f, 0.3f};
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    enableCulling();

    int numLights = 0;

    Asset::ShaderAsset* shaderAsset = assetPack->shaderAssets.begin()->second;
    bindShader(shaderAsset);
    setUniform(shaderAsset, Asset::ShaderUniform::VIEW_MATRIX, 0);
    setUniform(shaderAsset, Asset::ShaderUniform::PROJECTION_MATRIX, 0);
    setUniform(shaderAsset, Asset::ShaderUniform::SKY_COLOR, skyColor);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_POSITIONS, std::vector<Vector3f>(), numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COLORS, std::vector<Vector3f>(), numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_ATTENUATIONS, std::vector<Vector3f>(), numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COUNT, numLights);
    unbindShader();

    glfwSwapBuffers(window->glfwWindow);
    glfwPollEvents();
}