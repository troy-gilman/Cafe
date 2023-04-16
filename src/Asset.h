#ifndef ASSET_H
#define ASSET_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/Types.h"

namespace Asset {

    struct TextureAsset {
        UUID assetId;
        CharBuffer filePath;
        size_t textureId;
        size_t atlasSize;
    };

    bool loadTextureAsset(TextureAsset* asset, const char* filePath);
}

#endif //ASSET_H