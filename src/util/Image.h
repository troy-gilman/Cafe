#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include "Types.h"

namespace Image {

    static const ui64 MAX_IMAGE_SIZE_BYTES = 1024 * 1024 * 4;

    struct Image {
        ui32 width;
        ui32 height;
        ui32 channels;
        unsigned char buffer[MAX_IMAGE_SIZE_BYTES];
        CharBuffer filePath;
    };

    bool loadImage(Image* image, const char* filePath);
}

#endif //IMAGE_H