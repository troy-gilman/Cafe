#pragma once

#include <cstring>
#include "Types.hpp"
#include <unordered_map>

namespace Entity {

    struct Entity {
        UUID id;

        // Camera data
        float camera_DistanceFromTarget;
        float camera_VerticalAngle;

        // Light data
        Vector3f light_Color;
        Vector3f light_Attenuation;

        // Renderable3D data
        UUID renderable3D_MeshAssetId;
        UUID renderable3D_MaterialAssetId;
        int renderable3D_TextureAtlasIndex;

        // Spatial3D data
        Vector3f spatial3D_Position;
        Vector3f spatial3D_Rotation;
        float spatial3D_Scale;

        Entity() {
            reset();
        }

        void reset() {
            id = 0;
            camera_DistanceFromTarget = 0.0f;
            camera_VerticalAngle = 0.0f;
            memset(light_Color, 0, sizeof(light_Color));
            memset(light_Attenuation, 0, sizeof(light_Attenuation));
            renderable3D_MeshAssetId = 0;
            renderable3D_MaterialAssetId = 0;
            renderable3D_TextureAtlasIndex = 0;
            memset(spatial3D_Position, 0, sizeof(spatial3D_Position));
            memset(spatial3D_Rotation, 0, sizeof(spatial3D_Rotation));
            spatial3D_Scale = 0.0f;
        }
    };

    struct EntityState {
        std::unordered_map<UUID, Entity> entities;

        EntityState() {
            entities.reserve(1024);
        }
    };
}