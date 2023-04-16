#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include "Types.h"

namespace Image {

    static const size_t MAX_IMAGE_SIZE_BYTES = 1024 * 1024 * 4;

    struct Image {
        size_t width;
        size_t height;
        size_t channels;
        unsigned char buffer[MAX_IMAGE_SIZE_BYTES];
        CharBuffer filePath;
    };

    bool loadImage(Image* image, const char* filePath);
}

#endif //IMAGE_H