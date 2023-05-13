#include "Asset.h"


UUID Asset::loadMaterialAsset(AssetPack& assetPack, UUID shaderAssetId, UUID textureAssetId, UUID normalMapAssetId) {
    // Create the material asset
    MaterialAsset* asset = new MaterialAsset();
    asset->shaderAssetId = shaderAssetId;
    asset->textureAssetId = textureAssetId;
    asset->normalMapAssetId = normalMapAssetId;
    asset->shineDamper = 1.0f;
    asset->reflectivity = 0.0f;
    asset->hasTransparency = false;
    asset->useFakeLighting = false;

    // Add the material asset to the asset pack
    UUID assetId = assetPack.nextMaterialAssetId;
    asset->assetId = assetId;
    assetPack.materialAssets[assetId] = asset;
    assetPack.numMaterialAssets++;
    assetPack.nextMaterialAssetId++;
    return assetId;
}