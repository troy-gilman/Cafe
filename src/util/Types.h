#pragma once

#include <cstdint>

typedef float f32;
typedef double f64;
typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef ui64 UUID;

struct Vector2f {
    f32 x;
    f32 y;
};

struct Vector3f {
    f32 x;
    f32 y;
    f32 z;
};

static const ui64 CHAR_BUFFER_SIZE = 64;
typedef char CharBuffer[CHAR_BUFFER_SIZE];

union FieldUnion {
    f32 field_Float;
    i32 field_Integer;
    bool field_Boolean;
    CharBuffer field_String;
    Vector3f field_Vector3f;
    UUID field_UUID;
};

enum FieldType {
    FIELD_TYPE_FLOAT,
    FIELD_TYPE_INTEGER,
    FIELD_TYPE_BOOLEAN,
    FIELD_TYPE_STRING,
    FIELD_TYPE_VECTOR3F,
    FIELD_TYPE_UUID
};

