#pragma once

#include <cstdint>


typedef uint64_t UUID;

static const size_t CHAR_BUFFER_SIZE = 128;
typedef char CharBuffer[CHAR_BUFFER_SIZE];

struct Vector2f {
    float x;
    float y;
};

struct Vector3f {
    float x;
    float y;
    float z;
};

