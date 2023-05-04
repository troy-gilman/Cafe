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
        ui32 vao;
        ui32 vbo;
        ui32 tbo;
        ui32 nbo;
        ui32 ibo;
        ui32 numIndices;
    };

    struct MaterialAsset {
        UUID assetId;
        UUID shaderAssetId;
        UUID textureAssetId;
        UUID normalMapAssetId;
        f32 shineDamper;
        f32 reflectivity;
        bool hasTransparency;
        bool useFakeLighting;
    };

    struct TextureAsset {
        UUID assetId;
        CharBuffer filePath;
        ui32 textureId;
        ui32 atlasSize;
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
        TEXTURE_SAMPLER,
        SHINE_DAMPER,
        REFLECTIVITY,
        USE_FAKE_LIGHTING,
        TEXTURE_ATLAS_SIZE,
        TEXTURE_ATLAS_OFFSET,
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
        {TEXTURE_SAMPLER, "tex"},
        {SHINE_DAMPER, "shineDamper"},
        {REFLECTIVITY, "reflectivity"},
        {USE_FAKE_LIGHTING, "useFakeLighting"},
        {TEXTURE_ATLAS_SIZE, "textureAtlasSize"},
        {TEXTURE_ATLAS_OFFSET, "textureOffset"},
    };

    struct ShaderAsset {
        UUID assetId;
        CharBuffer vertexFilePath;
        CharBuffer fragmentFilePath;
        ui32 programId;
        ui32 vertexShaderId;
        ui32 fragmentShaderId;
        ui32 uniformLocations[TEXTURE_ATLAS_OFFSET + 1];
    };

    struct AssetPack {
        std::unordered_map<UUID, MeshAsset*> meshAssets;
        std::unordered_map<UUID, MaterialAsset*> materialAssets;
        std::unordered_map<UUID, TextureAsset*> textureAssets;
        std::unordered_map<UUID, ShaderAsset*> shaderAssets;
    };

    bool loadMeshAsset(MeshAsset* asset, const char* filePath);
    bool loadMaterialAsset(MaterialAsset* asset, UUID shaderAssetId, UUID textureAssetId, UUID normalMapAssetId);
    bool loadTextureAsset(TextureAsset* asset, const char* filePath);
    bool loadShaderAsset(ShaderAsset* asset, const char* vertexFilePath, const char* fragmentFilePath);
}

#endif //ASSET_H