#include "ECS.h"
#include <cstdlib>
#include <stdexcept>
#include "util/StringUtils.h"

using namespace ECS;

i32 addFieldToComponentInfo(ComponentInfo& componentInfo, const char* fieldName, ComponentFieldType fieldType, i32 fieldByteSize) {
    i32 fieldIndex = componentInfo.numFields;
    StringUtils::copyStringToBuffer(componentInfo.fieldNames[fieldIndex], fieldName);
    componentInfo.fieldTypes[fieldIndex] = fieldType;
    i32 prevFieldByteOffset = componentInfo.fieldOffsetBytes[fieldIndex - 1];
    i32 prevFieldByteSize = componentInfo.fieldSizeBytes[fieldIndex - 1];
    componentInfo.fieldOffsetBytes[fieldIndex] = prevFieldByteOffset + prevFieldByteSize;
    componentInfo.fieldSizeBytes[fieldIndex] = fieldByteSize;
    componentInfo.numFields++;
    if (componentInfo.fieldOffsetBytes[fieldIndex] + componentInfo.fieldSizeBytes[fieldIndex] > COMPONENT_NUM_BYTES_DATA) {
        printf("ERROR: ComponentInfo::addFieldToComponentInfo: field %s exceeds component data size\n", fieldName);
        exit(1);
    }
    if (componentInfo.numFields > MAX_FIELDS_PER_COMPONENT) {
        printf("ERROR: ComponentInfo::addFieldToComponentInfo: too many fields\n");
        exit(1);
    }
    return fieldIndex;
}

i32 ECS::addFieldToComponentInfo_i32(ComponentInfo& componentInfo, const char* fieldName) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_INTEGER, sizeof(i32));
}

i32 ECS::addFieldToComponentInfo_f32(ComponentInfo& componentInfo, const char* fieldName) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_FLOAT, sizeof(f32));
}

i32 ECS::addFieldToComponentInfo_Boolean(ComponentInfo& componentInfo, const char* fieldName) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_BOOLEAN, sizeof(bool));
}

i32 ECS::addFieldToComponentInfo_Vector2f(ComponentInfo& componentInfo, const char* fieldName) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_VECTOR2F, sizeof(Vector2f));
}

i32 ECS::addFieldToComponentInfo_Vector3f(ComponentInfo& componentInfo, const char* fieldName) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_VECTOR3F, sizeof(Vector3f));
}

i32 ECS::addFieldToComponentInfo_CharBuffer(ComponentInfo& componentInfo, const char* fieldName, i32 numChars) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_CHAR_BUFFER, numChars * (i32) sizeof(char));
}

i32 ECS::addFieldToComponentInfo_Matrix4f(ComponentInfo& componentInfo, const char* fieldName) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_MATRIX4F, sizeof(Matrix4f));
}

f32 ECS::getField_f32(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    return *(f32*)(component.data + byteOffset);
}

void ECS::setField_f32(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, f32 value) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    *(f32*)(component.data + byteOffset) = value;
}

i32 ECS::getField_i32(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    return *(i32*)(component.data + byteOffset);
}

void ECS::setField_i32(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, i32 value) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    *(i32*)(component.data + byteOffset) = value;
}

bool ECS::getField_Boolean(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    return *(bool*)(component.data + byteOffset);
}

void ECS::setField_Boolean(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, bool value) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    *(bool*)(component.data + byteOffset) = value;
}

Vector2f ECS::getField_Vector2f(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    return *(Vector2f*)(component.data + byteOffset);
}

void ECS::setField_Vector2f(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, Vector2f value) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    *(Vector2f*)(component.data + byteOffset) = value;
}

Vector3f ECS::getField_Vector3f(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    return *(Vector3f*)(component.data + byteOffset);
}

void ECS::setField_Vector3f(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, Vector3f value) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    *(Vector3f*)(component.data + byteOffset) = value;
}

const char* ECS::getField_CharBuffer(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    return (const char*)(component.data + byteOffset);
}

