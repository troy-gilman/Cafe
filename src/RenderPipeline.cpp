#include "Render.h"
#include "glm/gtc/matrix_transform.hpp"

Vector2f calcTextureAtlasOffset(ui32 atlasSize, ui32 index) {
    ui32 x = index % atlasSize;
    ui32 y = index / atlasSize;
    return {(f32) x / (f32) atlasSize, (f32) y / (f32) atlasSize};
}

void Render::render(Window* window, Asset::AssetPack* assetPack, Entity::EntityState* entityState) {
    glClearColor(window->backgroundColor.x, window->backgroundColor.y, window->backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enableCulling();

    i32 numLights = 1;

    Vector3f lightPositions[] = {
        {100.0f, 100.0f, 100.0f}
    };

    Vector3f lightColors[] = {
        {1.0f, 1.0f, 1.0f}
    };

    Vector3f lightAttenuations[] = {
        {1.0f, 0.0f, 0.0f}
    };

    Entity::Entity* entity1 = entityState->entities.at(1);
    Vector3f entity1Pos = entity1->spatial3D_Position;
    Vector3f entity1Rot = entity1->spatial3D_Rotation;
    glm::f32vec3 cameraPosition = {entity1Pos.x, entity1Pos.y, entity1Pos.z};
    glm::f32vec3 cameraRotation = {entity1Rot.x, entity1Rot.y, entity1Rot.z};
    glm::f32mat4 viewMatrix = glm::translate(glm::f32mat4(1.0f), cameraPosition);

    Asset::ShaderAsset* shaderAsset = assetPack->shaderAssets.begin()->second;
    bindShader(shaderAsset);
    setUniform(shaderAsset, Asset::ShaderUniform::VIEW_MATRIX, viewMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::PROJECTION_MATRIX, window->projectionMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::SKY_COLOR, window->backgroundColor);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_POSITIONS, lightPositions, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COLORS, lightColors, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_ATTENUATIONS, lightAttenuations, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COUNT, numLights);
    {
        Asset::MeshAsset* mesh = assetPack->meshAssets.begin()->second;
        Asset::MaterialAsset* material = assetPack->materialAssets.begin()->second;
        Asset::TextureAsset* texture = assetPack->textureAssets.at(material->textureAssetId);
        //Asset::TextureAsset* normalMap = assetPack->textureAssets.at(material->normalMapAssetId);
        bindMesh(mesh);
        bindTexture(shaderAsset, texture, Asset::ShaderUniform::TEXTURE_SAMPLER, 0);

        if (material->hasTransparency) {
            disableCulling();
        }

        setUniform(shaderAsset, Asset::ShaderUniform::SHINE_DAMPER, material->shineDamper);
        setUniform(shaderAsset, Asset::ShaderUniform::REFLECTIVITY, material->reflectivity);
        setUniform(shaderAsset, Asset::ShaderUniform::USE_FAKE_LIGHTING, material->useFakeLighting);
        setUniform(shaderAsset, Asset::ShaderUniform::TEXTURE_ATLAS_SIZE, (i32) texture->atlasSize);

        glm::f32mat4 modelMatrix = glm::translate(glm::f32mat4(1.0f), {0.0f, 0.0f, 0.0f});
        setUniform(shaderAsset, Asset::ShaderUniform::MODEL_MATRIX, modelMatrix);
        setUniform(shaderAsset, Asset::ShaderUniform::TEXTURE_ATLAS_OFFSET, calcTextureAtlasOffset(texture->atlasSize, 0));

        glDrawElements(GL_TRIANGLES, (i32) mesh->numIndices, GL_UNSIGNED_INT, nullptr);

        unbindTexture();
        unbindMesh();
    }
    unbindShader();

    glfwSwapBuffers(window->glfwWindow);
    glfwPollEvents();
}