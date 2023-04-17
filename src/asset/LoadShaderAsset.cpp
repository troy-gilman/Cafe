#include "Asset.h"
#include "../util/StringUtils.h"
#include "../util/UUIDGenerator.h"
#include <iostream>

bool Asset::loadShaderAsset(ShaderAsset* asset, const char* vertexFilePath, const char* fragmentFilePath) {
    // Load the shader files as strings
    const char* vertexShaderAsString = StringUtils::loadFileAsString(vertexFilePath);
    const char* fragmentShaderAsString = StringUtils::loadFileAsString(fragmentFilePath);

    // Check if the shader files were loaded successfully
    if (vertexShaderAsString == nullptr || fragmentShaderAsString == nullptr) {
        std::cout << "Failed to load shader files\n";
        return false;
    }

    // Create the shader program
    size_t programId = glCreateProgram();
    size_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    size_t fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile the vertex shader
    glShaderSource(vertexShaderId, 1, &vertexShaderAsString, nullptr);
    glCompileShader(vertexShaderId);

    // Check if the vertex shader compiled successfully
    GLint vertexCompileStatus;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &vertexCompileStatus);
    if (vertexCompileStatus == GL_FALSE) {
        std::cout << "Failed to compile vertex shader\n";
        return false;
    }

    // Compile the fragment shader
    glShaderSource(fragmentShaderId, 1, &fragmentShaderAsString, nullptr);
    glCompileShader(fragmentShaderId);

    // Check if the fragment shader compiled successfully
    GLint fragmentCompileStatus;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &fragmentCompileStatus);
    if (fragmentCompileStatus == GL_FALSE) {
        std::cout << "Failed to compile fragment shader\n";
        return false;
    }

    // Link the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glBindAttribLocation(programId, 0, "position");
    glBindAttribLocation(programId, 1, "textureCoords");
    glBindAttribLocation(programId, 2, "normal");
    glLinkProgram(programId);

    // Check if the shader program linked successfully
    GLint programLinkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &programLinkStatus);
    if (programLinkStatus == GL_FALSE) {
        std::cout << "Failed to link shader program\n";
        return false;
    }

    // Validate the shader program and check if it is valid
    glValidateProgram(programId);
    GLint programValidateStatus;
    glGetProgramiv(programId, GL_VALIDATE_STATUS, &programValidateStatus);
    if (programValidateStatus == GL_FALSE) {
        std::cout << "Failed to validate shader program\n";
        return false;
    }

    // Set the shader asset properties
    asset->assetId = UUIDGenerator::getInstance()->generateUUID();
    StringUtils::copyStringToBuffer(vertexFilePath, asset->vertexFilePath, CHAR_BUFFER_SIZE);
    StringUtils::copyStringToBuffer(fragmentFilePath, asset->fragmentFilePath, CHAR_BUFFER_SIZE);
    asset->programId = programId;
    asset->vertexShaderId = vertexShaderId;
    asset->fragmentShaderId = fragmentShaderId;

    return true;
}