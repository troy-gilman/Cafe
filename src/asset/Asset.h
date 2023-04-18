#ifndef ASSET_H
#define ASSET_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../util/Types.h"
#include <unordered_map>

namespace Asset {

    struct MeshAsset {
        UUID assetId;
        CharBuffer filePath;
        size_t vao;
        size_t vbo;
        size_t tbo;
        size_t nbo;
        size_t vertexCount;
    };

    struct MaterialAsset {
        UUID assetId;
        UUID shaderAssetId;
        UUID textureAssetId;
        UUID normalMapAssetId;
    };

    struct TextureAsset {
        UUID assetId;
        CharBuffer filePath;
        size_t textureId;
        size_t atlasSize;
    };

    enum ShaderUniform {
        VIEW_MATRIX,
        PROJECTION_MATRIX,
        MODEL_MATRIX,
        SKY_COLOR,
        LIGHT_POSITIONS,
        LIGHT_COLORS,
        LIGHT_ATTENUATIONS,
        LIGHT_COUNT,

    };

    inline std::unordered_map<ShaderUniform, const char*> shaderUniformNames = {
        {VIEW_MATRIX, "view"},
        {PROJECTION_MATRIX, "projection"},
        {MODEL_MATRIX, "model"},
        {SKY_COLOR, "skyColor"},
        {LIGHT_POSITIONS, "lightPositions"},
        {LIGHT_COLORS, "lightColors"},
        {LIGHT_ATTENUATIONS, "lightAttenuations"},
        {LIGHT_COUNT, "numLights"},
    };

    struct ShaderAsset {
        UUID assetId;
        CharBuffer vertexFilePath;
        CharBuffer fragmentFilePath;
        size_t programId;
        size_t vertexShaderId;
        size_t fragmentShaderId;
        std::unordered_map<ShaderUniform, int> uniformLocations;
    };

    struct AssetPack {
        std::unordered_map<UUID, MeshAsset*> meshAssets;
        std::unordered_map<UUID, MaterialAsset*> materialAssets;
        std::unordered_map<UUID, TextureAsset*> textureAssets;
        std::unordered_map<UUID, ShaderAsset*> shaderAssets;
    };

    bool loadMeshAsset(MeshAsset* asset, const char* filePath);
    bool loadMaterialAsset(MaterialAsset* asset, size_t shaderAssetId, size_t textureAssetId, size_t normalMapAssetId);
    bool loadTextureAsset(TextureAsset* asset, const char* filePath);
    bool loadShaderAsset(ShaderAsset* asset, const char* vertexFilePath, const char* fragmentFilePath);
}

#endif //ASSET_H