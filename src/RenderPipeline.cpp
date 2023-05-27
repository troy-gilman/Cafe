#include "Render.h"
#include "glm/gtc/matrix_transform.hpp"
#include "util/ArrayUtils.h"
#include <cstring>
#include <iostream>
#include "util/MathUtils.h"
#include "AxisAlignedBoundingBox.h"

Vector2f calcTextureAtlasOffset(ui32 atlasSize, ui32 index) {
    if (index == 0) {
        return { 0.0f, 0.0f };
    }
    ui32 x = index % atlasSize;
    ui32 y = index / atlasSize;
    return {(f32) x / (f32) atlasSize, (f32) y / (f32) atlasSize};
}

void Render::prepareRenderState(
        RenderState& renderState,
        const ECS::EntityComponentSystem& ecs,
        const Asset::AssetPack& assetPack,
        const Frustum& cameraFrustum) {

    LightData& lightData = renderState.lightData;
    EntityAssetGroupTable& entityAssetGroupTable = renderState.entityAssetGroupTable;
    ModelTransformCache& modelTransformCache = renderState.modelTransformCache;

    if (lightData.needsUpdate) {
        // Reset LightData
        lightData.numLights = 0;
    }

    // Reset EntityAssetGroupTable
    entityAssetGroupTable.numGroups = 0;
    memset(entityAssetGroupTable.numEntitiesArray, 0, sizeof(i32) * entityAssetGroupTable.maxGroups);
    if (entityAssetGroupTable.maxEntities != ecs.maxEntities) {
        entityAssetGroupTable.maxEntities = ecs.maxEntities;
        delete entityAssetGroupTable.groupTable;
        entityAssetGroupTable.groupTable = new UUID[entityAssetGroupTable.maxGroups * ecs.maxEntities];
    }

    // Initialize ModelTransformCache if necessary
    if (modelTransformCache.maxEntities != ecs.maxEntities) {
        initModelTransformCache(modelTransformCache, ecs.maxEntities);
    }

    // Iterate through all entities
    for (i32 entityId = 0; entityId < ecs.maxEntities; entityId++) {
        if (!ecs.entityExistsArray[entityId]) continue;

        bool hasRenderable3d  = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_RENDERABLE_3D);
        bool hasSpatial3d     = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
        bool hasCamera        = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_CAMERA);
        bool hasLight         = ECS::isComponentActive(ecs, entityId, ECS::COMPONENT_TYPE_LIGHT);

        if (lightData.needsUpdate) {
            // Add this entity to the light data
            if (hasLight && hasSpatial3d && lightData.numLights < MAX_NUM_LIGHTS) {
                const ECS::Component &spatial3d = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
                const ECS::Component &light = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_LIGHT);
                const ECS::ComponentInfo &spatial3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_SPATIAL_3D];
                const ECS::ComponentInfo &lightInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_LIGHT];
                Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
                Vector3f color = ECS::getField_Vector3f(light, lightInfo, ECS::Light::FIELD_INDEX_COLOR);
                Vector3f attenuation = ECS::getField_Vector3f(light, lightInfo, ECS::Light::FIELD_INDEX_ATTENUATION);
                lightData.lightPositions[lightData.numLights] = position;
                lightData.lightColors[lightData.numLights] = color;
                lightData.lightAttenuations[lightData.numLights] = attenuation;
                lightData.numLights++;
            }
        }

        // Add this entity to the entity asset group table
        if (hasRenderable3d && hasSpatial3d) {
            const ECS::Component &spatial3d = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
            const ECS::ComponentInfo &spatial3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_SPATIAL_3D];
            Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo,ECS::Spatial3d::FIELD_INDEX_POSITION);
            if (!MathUtils::isSphereInFrustum(cameraFrustum, position, 5.0f)) continue;

            // Only compute model transform matrix if we need to update it
            Matrix4f modelTransform{};
            if (modelTransformCache.needsUpdate[entityId]) {
                Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo,ECS::Spatial3d::FIELD_INDEX_POSITION);
                Vector3f rotation = ECS::getField_Vector3f(spatial3d, spatial3dInfo,ECS::Spatial3d::FIELD_INDEX_ROTATION);
                f32 scale = ECS::getField_f32(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_SCALE);
                MathUtils::createModelMatrix(position, rotation, scale, modelTransform);
                modelTransformCache.modelTransforms[entityId] = modelTransform;
                modelTransformCache.needsUpdate[entityId] = false;
            } else {
                modelTransform = modelTransformCache.modelTransforms[entityId];
            }

            const ECS::Component &renderable3d = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_RENDERABLE_3D);
            const ECS::ComponentInfo &renderable3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_RENDERABLE_3D];
            UUID meshId = ECS::getField_i32(renderable3d, renderable3dInfo, ECS::Renderable3d::FIELD_INDEX_MESH_ASSET_ID);
            UUID materialId = ECS::getField_i32(renderable3d, renderable3dInfo, ECS::Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID);
            if (meshId != -1 && materialId != -1) {
                {
                    Asset::MeshAsset* meshAsset = assetPack.meshAssets[meshId];
                    AABB::AABB aabb = meshAsset->aabb;
                    MathUtils::transformPoint(aabb.min, modelTransform, aabb.min);
                    MathUtils::transformPoint(aabb.max, modelTransform, aabb.max);
                    // TODO: Check if AABB is in frustum
                }

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

    lightData.needsUpdate = false;
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
    Matrix4f viewMatrix{};
    MathUtils::setIdentity(viewMatrix);
    MathUtils::rotateMatrix(viewMatrix, cameraRot.x, {1.0f, 0, 0}, viewMatrix);
    MathUtils::rotateMatrix(viewMatrix, cameraRot.y, {0, 1.0f, 0}, viewMatrix);
    MathUtils::rotateMatrix(viewMatrix, cameraRot.z, {0, 0, 1.0f}, viewMatrix);
    Vector3f negativePos = {cameraPos.x, cameraPos.y, cameraPos.z};
    MathUtils::translateMatrix(viewMatrix, negativePos, viewMatrix);

    // CAMERA FRUSTUM
    Frustum cameraFrustum{};
    Matrix4f projectionMatrix{};
    MathUtils::glmToMatrix4f(renderState.window.projectionMatrix, projectionMatrix);
    MathUtils::createCameraFrustum(viewMatrix, projectionMatrix, cameraFrustum);

    prepareRenderState(renderState, ecs, assetPack, cameraFrustum);
    LightData& lightData = renderState.lightData;
    EntityAssetGroupTable& entityAssetGroupTable = renderState.entityAssetGroupTable;

    // RENDER OBJECTS
    Asset::ShaderAsset* objectShader = assetPack.shaderAssets[0];
    bindShader(objectShader);
    setUniform(objectShader, Asset::ShaderUniform::VIEW_MATRIX, viewMatrix);
    setUniform(objectShader, Asset::ShaderUniform::PROJECTION_MATRIX, renderState.window.projectionMatrix);
    setUniform(objectShader, Asset::ShaderUniform::SKY_COLOR, renderState.window.backgroundColor);
    setUniform(objectShader, Asset::ShaderUniform::LIGHT_POSITIONS, lightData.lightPositions, lightData.numLights);
    setUniform(objectShader, Asset::ShaderUniform::LIGHT_COLORS, lightData.lightColors, lightData.numLights);
    setUniform(objectShader, Asset::ShaderUniform::LIGHT_ATTENUATIONS, lightData.lightAttenuations, lightData.numLights);
    setUniform(objectShader, Asset::ShaderUniform::LIGHT_COUNT, lightData.numLights);

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
            bindTexture(objectShader, texture, Asset::ShaderUniform::TEXTURE_SAMPLER, 0);
            setUniform(objectShader, Asset::ShaderUniform::SHINE_DAMPER, material->shineDamper);
            setUniform(objectShader, Asset::ShaderUniform::REFLECTIVITY, material->reflectivity);
            setUniform(objectShader, Asset::ShaderUniform::USE_FAKE_LIGHTING, material->useFakeLighting);
            setUniform(objectShader, Asset::ShaderUniform::TEXTURE_ATLAS_SIZE, (i32) texture->atlasSize);
            prevMaterialId = materialId;
        }

        i32 numEntitiesInGroup = entityAssetGroupTable.numEntitiesArray[groupIndex];
        for (size_t entityIt = 0; entityIt < numEntitiesInGroup; entityIt++) {
            UUID entityId = entityAssetGroupTable.groupTable[groupIndex * entityAssetGroupTable.maxEntities + entityIt];

            Matrix4f modelTransform = renderState.modelTransformCache.modelTransforms[entityId];

            ECS::Component& spatial3d = ECS::getComponent(ecs, entityId, ECS::COMPONENT_TYPE_SPATIAL_3D);
            const ECS::ComponentInfo& spatial3dInfo = ecs.componentTypesArray[ECS::COMPONENT_TYPE_SPATIAL_3D];

            setUniform(objectShader, Asset::ShaderUniform::MODEL_MATRIX, modelTransform);
            setUniform(objectShader, Asset::ShaderUniform::TEXTURE_ATLAS_OFFSET, calcTextureAtlasOffset(texture->atlasSize, 0));

            glDrawElements(GL_TRIANGLES, (i32) mesh->numIndices, GL_UNSIGNED_INT, nullptr);
        }
    }
    unbindTexture();
    unbindMesh();
    unbindShader();

    if (renderState.renderAABBs) { // RENDER AABBs
        Asset::ShaderAsset *aabbShader = assetPack.shaderAssets[1];
        bindShader(aabbShader);
        setUniform(aabbShader, Asset::ShaderUniform::VIEW_MATRIX, viewMatrix);
        setUniform(aabbShader, Asset::ShaderUniform::PROJECTION_MATRIX, renderState.window.projectionMatrix);
        for (i32 i = 0; i < numAssetGroups; i++) {
            i32 groupIndex = entityAssetGroupTable.renderOrderArray[i];
            UUID meshId = entityAssetGroupTable.meshIdArray[groupIndex];
            Asset::MeshAsset *meshAsset = assetPack.meshAssets[meshId];
            AABB::AABB aabb = meshAsset->aabb;
            AABB::bindAABBMesh(aabb.mesh);
            i32 numEntitiesInGroup = entityAssetGroupTable.numEntitiesArray[groupIndex];
            for (size_t entityIt = 0; entityIt < numEntitiesInGroup; entityIt++) {
                UUID entityId = entityAssetGroupTable.groupTable[groupIndex * entityAssetGroupTable.maxEntities +
                                                                 entityIt];
                Matrix4f modelTransform = renderState.modelTransformCache.modelTransforms[entityId];
                setUniform(aabbShader, Asset::ShaderUniform::MODEL_MATRIX, modelTransform);
                glDrawElements(GL_TRIANGLES, (i32) aabb.mesh.numIndices, GL_UNSIGNED_INT, nullptr);

            }
            AABB::unbindAABBMesh();
        }
        unbindShader();
    }

    glfwSwapBuffers(renderState.window.glfwWindow);
    glfwPollEvents();
}