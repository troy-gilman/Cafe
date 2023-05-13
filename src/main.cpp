#include <iostream>
#include "Atlas.h"
#include "asset/Asset.h"



int main(int argc, char* argv[]) {

    Atlas::EngineState* engine = new Atlas::EngineState();
    Atlas::init(engine);

    UUID textureId = Asset::loadTextureAsset(engine->assetPack, "resources/models/tree/tree.png");
    UUID shaderId = Asset::loadShaderAsset(engine->assetPack, "resources/shaders/mainVertex.glsl", "resources/shaders/mainFragment.glsl");
    UUID materialId = Asset::loadMaterialAsset(engine->assetPack, shaderId, textureId, 0);
    UUID meshId = Asset::loadMeshAsset(engine->assetPack, "resources/models/tree/tree.obj");


    {   // Camera
        UUID entityId = ECS::createEntity(engine->ecs);
        ECS::addSpatial3dComponentToEntity(engine->ecs, entityId, {0, -2, -5}, {0, 0, 0}, 1);
        ECS::addCameraComponentToEntity(engine->ecs, entityId, 0.0f, 0.0f);
    }

    {   // Light
        UUID entityId = ECS::createEntity(engine->ecs);
        ECS::addSpatial3dComponentToEntity(engine->ecs, entityId, {100.0f, 100.0f, 100.0f}, {0, 0, 0}, 1);
        ECS::addLightComponentToEntity(engine->ecs, entityId, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f});
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 100; j++) {
            UUID entityId = ECS::createEntity(engine->ecs);
            ECS::addSpatial3dComponentToEntity(engine->ecs, entityId, {5.0f * i, 0, 5.0f*j}, {0, 0, 0}, 1);
            ECS::addRenderable3dComponentToEntity(engine->ecs, entityId, meshId, materialId, 0);
        }
    }

    Atlas::render(engine);
    return 0;
}