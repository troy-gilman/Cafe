#pragma once

#include <cstring>
#include "util/Types.h"
#include <unordered_map>

namespace Entity {

    struct Entity {
        UUID id;

        // Camera data
        f32 camera_DistanceFromTarget;
        f32 camera_VerticalAngle;

        // Light data
        Vector3f light_Color;
        Vector3f light_Attenuation;

        // Renderable3D data
        UUID renderable3D_MeshAssetId;
        UUID renderable3D_MaterialAssetId;
        ui32 renderable3D_TextureAtlasIndex;

        // Spatial3D data
        Vector3f spatial3D_Position;
        Vector3f spatial3D_Rotation;
        f32 spatial3D_Scale;

        Entity() {
            reset();
        }

        void reset() {
            id = 0;
            camera_DistanceFromTarget = 0.0f;
            camera_VerticalAngle = 0.0f;
            light_Color = { 0.0f, 0.0f, 0.0f };
            light_Attenuation = { 0.0f, 0.0f, 0.0f };
            renderable3D_MeshAssetId = 0;
            renderable3D_MaterialAssetId = 0;
            renderable3D_TextureAtlasIndex = 0;
            spatial3D_Position = { 0.0f, 0.0f, 0.0f };
            spatial3D_Rotation = { 0.0f, 0.0f, 0.0f };
            spatial3D_Scale = 0.0f;
        }
    };

    struct EntityState {
        std::unordered_map<UUID, Entity*> entities;

        EntityState() {
            entities.reserve(1024);
        }
    };
}