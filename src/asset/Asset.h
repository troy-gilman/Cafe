#ifndef ASSET_H
#define ASSET_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../util/Types.h"

namespace Asset {

    struct TextureAsset {
        UUID assetId;
        CharBuffer filePath;
        size_t textureId;
        size_t atlasSize;
    };

    struct ShaderAsset {
        UUID assetId;
        CharBuffer vertexFilePath;
        CharBuffer fragmentFilePath;
        size_t programId;
        size_t vertexShaderId;
        size_t fragmentShaderId;
    };

    bool loadTextureAsset(TextureAsset* asset, const char* filePath);
    bool loadShaderAsset(ShaderAsset* asset, const char* vertexFilePath, const char* fragmentFilePath);
}

#endif //ASSET_H