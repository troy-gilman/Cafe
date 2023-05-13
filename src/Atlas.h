#pragma once

#include "asset/Asset.h"
#include "Render.h"
#include "Input.h"

namespace Atlas {

    struct EngineState {
        ECS::EntityComponentSystem ecs;
        Asset::AssetPack assetPack;
        Render::RenderState renderState;
        Input::InputState input;
    };

    void init(EngineState* engineState);
    void render(EngineState* engineState);
};