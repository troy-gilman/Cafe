#pragma once

#include <cstring>
#include "util/Types.h"
#include <unordered_map>

namespace ECS {

    // CONSTANTS
    static const ui32 MAX_ENTITIES = 1024;
    static const ui32 MAX_COMPONENT_TYPES = 64;
    static const ui32 MAX_FIELDS_PER_COMPONENT = 32;

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
        std::byte data[252];
    };

    enum ComponentFieldType {
        FIELD_TYPE_FLOAT,
        FIELD_TYPE_INTEGER,
        FIELD_TYPE_BOOLEAN,
        FIELD_TYPE_STRING,
        FIELD_TYPE_VECTOR3F,
        FIELD_TYPE_UUID
    };

    struct ComponentInfo {
        CharBuffer name;
        i32 numFields;
        CharBuffer fieldNames[MAX_FIELDS_PER_COMPONENT];
        ComponentFieldType fieldTypes[MAX_FIELDS_PER_COMPONENT];
        i32 fieldByteOffsets[MAX_FIELDS_PER_COMPONENT];
        i32 fieldByteSizes[MAX_FIELDS_PER_COMPONENT];
    };

    inline f32 getField_f32(Component* component, ComponentInfo* componentInfo, i32 fieldIndex) {
        return *(f32*)(component->data + componentInfo->fieldByteOffsets[fieldIndex]);
    }

    inline void setField_f32(Component* component, ComponentInfo* componentInfo, i32 fieldIndex, f32 value) {
        *(f32*)(component->data + componentInfo->fieldByteOffsets[fieldIndex]) = value;
    }

    inline i32 getField_i32(Component* component, ComponentInfo* componentInfo, i32 fieldIndex) {
        return *(i32*)(component->data + componentInfo->fieldByteOffsets[fieldIndex]);
    }

    inline void setField_i32(Component* component, ComponentInfo* componentInfo, i32 fieldIndex, i32 value) {
        *(i32*)(component->data + componentInfo->fieldByteOffsets[fieldIndex]) = value;
    }

    inline Vector3f getField_Vector3f(Component* component, ComponentInfo* componentInfo, i32 fieldIndex) {
        return *(Vector3f*)(component->data + componentInfo->fieldByteOffsets[fieldIndex]);
    }

    inline void setField_Vector3f(Component* component, ComponentInfo* componentInfo, i32 fieldIndex, Vector3f value) {
        *(Vector3f*)(component->data + componentInfo->fieldByteOffsets[fieldIndex]) = value;
    }

    struct Entity  {
        UUID id;
        Component* components[MAX_COMPONENT_TYPES];
    };

    struct EntityComponentSystem {
        i32 nextEntityId;
        i32 numEntities;
        Entity* entities[MAX_ENTITIES];
        i32 numComponentTypes;
        ComponentInfo* componentTypes[MAX_COMPONENT_TYPES];
    };

    void initEntityComponentSystem(EntityComponentSystem* ecs);
    i32 registerComponentType(EntityComponentSystem* ecs, ComponentInfo* componentInfo);
}