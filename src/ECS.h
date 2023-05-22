#pragma once

#include <cstddef>
#include <vector>
#include "util/Types.h"

namespace ECS {

    // CONSTANTS
    static const i32 MAX_ENTITIES = 256;
    static const i32 MAX_COMPONENT_TYPES = 64;
    static const i32 MAX_FIELDS_PER_COMPONENT = 32;

    static const i32 COMPONENT_TYPE_CAMERA = 0;
    static const i32 COMPONENT_TYPE_LIGHT = 1;
    static const i32 COMPONENT_TYPE_RENDERABLE_3D = 2;
    static const i32 COMPONENT_TYPE_SPATIAL_3D = 3;
    static const i32 COMPONENT_TYPE_CONTROLLER_1P = 4;

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

    namespace Controller1p {
        static const char* COMPONENT_TYPE_STR = "Controller (First Person)";
        static const i32 FIELD_INDEX_MOVE_SPEED = 0;
        static const i32 FIELD_INDEX_MOUSE_SENSITIVITY = 1;
        static const i32 FIELD_INDEX_VERTICAL_VIEW_RANGE = 2;
    }

    static const i32 COMPONENT_NUM_BYTES_DATA = 256;

    struct Component {
        std::byte data[COMPONENT_NUM_BYTES_DATA];
    };

    enum ComponentFieldType {
        FIELD_TYPE_NONE,
        FIELD_TYPE_FLOAT,
        FIELD_TYPE_INTEGER,
        FIELD_TYPE_BOOLEAN,
        FIELD_TYPE_CHAR_BUFFER,
        FIELD_TYPE_VECTOR2F,
        FIELD_TYPE_VECTOR3F,
    };

    struct ComponentInfo {
        CharBuffer name;
        i32 numFields;
        CharBuffer fieldNames[MAX_FIELDS_PER_COMPONENT];
        ComponentFieldType fieldTypes[MAX_FIELDS_PER_COMPONENT];
        i32 fieldOffsetBytes[MAX_FIELDS_PER_COMPONENT];
        i32 fieldSizeBytes[MAX_FIELDS_PER_COMPONENT];
    };



    i32 addFieldToComponentInfo_i32(ComponentInfo& componentInfo, const char* fieldName);
    i32 addFieldToComponentInfo_f32(ComponentInfo& componentInfo, const char* fieldName);
    i32 addFieldToComponentInfo_Boolean(ComponentInfo& componentInfo, const char* fieldName);
    i32 addFieldToComponentInfo_Vector2f(ComponentInfo& componentInfo, const char* fieldName);
    i32 addFieldToComponentInfo_Vector3f(ComponentInfo& componentInfo, const char* fieldName);
    i32 addFieldToComponentInfo_CharBuffer(ComponentInfo& componentInfo, const char* fieldName, i32 numChars);

    f32 getField_f32(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex);
    void setField_f32(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, f32 value);
    i32 getField_i32(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex);
    void setField_i32(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, i32 value);
    bool getField_Boolean(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex);
    void setField_Boolean(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, bool value);
    Vector2f getField_Vector2f(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex);
    void setField_Vector2f(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, Vector2f value);
    Vector3f getField_Vector3f(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex);
    void setField_Vector3f(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, Vector3f value);
    const char* getField_CharBuffer(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex);
    void setField_CharBuffer(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, const char* value);

//    struct EntityComponentSystem {
//        i32 nextEntityId;
//        i32 numEntities;
//        bool entityExists[MAX_ENTITIES];
//        bool activeComponentTable[MAX_COMPONENT_TYPES][MAX_ENTITIES];
//        Component componentTable[MAX_COMPONENT_TYPES][MAX_ENTITIES];
//        i32 numComponentTypes;
//        ComponentInfo componentTypes[MAX_COMPONENT_TYPES];
//    };

    struct EntityComponentSystem {
        i32 prevEntityId;
        i32 numEntities;
        i32 maxEntities;
        bool* entityExistsArray;
        std::vector<Component> componentTable;
        bool* activeComponentTable;
        i32 numComponentTypes;
        i32 maxComponentTypes;
        std::vector<ComponentInfo> componentTypes;
    };

    void initEntityComponentSystem(EntityComponentSystem& ecs);
    bool isComponentActive(const EntityComponentSystem& ecs, UUID entityId, i32 componentType);
    void setComponentActive(EntityComponentSystem& ecs, UUID entityId, i32 componentType, bool isActive);
    Component& getComponent(const EntityComponentSystem& ecs, UUID entityId, i32 componentType);

    UUID createEntity(EntityComponentSystem& ecs);
    bool addComponentToEntity(EntityComponentSystem& ecs, UUID entityId, i32 componentType);
    bool addSpatial3dComponentToEntity(EntityComponentSystem& ecs, UUID entityId, Vector3f position, Vector3f rotation, f32 scale);
    bool addRenderable3dComponentToEntity(EntityComponentSystem& ecs, UUID entityId, UUID meshAssetId, UUID materialAssetId, i32 textureAtlasIndex);
    bool addCameraComponentToEntity(EntityComponentSystem& ecs, UUID entityId, f32 distanceFromTarget, f32 verticalAngle);
    bool addLightComponentToEntity(EntityComponentSystem& ecs, UUID entityId, Vector3f color, Vector3f attenuation);
    bool addController1pToEntity(EntityComponentSystem& ecs, UUID entityId, f32 moveSpeed, f32 mouseSensitivity, Vector2f verticalViewRange);
}