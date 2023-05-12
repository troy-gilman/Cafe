#pragma once

#include "asset/Asset.h"
#include "Render.h"
#include "Input.h"

class Atlas {
private:
    ECS::EntityComponentSystem* ecs;
    Asset::AssetPack* assetPack;
    Render::RenderState* renderState;
    Input::InputState* input;

public:
    Atlas();
    ~Atlas();
    void init();
    void start();
    void render();

    bool addMeshToAssetPack(Asset::MeshAsset* meshAsset);
    bool addMaterialToAssetPack(Asset::MaterialAsset* materialAsset);
    bool addTextureToAssetPack(Asset::TextureAsset* textureAsset);
    bool addShaderToAssetPack(Asset::ShaderAsset* shaderAsset);

    UUID createEntity();

    bool addSpatial3dComponentToEntity(UUID entityId, Vector3f position, Vector3f rotation, f32 scale);
    bool addRenderable3dComponentToEntity(UUID entityId, UUID meshAssetId, UUID materialAssetId, i32 textureAtlasIndex);
    bool addCameraComponentToEntity(UUID entityId, f32 distanceFromTarget, f32 verticalAngle);
    bool addLightComponentToEntity(UUID entityId, Vector3f color, Vector3f attenuation);

};