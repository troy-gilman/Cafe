#pragma once

#include <cstring>
#include "util/Types.h"
#include <unordered_map>

namespace ECS {

    // CONSTANTS
    static const ui32 MAX_COMPONENT_TYPES = 256;
    static const ui32 MAX_FIELDS_PER_COMPONENT = 16;

    static const i32 COMPONENT_TYPE_CAMERA = 0;
    static const i32 COMPONENT_TYPE_LIGHT = 1;
    static const i32 COMPONENT_TYPE_RENDERABLE_3D = 2;
    static const i32 COMPONENT_TYPE_SPATIAL_3D = 3;

    namespace Camera {
        static const char* COMPONENT_TYPE_STR = "Camera";
        static const i32 FIELD_INDEX_DISTANCE_FROM_TARGET = 0;
        static const i32 FIELD_INDEX_VERTICAL_ANGLE = 1;
    }

    namespace Light {
        static const char* COMPONENT_TYPE_STR = "Light";
        static const i32 FIELD_INDEX_COLOR = 0;
        static const i32 FIELD_INDEX_ATTENUATION = 1;
    }

    namespace Renderable3d {
        static const char* COMPONENT_TYPE_STR = "Renderable3d";
        static const i32 FIELD_INDEX_MESH_ASSET_ID = 0;
        static const i32 FIELD_INDEX_MATERIAL_ASSET_ID = 1;
        static const i32 FIELD_INDEX_TEXTURE_ATLAS_INDEX = 2;
    }

    namespace Spatial3d {
        static const char* COMPONENT_TYPE_STR = "Spatial3d";
        static const i32 FIELD_INDEX_POSITION = 0;
        static const i32 FIELD_INDEX_ROTATION = 1;
        static const i32 FIELD_INDEX_SCALE = 2;
    }

    struct Component {
        i32 type;
        FieldUnion fields[MAX_FIELDS_PER_COMPONENT];
    };

    struct ComponentInfo {
        CharBuffer name;
        i32 numFields;
        CharBuffer fieldNames[MAX_FIELDS_PER_COMPONENT];
        FieldType fieldTypes[MAX_FIELDS_PER_COMPONENT];
    };

    struct Entity  {
        UUID id;
        std::unordered_map<i32, Component*> components;
    };

    struct EntityComponentSystem {
        std::unordered_map<UUID, Entity*> entities;
        i32 numComponentTypes;
        ComponentInfo componentTypes[MAX_COMPONENT_TYPES];

        EntityComponentSystem() {
            entities.reserve(1024);
        }
    };

    void initComponentTypes(EntityComponentSystem* ecs);
    i32 registerComponentType(EntityComponentSystem* ecs, ComponentInfo componentInfo);
}