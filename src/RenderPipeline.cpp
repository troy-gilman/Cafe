#include "Render.h"
#include "glm/gtc/matrix_transform.hpp"
#include "util/ArrayUtils.h"
#include <cstring>

Vector2f calcTextureAtlasOffset(ui32 atlasSize, ui32 index) {
    if (index == 0) {
        return { 0.0f, 0.0f };
    }
    ui32 x = index % atlasSize;
    ui32 y = index / atlasSize;
    return {(f32) x / (f32) atlasSize, (f32) y / (f32) atlasSize};
}

void Render::prepareRenderState(RenderState& renderState, const ECS::EntityComponentSystem& ecs) {
    LightData& lightData = renderState.lightData;
    EntityAssetGroupTable& entityAssetGroupTable = renderState.entityAssetGroupTable;

    // Reset number of lights
    lightData.numLights = 0;

    if (entityAssetGroupTable.needsUpdate) {
        // Reset EntityAssetGroupTable
        entityAssetGroupTable.numGroups = 0;
        memset(entityAssetGroupTable.numEntitiesArray, 0, sizeof(i32) * entityAssetGroupTable.maxGroups);
        if (entityAssetGroupTable.maxEntities != ecs.maxEntities) {
            entityAssetGroupTable.maxEntities = ecs.maxEntities;
            delete entityAssetGroupTable.groupTable;
            entityAssetGroupTable.groupTable = new UUID[entityAssetGroupTable.maxGroups * ecs.maxEntities];
        }
    }

    // Iterate through all entities
    for (i32 entityId = 0; entityId < ecs.maxEntities; entityId++) {
        if (!ecs.entityExistsArray[entityId]) continue;

        bool hasRenderable3d  = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_RENDERABLE_3D);
        bool hasSpatial3d     = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
        bool hasCamera        = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_CAMERA);
        bool hasLight         = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_LIGHT);

        // Add this entity to the light data
        if (hasLight && hasSpatial3d && lightData.numLights < MAX_NUM_LIGHTS) {
            const ECS::Component& spatial3d = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
            const ECS::Component& light = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_LIGHT);
            const ECS::ComponentInfo& spatial3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_SPATIAL_3D];
            const ECS::ComponentInfo& lightInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_LIGHT];
            Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
            Vector3f color = ECS::getField_Vector3f(light, lightInfo, ECS::Light::FIELD_INDEX_COLOR);
            Vector3f attenuation = ECS::getField_Vector3f(light, lightInfo, ECS::Light::FIELD_INDEX_ATTENUATION);
            lightData.lightPositions[lightData.numLights] = position;
            lightData.lightColors[lightData.numLights] = color;
            lightData.lightAttenuations[lightData.numLights] = attenuation;
            lightData.numLights++;
        }

        if (entityAssetGroupTable.needsUpdate) {
            // Add this entity to the entity asset group table
            if (hasRenderable3d && hasSpatial3d) {
                const ECS::Component &renderable3d = ECS::getComponent(ecs, entityId,
                                                                       ECS::COMPONENT_TYPE_RENDERABLE_3D);
                const ECS::ComponentInfo &renderable3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_RENDERABLE_3D];
                UUID meshId = ECS::getField_i32(renderable3d, renderable3dInfo,
                                                ECS::Renderable3d::FIELD_INDEX_MESH_ASSET_ID);
                UUID materialId = ECS::getField_i32(renderable3d, renderable3dInfo,
                                                    ECS::Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID);
                if (meshId != -1 && materialId != -1) {
                    // Find the asset group for this mesh and material
                    i32 tableIndex = 0;
                    while (tableIndex < entityAssetGroupTable.numGroups) {
                        UUID assetGroupMeshId = entityAssetGroupTable.meshIdArray[tableIndex];
                        UUID assetGroupMaterialId = entityAssetGroupTable.meshIdArray[tableIndex];
                        if (assetGroupMeshId == meshId && assetGroupMaterialId == materialId) break;
                        tableIndex++;
                    }

                    // If no asset group exists for this mesh and material, create one
                    // We have to find the correct place to insert it in the render order
                    i32 numEntries = entityAssetGroupTable.numEntitiesArray[tableIndex];
                    if (numEntries == 0) {
                        i32 numGroups = entityAssetGroupTable.numGroups;
                        i32 orderIndex = 0;
                        while (orderIndex < numGroups) {
                            i32 groupIndex = entityAssetGroupTable.renderOrderArray[orderIndex];
                            UUID assetGroupMeshId = entityAssetGroupTable.meshIdArray[groupIndex];
                            if (assetGroupMeshId == meshId) {
                                ArrayUtils::shiftArrayRight(entityAssetGroupTable.renderOrderArray, numGroups, orderIndex);
                                break;
                            }
                            orderIndex++;
                        }
                        entityAssetGroupTable.renderOrderArray[orderIndex] = tableIndex;
                        entityAssetGroupTable.meshIdArray[tableIndex] = meshId;
                        entityAssetGroupTable.materialIdArray[tableIndex] = materialId;
                        entityAssetGroupTable.numGroups++;
                    }

                    // Add this entity to the asset group
                    entityAssetGroupTable.groupTable[tableIndex * entityAssetGroupTable.maxEntities + numEntries] = entityId;
                    entityAssetGroupTable.numEntitiesArray[tableIndex]++;
                }
            }
        }
    }
    entityAssetGroupTable.needsUpdate = false;
}

