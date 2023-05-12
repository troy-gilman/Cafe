#include "ECS.h"
#include <cstdlib>
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

i32 ECS::addFieldToComponentInfo_Vector3f(ComponentInfo& componentInfo, const char* fieldName) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_VECTOR3F, sizeof(Vector3f));
}

i32 ECS::addFieldToComponentInfo_CharBuffer(ComponentInfo& componentInfo, const char* fieldName, i32 numChars) {
    return addFieldToComponentInfo(componentInfo, fieldName, FIELD_TYPE_CHAR_BUFFER, numChars * (i32) sizeof(char));
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

