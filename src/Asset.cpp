#include "Asset.h"
#include "util/Image.h"
#include <iostream>
#include "util/StringUtils.h"


bool Asset::loadTextureAsset(TextureAsset* asset, const char* filePath) {
    Image::Image* image = new Image::Image();
    bool success = Image::loadImage(image, filePath);
    if (!success) {
        std::cout << "Failed to load image: " << filePath << "\n";
        return false;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            image->width,
            image->height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image->buffer);


    glEnable(GL_TEXTURE_2D);
    glGenerateMipmap(GL_TEXTURE_2D);
    gl
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 1.0f);

    glBindTexture(GL_TEXTURE_2D, 0);
    delete image;

    asset->assetId = 0;
    StringUtils::copyStringToBuffer(filePath, asset->filePath, CHAR_BUFFER_SIZE);
    asset->textureId = textureId;
    asset->atlasSize = 1;
    return true;
}