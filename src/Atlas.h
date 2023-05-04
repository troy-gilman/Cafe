#pragma once

#include <queue>
#include <atomic>
#include "Event.h"
#include "util/ThreadSafeQueue.h"
#include "Network.h"
#include "asset/Asset.h"
#include "Render.h"
#include "Input.h"

class Atlas {
private:
    bool isServer;
    Event::EventState* eventState;
    ECS::EntityComponentSystem* ecs;
    Asset::AssetPack* assetPack;
    Render::Window* window;
    Input::InputState* input;
    Network::NetworkState* networkState;

public:
    Atlas(bool isServer);
    ~Atlas();
    void init();
    void start();
    void render();

    bool addMeshToAssetPack(Asset::MeshAsset* meshAsset);
    bool addMaterialToAssetPack(Asset::MaterialAsset* materialAsset);
    bool addTextureToAssetPack(Asset::TextureAsset* textureAsset);
    bool addShaderToAssetPack(Asset::ShaderAsset* shaderAsset);

    UUID createEntity();
    bool addComponentToEntity(UUID entityId, i32 componentType);
    bool setComponentField(UUID entityId, i32 componentType, i32 fieldIndex, FieldUnion field);

    bool addSpatial3dComponentToEntity(UUID entityId, Vector3f position, Vector3f rotation, f32 scale);
    bool addRenderable3dComponentToEntity(UUID entityId, UUID meshAssetId, UUID materialAssetId, i32 textureAtlasIndex);

};