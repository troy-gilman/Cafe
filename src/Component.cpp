#include "ECS.h"
#include "util/StringUtils.h"


void ECS::initEntityComponentSystem(EntityComponentSystem* ecs) {
    // Initialize Entities
    ecs->nextEntityId = 0;

    // Initialize Component Info
    {   // Camera
        ComponentInfo* componentInfo = new ComponentInfo();
        StringUtils::copyStringToBuffer(componentInfo->name, Camera::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_f32(componentInfo, "DistanceFromTarget");
        addFieldToComponentInfo_i32(componentInfo, "VerticalAngle");
        ecs->componentTypes[COMPONENT_TYPE_CAMERA] = componentInfo;
    }
    {   // Light
        ComponentInfo* componentInfo = new ComponentInfo();
        StringUtils::copyStringToBuffer(componentInfo->name, Light::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_Vector3f(componentInfo, "Color");
        addFieldToComponentInfo_Vector3f(componentInfo, "Attenuation");
        ecs->componentTypes[COMPONENT_TYPE_LIGHT] = componentInfo;
    }
    {   // Renderable3D
        ComponentInfo* componentInfo = new ComponentInfo();
        StringUtils::copyStringToBuffer(componentInfo->name, Renderable3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_i32(componentInfo, "MeshAssetId");
        addFieldToComponentInfo_i32(componentInfo, "MaterialAssetId");
        addFieldToComponentInfo_i32(componentInfo, "TextureAtlasIndex");
        ecs->componentTypes[COMPONENT_TYPE_RENDERABLE_3D] = componentInfo;
    }
    {   // Spatial3D
        ComponentInfo* componentInfo = new ComponentInfo();
        StringUtils::copyStringToBuffer(componentInfo->name, Spatial3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_Vector3f(componentInfo, "Position");
        addFieldToComponentInfo_Vector3f(componentInfo, "Rotation");
        addFieldToComponentInfo_f32(componentInfo, "Scale");
        ecs->componentTypes[COMPONENT_TYPE_SPATIAL_3D] = componentInfo;
    }
    ecs->numComponentTypes = 4;
}

i32 ECS::registerComponentType(EntityComponentSystem* ecs, ComponentInfo* componentInfo) {
    i32 componentType = ecs->numComponentTypes;
    ecs->numComponentTypes++;
    ecs->componentTypes[componentType] = componentInfo;
    return componentType;
}