void ECS::setField_CharBuffer(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, const char* value) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    i32 numChars = componentInfo.fieldSizeBytes[fieldIndex];
    StringUtils::copyStringToBuffer((char*)(component.data + byteOffset), value, numChars);
}

Matrix4f ECS::getField_Matrix4f(const Component& component, const ComponentInfo& componentInfo, i32 fieldIndex) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    return *(Matrix4f*)(component.data + byteOffset);
}

void ECS::setField_Matrix4f(Component& component, const ComponentInfo& componentInfo, i32 fieldIndex, Matrix4f value) {
    i32 byteOffset = componentInfo.fieldOffsetBytes[fieldIndex];
    *(Matrix4f*)(component.data + byteOffset) = value;
}

void ECS::initEntityComponentSystem(EntityComponentSystem& ecs) {
    // Delete old data
    delete ecs.entityExistsArray;
    delete ecs.activeComponentTable;
    delete ecs.componentTable;
    delete ecs.componentTypesArray;

    // Initialize data
    ecs.numEntities = 0;
    ecs.prevEntityId = 0;
    ecs.maxEntities = MAX_ENTITIES;
    ecs.maxComponentTypes = MAX_COMPONENT_TYPES;
    ecs.entityExistsArray = new bool[ecs.maxEntities];
    ecs.activeComponentTable = new bool[ecs.maxComponentTypes * ecs.maxEntities];
    ecs.componentTable = new Component[ecs.maxComponentTypes * ecs.maxEntities];
    ecs.componentTypesArray = new ComponentInfo[ecs.maxComponentTypes];
    memset(ecs.entityExistsArray, 0, ecs.maxEntities * sizeof(bool));
    memset(ecs.activeComponentTable, 0, ecs.maxComponentTypes * ecs.maxEntities * sizeof(bool));
    memset(ecs.componentTable, 0, ecs.maxComponentTypes * ecs.maxEntities * sizeof(Component));
    memset(ecs.componentTypesArray, 0, ecs.maxComponentTypes * sizeof(ComponentInfo));

    // Initialize Component Info
    {   // Camera
        ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_CAMERA];
        StringUtils::copyStringToBuffer(componentInfo.name, Camera::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_f32(componentInfo, "DistanceFromTarget");
        addFieldToComponentInfo_i32(componentInfo, "VerticalAngle");
    }
    {   // Light
        ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_LIGHT];
        StringUtils::copyStringToBuffer(componentInfo.name, Light::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_Vector3f(componentInfo, "Color");
        addFieldToComponentInfo_Vector3f(componentInfo, "Attenuation");
    }
    {   // Renderable3D
        ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_RENDERABLE_3D];
        StringUtils::copyStringToBuffer(componentInfo.name, Renderable3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_i32(componentInfo, "MeshAssetId");
        addFieldToComponentInfo_i32(componentInfo, "MaterialAssetId");
        addFieldToComponentInfo_i32(componentInfo, "TextureAtlasIndex");
    }
    {   // Spatial3D
        ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_SPATIAL_3D];
        StringUtils::copyStringToBuffer(componentInfo.name, Spatial3d::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_Vector3f(componentInfo, "Position");
        addFieldToComponentInfo_Vector3f(componentInfo, "Rotation");
        addFieldToComponentInfo_f32(componentInfo, "Scale");
    }
    {   // Controller1p
        ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_CONTROLLER_1P];
        StringUtils::copyStringToBuffer(componentInfo.name, Controller1p::COMPONENT_TYPE_STR, CHAR_BUFFER_SIZE);
        addFieldToComponentInfo_f32(componentInfo, "MoveSpeed");
        addFieldToComponentInfo_f32(componentInfo, "MouseSensitivity");
        addFieldToComponentInfo_Vector2f(componentInfo, "VerticalViewRange");
    }
    ecs.numComponentTypes = 5;
}

