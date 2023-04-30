#include "ECS.h"
#include "util/StringUtils.h"


void ECS::initComponentTypes(EntityComponentSystem* ecs) {
    {   // Camera
        ComponentInfo componentInfo = {};
        StringUtils::copyStringToBuffer(componentInfo.name, Camera::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        componentInfo.numFields = 2;
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Camera::FIELD_INDEX_DISTANCE_FROM_TARGET], "DistanceFromTarget");
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Camera::FIELD_INDEX_VERTICAL_ANGLE], "VerticalAngle");
        componentInfo.fieldTypes[Camera::FIELD_INDEX_DISTANCE_FROM_TARGET] = FIELD_TYPE_FLOAT;
        componentInfo.fieldTypes[Camera::FIELD_INDEX_VERTICAL_ANGLE] = FIELD_TYPE_INTEGER;
        ecs->componentTypes[COMPONENT_TYPE_CAMERA] = componentInfo;
    }
    {   // Light
        ComponentInfo componentInfo = {};
        StringUtils::copyStringToBuffer(componentInfo.name, Light::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        componentInfo.numFields = 2;
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Light::FIELD_INDEX_COLOR], "Color");
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Light::FIELD_INDEX_ATTENUATION], "Attenuation");
        componentInfo.fieldTypes[Light::FIELD_INDEX_COLOR] = FIELD_TYPE_VECTOR3F;
        componentInfo.fieldTypes[Light::FIELD_INDEX_ATTENUATION] = FIELD_TYPE_VECTOR3F;
        ecs->componentTypes[COMPONENT_TYPE_LIGHT] = componentInfo;
    }
    {   // Renderable3D
        ComponentInfo componentInfo = {};
        StringUtils::copyStringToBuffer(componentInfo.name, Renderable3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        componentInfo.numFields = 3;
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Renderable3d::FIELD_INDEX_MESH_ASSET_ID], "MeshAssetId");
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID], "MaterialAssetId");
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Renderable3d::FIELD_INDEX_TEXTURE_ATLAS_INDEX], "TextureAtlasIndex");
        componentInfo.fieldTypes[Renderable3d::FIELD_INDEX_MESH_ASSET_ID] = FIELD_TYPE_UUID;
        componentInfo.fieldTypes[Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID] = FIELD_TYPE_UUID;
        componentInfo.fieldTypes[Renderable3d::FIELD_INDEX_TEXTURE_ATLAS_INDEX] = FIELD_TYPE_INTEGER;
        ecs->componentTypes[COMPONENT_TYPE_RENDERABLE_3D] = componentInfo;
    }
    {   // Spatial3D
        ComponentInfo componentInfo = {};
        StringUtils::copyStringToBuffer(componentInfo.name, Spatial3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        componentInfo.numFields = 3;
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Spatial3d::FIELD_INDEX_POSITION], "Position");
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Spatial3d::FIELD_INDEX_ROTATION], "Rotation");
        StringUtils::copyStringToBuffer(componentInfo.fieldNames[Spatial3d::FIELD_INDEX_SCALE], "Scale");
        componentInfo.fieldTypes[Spatial3d::FIELD_INDEX_POSITION] = FIELD_TYPE_VECTOR3F;
        componentInfo.fieldTypes[Spatial3d::FIELD_INDEX_ROTATION] = FIELD_TYPE_VECTOR3F;
        componentInfo.fieldTypes[Spatial3d::FIELD_INDEX_SCALE] = FIELD_TYPE_FLOAT;
        ecs->componentTypes[COMPONENT_TYPE_SPATIAL_3D] = componentInfo;
    }
    ecs->numComponentTypes = 4;
}

i32 ECS::registerComponentType(EntityComponentSystem* ecs, ComponentInfo componentInfo) {
    i32 componentType = ecs->numComponentTypes;
    ecs->numComponentTypes++;
    ecs->componentTypes[componentType] = componentInfo;
    return componentType;
}