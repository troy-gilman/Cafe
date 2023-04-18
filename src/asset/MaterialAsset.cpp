#include "Asset.h"
#include "../util/UUIDGenerator.h"


bool Asset::loadMaterialAsset(MaterialAsset* asset, UUID shaderAssetId, UUID textureAssetId, UUID normalMapAssetId) {
    asset->assetId = UUIDGenerator::getInstance()->generateUUID();
    asset->shaderAssetId = shaderAssetId;
    asset->textureAssetId = textureAssetId;
    asset->normalMapAssetId = normalMapAssetId;
    return true;
}