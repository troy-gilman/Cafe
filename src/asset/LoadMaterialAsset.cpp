#include "Asset.h"
#include "../util/UUIDGenerator.h"


bool Asset::loadMaterialAsset(MaterialAsset* asset, size_t shaderAssetId, size_t textureAssetId, size_t normalMapAssetId) {
    asset->assetId = UUIDGenerator::getInstance()->generateUUID();
    asset->shaderAssetId = shaderAssetId;
    asset->textureAssetId = textureAssetId;
    asset->normalMapAssetId = normalMapAssetId;
    return true;
}