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
    if (image->channels != 4) {
        std::cout << "Image must have 4 channels: " << filePath << "\n";
        return false;
    }
    //Image::flipImageHorizontally(image);
    //Image::flipImageVertically(image);


    // Create the texture
    ui32 textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set the texture wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set the texture scaling filter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set the texture parameters
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
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
    StringUtils::copyStringToBuffer(asset->filePath, filePath, CHAR_BUFFER_SIZE);
    asset->textureId = textureId;
    asset->atlasSize = 1;
    return true;
}