#include "Asset.h"
#include "../util/UUIDGenerator.h"


bool Asset::loadMaterialAsset(MaterialAsset* asset, size_t shaderAssetId, size_t textureAssetId, size_t normalMapAssetId) {
    asset->assetId = UUIDGenerator::getInstance()->generateUUID();
    asset->shaderAssetId = shaderAssetId;
    asset->textureAssetId = textureAssetId;
    asset->normalMapAssetId = normalMapAssetId;
    return true;
}

bool Asset::addMaterialToAssetPack(AssetPack* assetPack, MaterialAsset* asset) {
    if (!assetPack || !asset) {
        return false;
    }
    if (assetPack->meshAssets.find(asset->assetId) != assetPack->meshAssets.end()) {
        return false;
    }
    assetPack->materialAssets[asset->assetId] = asset;
    return true;
}