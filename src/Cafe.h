#pragma once

#include "asset/Asset.h"
#include "Render.h"
#include "Input.h"

namespace Cafe {

    struct EngineState {
        ECS::EntityComponentSystem ecs;
        Asset::AssetPack assetPack;
        Render::RenderState renderState;
        Input::InputState input;
    };

    void init(EngineState* engine);
    void destroy(EngineState* engine);
    void update(EngineState* engine);
    void render(EngineState* engine);

    namespace Trait {
        void handleController1p_Move(ECS::EntityComponentSystem& ecs, const Input::InputState& input, f32 lastFrameTimeMs);
    }
};