bool ECS::isComponentActive(const EntityComponentSystem& ecs, UUID entityId, i32 componentType) {
    i32 index = componentType * ecs.maxEntities + entityId;
    if (index >= ecs.maxComponentTypes * ecs.maxEntities) throw std::runtime_error("Invalid component type or entity id");
    return ecs.activeComponentTable[index];
}

void ECS::setComponentActive(EntityComponentSystem& ecs, UUID entityId, i32 componentType, bool isActive) {
    i32 index = componentType * ecs.maxEntities + entityId;
    if (index >= ecs.maxComponentTypes * ecs.maxEntities) throw std::runtime_error("Invalid component type or entity id");
    ecs.activeComponentTable[componentType * ecs.maxEntities + entityId] = isActive;
}

Component& ECS::getComponent(const EntityComponentSystem& ecs, UUID entityId, i32 componentType) {
    i32 index = componentType * ecs.maxEntities + entityId;
    if (index >= ecs.maxComponentTypes * ecs.maxEntities) throw std::runtime_error("Invalid component type or entity id");
    return ecs.componentTable[componentType * ecs.maxEntities + entityId];
}

UUID ECS::createEntity(EntityComponentSystem& ecs) {
    // Increase max entities if necessary (2x)
    if (ecs.numEntities == ecs.maxEntities) {
        // Allocate new data
        i32 newMaxEntities = ecs.maxEntities * 2;
        Component* temp = new Component[ecs.maxComponentTypes * newMaxEntities];
        bool* tempActive = new bool[ecs.maxComponentTypes * newMaxEntities];
        bool* tempEntityExists = new bool[newMaxEntities];
        memset(temp, 0, ecs.maxComponentTypes * newMaxEntities * sizeof(Component));
        memset(tempActive, 0, ecs.maxComponentTypes * newMaxEntities * sizeof(bool));
        memset(tempEntityExists, 0, newMaxEntities * sizeof(bool));

        // Copy old data
        for (i32 componentType = 0; componentType < ecs.maxComponentTypes; componentType++) {
            i32 offset = componentType * ecs.maxEntities;
            i32 tempOffset = componentType * newMaxEntities;
            memcpy(temp + tempOffset, ecs.componentTable + offset, ecs.maxEntities * sizeof(Component));
            memcpy(tempActive + tempOffset, ecs.activeComponentTable + offset, ecs.maxEntities * sizeof(bool));
        }
        memcpy(tempEntityExists, ecs.entityExistsArray, ecs.maxEntities * sizeof(bool));

        // Delete and replace old data
        delete ecs.componentTable;
        delete ecs.activeComponentTable;
        delete ecs.entityExistsArray;
        ecs.componentTable = temp;
        ecs.activeComponentTable = tempActive;
        ecs.entityExistsArray = tempEntityExists;
        ecs.maxEntities = newMaxEntities;
    }

    // Find next available entity id
    i32 entityId = ecs.prevEntityId;
    while (ecs.entityExistsArray[entityId]) {
        entityId = (entityId + 1) % ecs.maxEntities;
        if (entityId == ecs.prevEntityId) {
            throw std::runtime_error("No more available entity ids");
        }
    }

    // Initialize entity
    ecs.entityExistsArray[entityId] = true;
    for (i32 componentType = 0; componentType < ecs.maxComponentTypes; componentType++) {
        ECS::setComponentActive(ecs, entityId, componentType, false);
    }
    ecs.prevEntityId = entityId;
    ecs.numEntities++;

    return entityId;
}

bool ECS::addComponentToEntity(EntityComponentSystem& ecs, UUID entityId, i32 componentType) {
    if (entityId < 0 || entityId >= ecs.maxEntities) return false;
    if (!ecs.entityExistsArray[entityId]) return false;
    if (ECS::isComponentActive(ecs, entityId, componentType)) return false;
    Component& component = ECS::getComponent(ecs, entityId, componentType);
    memset(&component, 0, sizeof(Component));
    ECS::setComponentActive(ecs, entityId, componentType, true);
    return true;
}

