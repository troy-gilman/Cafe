#include "Asset.h"
#include "../util/Image.h"
#include "../util/StringUtils.h"
#include "../util/UUIDGenerator.h"
#include <iostream>

bool Asset::loadTextureAsset(TextureAsset* asset, const char* filePath) {
    // Load the image
    Image::Image* image = new Image::Image();

    // Check if the image loaded successfully
    bool success = Image::loadImage(image, filePath);
    if (!success) {
        std::cout << "Failed to load image: " << filePath << "\n";
        return false;
    }

    // Create the texture
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set the texture parameters
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


    // Generate the mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 1.0f);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    delete image;

    // Set the texture asset properties
    asset->assetId = UUIDGenerator::getInstance()->generateUUID();
    StringUtils::copyStringToBuffer(filePath, asset->filePath, CHAR_BUFFER_SIZE);
    asset->textureId = textureId;
    asset->atlasSize = 1;
    return true;
}