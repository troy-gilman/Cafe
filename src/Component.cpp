#include "ECS.h"
#include "util/StringUtils.h"


void ECS::initEntityComponentSystem(EntityComponentSystem* ecs) {
    // Initialize Entities
    ecs->nextEntityId = 0;

    // Initialize Component Info
    {   // Camera
        ComponentInfo& componentInfo = ecs->componentTypes[COMPONENT_TYPE_CAMERA];
        StringUtils::copyStringToBuffer(componentInfo.name, Camera::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_f32(componentInfo, "DistanceFromTarget");
        addFieldToComponentInfo_i32(componentInfo, "VerticalAngle");
    }
    {   // Light
        ComponentInfo& componentInfo = ecs->componentTypes[COMPONENT_TYPE_LIGHT];
        StringUtils::copyStringToBuffer(componentInfo.name, Light::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_Vector3f(componentInfo, "Color");
        addFieldToComponentInfo_Vector3f(componentInfo, "Attenuation");
    }
    {   // Renderable3D
        ComponentInfo& componentInfo = ecs->componentTypes[COMPONENT_TYPE_RENDERABLE_3D];
        StringUtils::copyStringToBuffer(componentInfo.name, Renderable3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_i32(componentInfo, "MeshAssetId");
        addFieldToComponentInfo_i32(componentInfo, "MaterialAssetId");
        addFieldToComponentInfo_i32(componentInfo, "TextureAtlasIndex");
    }
    {   // Spatial3D
        ComponentInfo& componentInfo = ecs->componentTypes[COMPONENT_TYPE_SPATIAL_3D];
        StringUtils::copyStringToBuffer(componentInfo.name, Spatial3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_Vector3f(componentInfo, "Position");
        addFieldToComponentInfo_Vector3f(componentInfo, "Rotation");
        addFieldToComponentInfo_f32(componentInfo, "Scale");
    }
    ecs->numComponentTypes = 4;
}