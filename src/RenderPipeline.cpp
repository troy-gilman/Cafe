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

    // CAMERA
    ECS::Entity* camera = ecs->entities.at(1);
    ECS::Component* cameraSpatial3d = MapUtils::getValueOrNullPtr(camera->components, ECS::COMPONENT_TYPE_SPATIAL_3D);
    Vector3f cameraPos = cameraSpatial3d->fields[ECS::Spatial3d::FIELD_INDEX_POSITION].field_Vector3f;
    Vector3f cameraRot = cameraSpatial3d->fields[ECS::Spatial3d::FIELD_INDEX_ROTATION].field_Vector3f;
    glm::f32vec3 cameraPosition = {cameraPos.x, cameraPos.y, cameraPos.z};
    glm::f32vec3 cameraRotation = {cameraRot.x, cameraRot.y, cameraRot.z};

    // VIEW MATRIX
    glm::f32vec3 negativePos = { cameraPosition.x, cameraPosition.y, cameraPosition.z };
    glm::f32mat4 viewMatrix(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotation.x), glm::f32vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotation.y), glm::f32vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotation.z), glm::f32vec3(0.0f, 0.0f, 1.0f));
    viewMatrix = glm::translate(viewMatrix,  negativePos);

    // TODO: Make all IDs integer indexed to avoid using maps. This will speed up performance.

    // ENTITIES BY MESH
    size_t numMeshes = assetPack->meshAssets.size();
    size_t numEntities = ecs->entities.size();
    UUID** entitiesByMesh = new UUID*[numMeshes];
    size_t* numEntitiesByMesh = new size_t[numEntities];
    for (auto& pair : ecs->entities) {
        ECS::Entity* entity = pair.second;
        ECS::Component* spatial3d = MapUtils::getValueOrNullPtr(entity->components, ECS::COMPONENT_TYPE_SPATIAL_3D);
        ECS::Component* renderable3d = MapUtils::getValueOrNullPtr(entity->components, ECS::COMPONENT_TYPE_RENDERABLE_3D);
        if (spatial3d == nullptr || renderable3d == nullptr) continue;
        UUID meshId = renderable3d->fields[ECS::Renderable3d::FIELD_INDEX_MESH_ASSET_ID].field_UUID;
        if (entitiesByMesh[meshId] == nullptr) {
            entitiesByMesh[meshId] = new UUID[numEntities];
            numEntitiesByMesh[meshId] = 0;
        }
        entitiesByMesh[meshId][numEntitiesByMesh[meshId]] = entity->id;
        numEntitiesByMesh[meshId]++;
    }

    Asset::ShaderAsset* shaderAsset = assetPack->shaderAssets.begin()->second;
    bindShader(shaderAsset);
    setUniform(shaderAsset, Asset::ShaderUniform::VIEW_MATRIX, viewMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::PROJECTION_MATRIX, window->projectionMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::SKY_COLOR, window->backgroundColor);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_POSITIONS, lightPositions, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COLORS, lightColors, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_ATTENUATIONS, lightAttenuations, numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COUNT, numLights);

    for (i64 meshId = 0; meshId < numMeshes; meshId++) {
        size_t numEntitiesWithMesh = numEntitiesByMesh[meshId];
        if (numEntitiesWithMesh == 0) continue;
        Asset::MeshAsset* mesh = assetPack->meshAssets.at(meshId);
        bindMesh(mesh);

        for (size_t entityIt = 0; entityIt < numEntitiesWithMesh; entityIt++) {
            UUID entityId = entitiesByMesh[meshId][entityIt];
            ECS::Entity* entity = ecs->entities.at(entityId);
            ECS::Component* spatial3d = MapUtils::getValueOrNullPtr(entity->components, ECS::COMPONENT_TYPE_SPATIAL_3D);
            ECS::Component* renderable3d = MapUtils::getValueOrNullPtr(entity->components, ECS::COMPONENT_TYPE_RENDERABLE_3D);
            UUID materialId = renderable3d->fields[ECS::Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID].field_UUID;
            Vector3f position = spatial3d->fields[ECS::Spatial3d::FIELD_INDEX_POSITION].field_Vector3f;
            Vector3f rotation = spatial3d->fields[ECS::Spatial3d::FIELD_INDEX_ROTATION].field_Vector3f;
            f32 scale = spatial3d->fields[ECS::Spatial3d::FIELD_INDEX_SCALE].field_Float;

            Asset::MaterialAsset* material = assetPack->materialAssets.at(materialId);
            Asset::TextureAsset* texture = assetPack->textureAssets.at(material->textureAssetId);
            //Asset::TextureAsset* normalMap = assetPack->textureAssets.at(material->normalMapAssetId);

            bindTexture(shaderAsset, texture, Asset::ShaderUniform::TEXTURE_SAMPLER, 0);

            if (material->hasTransparency) {
                disableCulling();
            }

            setUniform(shaderAsset, Asset::ShaderUniform::SHINE_DAMPER, material->shineDamper);
            setUniform(shaderAsset, Asset::ShaderUniform::REFLECTIVITY, material->reflectivity);
            setUniform(shaderAsset, Asset::ShaderUniform::USE_FAKE_LIGHTING, material->useFakeLighting);
            setUniform(shaderAsset, Asset::ShaderUniform::TEXTURE_ATLAS_SIZE, (i32) texture->atlasSize);

            glm::f32mat4 modelMatrix = glm::translate(glm::f32mat4(1.0f), {position.x, position.y, position.z});
            setUniform(shaderAsset, Asset::ShaderUniform::MODEL_MATRIX, modelMatrix);
            setUniform(shaderAsset, Asset::ShaderUniform::TEXTURE_ATLAS_OFFSET, calcTextureAtlasOffset(texture->atlasSize, 0));

            glDrawElements(GL_TRIANGLES, (i32) mesh->numIndices, GL_UNSIGNED_INT, nullptr);
        }

        unbindTexture();
        unbindMesh();
    }
    unbindShader();

    for (i64 meshId = 0; meshId < numMeshes; meshId++) {
        delete[] entitiesByMesh[meshId];
    }
    delete[] entitiesByMesh;
    delete[] numEntitiesByMesh;

    glfwSwapBuffers(window->glfwWindow);
    glfwPollEvents();
}