void Render::render(RenderState& renderState, const Asset::AssetPack& assetPack, const ECS::EntityComponentSystem& ecs) {
    Vector3f backgroundColor = renderState.window.backgroundColor;
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enableCulling();

    // CAMERA
    UUID cameraEntityId = 0;
    const ECS::Component& cameraSpatial3d = ECS::getComponent(ecs, cameraEntityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
    const ECS::ComponentInfo& cameraSpatial3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_SPATIAL_3D];
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

    prepareRenderState(renderState, ecs);
    LightData& lightData = renderState.lightData;
    EntityAssetGroupTable& entityAssetGroupTable = renderState.entityAssetGroupTable;

    Asset::ShaderAsset* shaderAsset = assetPack.shaderAssets[0];
    bindShader(shaderAsset);
    setUniform(shaderAsset, Asset::ShaderUniform::VIEW_MATRIX, viewMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::PROJECTION_MATRIX, renderState.window.projectionMatrix);
    setUniform(shaderAsset, Asset::ShaderUniform::SKY_COLOR, renderState.window.backgroundColor);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_POSITIONS, lightData.lightPositions, lightData.numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COLORS, lightData.lightColors, lightData.numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_ATTENUATIONS, lightData.lightAttenuations, lightData.numLights);
    setUniform(shaderAsset, Asset::ShaderUniform::LIGHT_COUNT, lightData.numLights);

    UUID prevMeshId = -1;
    UUID prevMaterialId = -1;
    i32 numAssetGroups = entityAssetGroupTable.numGroups;
    for (i32 i = 0; i < numAssetGroups; i++) {
        i32 groupIndex = entityAssetGroupTable.renderOrderArray[i];
        UUID meshId = entityAssetGroupTable.meshIdArray[groupIndex];
        UUID materialId = entityAssetGroupTable.materialIdArray[groupIndex];
        if (meshId == -1 || materialId == -1) continue;
        Asset::MeshAsset* mesh = assetPack.meshAssets[meshId];
        Asset::MaterialAsset* material = assetPack.materialAssets[materialId];
        Asset::TextureAsset* texture = assetPack.textureAssets[material->textureAssetId];
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

        i32 numEntitiesInGroup = entityAssetGroupTable.numEntitiesArray[groupIndex];
        for (size_t entityIt = 0; entityIt < numEntitiesInGroup; entityIt++) {
            UUID entityId = entityAssetGroupTable.groupTable[groupIndex * entityAssetGroupTable.maxEntities + entityIt];
            const ECS::Component& spatial3d = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
            const ECS::ComponentInfo& spatial3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_SPATIAL_3D];
            Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
            // Vector3f rotation = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION);
            // f32 scale = ECS::getField_f32(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_SCALE);

            glm::f32mat4 modelMatrix = glm::translate(glm::f32mat4(1.0f), {position.x, position.y, position.z});
            setUniform(shaderAsset, Asset::ShaderUniform::MODEL_MATRIX, modelMatrix);
            setUniform(shaderAsset, Asset::ShaderUniform::TEXTURE_ATLAS_OFFSET, calcTextureAtlasOffset(texture->atlasSize, 0));

            glDrawElements(GL_TRIANGLES, (i32) mesh->numIndices, GL_UNSIGNED_INT, nullptr);
        }
    }
    unbindTexture();
    unbindMesh();
    unbindShader();

    glfwSwapBuffers(renderState.window.glfwWindow);
    glfwPollEvents();
}