#include "Render.h"
#include "glm/gtc/matrix_transform.hpp"
#include "util/MapUtils.h"

Vector2f calcTextureAtlasOffset(ui32 atlasSize, ui32 index) {
    if (index == 0) {
        return { 0.0f, 0.0f };
    }
    ui32 x = index % atlasSize;
    ui32 y = index / atlasSize;
    return {(f32) x / (f32) atlasSize, (f32) y / (f32) atlasSize};
}

void Render::render(Window* window, Asset::AssetPack* assetPack, ECS::EntityComponentSystem* ecs) {
    glClearColor(window->backgroundColor.x, window->backgroundColor.y, window->backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enableCulling();

    // LIGHTS
    i32 numLights = 0;
    const i32 numMaxLights = 4;
    Vector3f lightPositions[numMaxLights];
    Vector3f lightColors[numMaxLights];
    Vector3f lightAttenuations[numMaxLights];
    for (i32 entityId = 0; entityId < ecs->numEntities; entityId++) {
        ECS::Entity* entity = ecs->entities[entityId];
        if (entity == nullptr) continue;
        ECS::Component* spatial3d = entity->components[ECS::COMPONENT_TYPE_SPATIAL_3D];
        ECS::Component* light = entity->components[ECS::COMPONENT_TYPE_LIGHT];
        if (spatial3d == nullptr || light == nullptr) continue;
        ECS::ComponentInfo* spatial3dInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];
        ECS::ComponentInfo* lightInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_LIGHT];
        Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
        Vector3f color = ECS::getField_Vector3f(light, lightInfo, ECS::Light::FIELD_INDEX_COLOR);
        Vector3f attenuation = ECS::getField_Vector3f(light, lightInfo, ECS::Light::FIELD_INDEX_ATTENUATION);
        lightPositions[numLights] = position;
        lightColors[numLights] = color;
        lightAttenuations[numLights] = attenuation;
        numLights++;
        if (numLights == numMaxLights) break;
    }

    // CAMERA
    ECS::Entity* camera = ecs->entities[0];
    ECS::Component* cameraSpatial3d = camera->components[ECS::COMPONENT_TYPE_SPATIAL_3D];
    ECS::ComponentInfo* cameraSpatial3dInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];
    Vector3f cameraPos = ECS::getField_Vector3f(cameraSpatial3d, cameraSpatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
    Vector3f cameraRot = ECS::getField_Vector3f(cameraSpatial3d, cameraSpatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION);
    glm::f32vec3 cameraPosition = {cameraPos.x, cameraPos.y, cameraPos.z};
    glm::f32vec3 cameraRotation = {cameraRot.x, cameraRot.y, cameraRot.z};

    // VIEW MATRIX
    glm::f32vec3 negativePos = { cameraPosition.x, cameraPosition.y, cameraPosition.z };
    glm::f32mat4 viewMatrix(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotation.x), glm::f32vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotation.y), glm::f32vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotation.z), glm::f32vec3(0.0f, 0.0f, 1.0f));
    viewMatrix = glm::translate(viewMatrix,  negativePos);

    ECS::buildEntityAssetTable(ecs);

    Asset::ShaderAsset* shaderAsset = assetPack->shaderAssets[0];
    bindShader(shaderAsset);
    setUniform(shaderAsset, Asset::ShaderUniform::VIEW_MATRIX, viewMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::PROJECTION_MATRIX, window->projectionMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::SKY_COLOR, window->backgroundColor);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_POSITIONS, lightPositions, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COLORS, lightColors, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_ATTENUATIONS, lightAttenuations, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COUNT, numLights);

    UUID prevMeshId = -1;
    UUID prevMaterialId = -1;
    i32 numAssetGroups = ecs->entityAssetGroupTable.numGroups;
    for (i32 i = 0; i < numAssetGroups; i++) {
        i32 groupIndex = ecs->entityAssetGroupTable.renderOrder[i];
        UUID meshId = ecs->entityAssetGroupTable.meshIds[groupIndex];
        UUID materialId = ecs->entityAssetGroupTable.materialIds[groupIndex];
        if (meshId == -1 || materialId == -1) continue;
        Asset::MeshAsset* mesh = assetPack->meshAssets[meshId];
        Asset::MaterialAsset* material = assetPack->materialAssets[materialId];
        Asset::TextureAsset* texture = assetPack->textureAssets[material->textureAssetId];
        //Asset::TextureAsset* normalMap = assetPack->textureAssets.at(material->normalMapAssetId);

        if (meshId != prevMeshId) {
            bindMesh(mesh);
            prevMeshId = meshId;
        }
        if (materialId != prevMaterialId) {
            if (material->hasTransparency) {
                disableCulling();
            }
            bindTexture(shaderAsset, texture, Asset::ShaderUniform::TEXTURE_SAMPLER, 0);
            setUniform(shaderAsset, Asset::ShaderUniform::SHINE_DAMPER, material->shineDamper);
            setUniform(shaderAsset, Asset::ShaderUniform::REFLECTIVITY, material->reflectivity);
            setUniform(shaderAsset, Asset::ShaderUniform::USE_FAKE_LIGHTING, material->useFakeLighting);
            setUniform(shaderAsset, Asset::ShaderUniform::TEXTURE_ATLAS_SIZE, (i32) texture->atlasSize);
            prevMaterialId = materialId;
        }

        i32 numEntitiesInGroup = ecs->entityAssetGroupTable.numEntries[groupIndex];
        for (size_t entityIt = 0; entityIt < numEntitiesInGroup; entityIt++) {
            UUID entityId = ecs->entityAssetGroupTable.table[groupIndex][entityIt];
            ECS::Entity* entity = ecs->entities[entityId];
            ECS::Component* spatial3d = entity->components[ECS::COMPONENT_TYPE_SPATIAL_3D];
            ECS::ComponentInfo* spatial3dInfo = ecs->componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];
            Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
            Vector3f rotation = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION);
            f32 scale = ECS::getField_f32(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_SCALE);

            glm::f32mat4 modelMatrix = glm::translate(glm::f32mat4(1.0f), {position.x, position.y, position.z});
            setUniform(shaderAsset, Asset::ShaderUniform::MODEL_MATRIX, modelMatrix);
            setUniform(shaderAsset, Asset::ShaderUniform::TEXTURE_ATLAS_OFFSET, calcTextureAtlasOffset(texture->atlasSize, 0));

            glDrawElements(GL_TRIANGLES, (i32) mesh->numIndices, GL_UNSIGNED_INT, nullptr);
        }
    }
    unbindTexture();
    unbindMesh();
    unbindShader();

    glfwSwapBuffers(window->glfwWindow);
    glfwPollEvents();
}