bool ECS::addSpatial3dComponentToEntity(EntityComponentSystem& ecs, UUID entityId, Vector3f position, Vector3f rotation, f32 scale) {
    bool success = addComponentToEntity(ecs, entityId, COMPONENT_TYPE_SPATIAL_3D);
    if (!success) return false;
    const ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_SPATIAL_3D];
    Component& component = ECS::getComponent(ecs, entityId, COMPONENT_TYPE_SPATIAL_3D);
    setField_Vector3f(component, componentInfo, Spatial3d::FIELD_INDEX_POSITION, position);
    setField_Vector3f(component, componentInfo, Spatial3d::FIELD_INDEX_ROTATION, rotation);
    setField_f32(component, componentInfo, Spatial3d::FIELD_INDEX_SCALE, scale);
    return true;
}

bool ECS::addRenderable3dComponentToEntity(EntityComponentSystem& ecs, UUID entityId, UUID meshAssetId, UUID materialAssetId, i32 textureAtlasIndex) {
    bool success = addComponentToEntity(ecs, entityId, COMPONENT_TYPE_RENDERABLE_3D);
    if (!success) return false;
    const ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_RENDERABLE_3D];
    Component& component = ECS::getComponent(ecs, entityId, COMPONENT_TYPE_RENDERABLE_3D);
    setField_i32(component, componentInfo, Renderable3d::FIELD_INDEX_MESH_ASSET_ID, meshAssetId);
    setField_i32(component, componentInfo, Renderable3d::FIELD_INDEX_MATERIAL_ASSET_ID, materialAssetId);
    setField_i32(component, componentInfo, Renderable3d::FIELD_INDEX_TEXTURE_ATLAS_INDEX, textureAtlasIndex);
    return true;
}

bool ECS::addCameraComponentToEntity(EntityComponentSystem& ecs, UUID entityId, f32 distanceFromTarget, f32 verticalAngle) {
    bool success = addComponentToEntity(ecs, entityId, COMPONENT_TYPE_CAMERA);
    if (!success) return false;
    const ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_CAMERA];
    Component& component = ECS::getComponent(ecs, entityId, COMPONENT_TYPE_CAMERA);
    setField_f32(component, componentInfo, Camera::FIELD_INDEX_DISTANCE_FROM_TARGET, distanceFromTarget);
    setField_f32(component, componentInfo, Camera::FIELD_INDEX_VERTICAL_ANGLE, verticalAngle);
    return true;
}

bool ECS::addLightComponentToEntity(EntityComponentSystem& ecs, UUID entityId, Vector3f color, Vector3f attenuation) {
    bool success = addComponentToEntity(ecs, entityId, COMPONENT_TYPE_LIGHT);
    if (!success) return false;
    const ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_LIGHT];
    Component& component = ECS::getComponent(ecs, entityId, COMPONENT_TYPE_LIGHT);
    setField_Vector3f(component, componentInfo, Light::FIELD_INDEX_COLOR, color);
    setField_Vector3f(component, componentInfo, Light::FIELD_INDEX_ATTENUATION, attenuation);
    return true;
}

bool ECS::addController1pToEntity(EntityComponentSystem& ecs, UUID entityId, f32 moveSpeed, f32 mouseSensitivity, Vector2f verticalViewRange) {
    bool success = addComponentToEntity(ecs, entityId, COMPONENT_TYPE_CONTROLLER_1P);
    if (!success) return false;
    const ComponentInfo& componentInfo = ecs.componentTypesArray[COMPONENT_TYPE_CONTROLLER_1P];
    Component& component = ECS::getComponent(ecs, entityId, COMPONENT_TYPE_CONTROLLER_1P);
    setField_f32(component, componentInfo, Controller1p::FIELD_INDEX_MOVE_SPEED, moveSpeed);
    setField_f32(component, componentInfo, Controller1p::FIELD_INDEX_MOUSE_SENSITIVITY, mouseSensitivity);
    setField_Vector2f(component, componentInfo, Controller1p::FIELD_INDEX_VERTICAL_VIEW_RANGE, verticalViewRange);
    return true;
}
