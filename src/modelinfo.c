#include "global.h"
#include "modelinfo.h"
#include "recompdata.h"
#include "logger.h"
#include "playermodelmanager_utils.h"

void ModelInfo_init(ModelInfo *modelInfo) {
    modelInfo->gfx = 0;
    modelInfo->mtx = 0;
    modelInfo->gfxOverrides = recomputil_create_u32_value_hashmap();
    modelInfo->mtxOverrides = recomputil_create_u32_value_hashmap();
    ModelInfo_clearFlags(modelInfo);
    ModelInfo_unsetSkeleton(modelInfo);
    ModelInfo_unsetShieldingSkeleton(modelInfo);
    ModelInfo_clearEyesTextures(modelInfo);
    ModelInfo_clearMouthTextures(modelInfo);
}

Gfx *ModelInfo_getGfx(ModelInfo *modelInfo, Link_DisplayList id) {
    if (!isValidDisplayListID(id)) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getGfx called with invalid display list ID %d.", id);
    }

    uintptr_t gfx = 0;

    if (!recomputil_u32_value_hashmap_get(modelInfo->gfxOverrides, id, &gfx)) {
        if (modelInfo->gfx) {
            recomputil_u32_value_hashmap_get(modelInfo->gfx, id, &gfx);
        }
    }

    return (Gfx *)gfx;
}

Mtx *ModelInfo_getMtx(ModelInfo *modelInfo, Link_EquipmentMatrix id) {
    if (!isValidMatrixID(id)) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getMtx called with invalid matrix ID %d.", id);
        return NULL;
    }

    uintptr_t mtx = 0;

    if (!recomputil_u32_value_hashmap_get(modelInfo->mtxOverrides, id, &mtx)) {
        if (modelInfo->mtx) {
            recomputil_u32_value_hashmap_get(modelInfo->mtx, id, &mtx);
        }
    }

    return (Mtx *)mtx;
}

void ModelInfo_setEyesTextures(ModelInfo *modelInfo, TexturePtr eyeTextures[]) {
    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        modelInfo->eyesTextures[i] = eyeTextures[i];
    }
}

TexturePtr ModelInfo_getEyesTexture(ModelInfo *modelInfo, PlayerEyeIndex i) {
    if (i >= PLAYER_EYES_MAX || i < 0) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getEyesTexture called with invalid PlayerEyeIndex %d.", i);
        return (TexturePtr)NULL;
    }

    return modelInfo->eyesTextures[i];
}

void ModelInfo_setMouthTextures(ModelInfo *modelInfo, TexturePtr mouthTextures[]) {
    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        modelInfo->mouthTextures[i] = mouthTextures[i];
    }
}

TexturePtr ModelInfo_getMouthTexture(ModelInfo *modelInfo, PlayerMouthIndex i) {
    if (i >= PLAYER_MOUTH_MAX || i < 0) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getMouthTexture called with invalid PlayerMouthIndex %d.", i);
        return (TexturePtr)NULL;
    }

    return modelInfo->mouthTextures[i];
}

void ModelInfo_setSkeleton(ModelInfo *modelInfo, FlexSkeletonHeader *skel) {
    modelInfo->skeleton = skel;
}

void ModelInfo_unsetSkeleton(ModelInfo *modelInfo) {
    modelInfo->skeleton = NULL;
}

void ModelInfo_setShieldingSkeleton(ModelInfo *modelInfo, FlexSkeletonHeader *skel) {
    modelInfo->shieldingSkeleton = skel;
}

void ModelInfo_unsetShieldingSkeleton(ModelInfo *modelInfo) {
    modelInfo->shieldingSkeleton = NULL;
}

void ModelInfo_setGfxMap(ModelInfo *modelInfo, U32ValueHashmapHandle gfxMap) {
    modelInfo->gfx = gfxMap;
}

void ModelInfo_unsetGfxMap(ModelInfo *modelInfo) {
    modelInfo->gfx = 0;
}

bool ModelInfo_setGfxOverride(ModelInfo *modelInfo, Link_DisplayList id, Gfx *dl) {
    return recomputil_u32_value_hashmap_insert(modelInfo->gfxOverrides, id, (uintptr_t)dl);
}

bool ModelInfo_unsetGfxOverride(ModelInfo *modelInfo, Link_DisplayList id) {
    return recomputil_u32_value_hashmap_erase(modelInfo->gfxOverrides, id);
}

void ModelInfo_setMtxMap(ModelInfo *modelInfo, U32ValueHashmapHandle mtxMap) {
    modelInfo->mtx = mtxMap;
}

void ModelInfo_unsetMtxMap(ModelInfo *modelInfo) {
    modelInfo->mtx = 0;
}

bool ModelInfo_setMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id, Mtx *mtx) {
    return recomputil_u32_value_hashmap_insert(modelInfo->mtxOverrides, id, (uintptr_t)mtx);
}

bool ModelInfo_unsetMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id) {
    return recomputil_u32_value_hashmap_erase(modelInfo->mtxOverrides, id);
}

void ModelInfo_setFlags(ModelInfo *modelInfo, u64 flags) {
    modelInfo->flags |= flags;
}

void ModelInfo_unsetFlags(ModelInfo *modelInfo, u64 flags) {
    modelInfo->flags &= (~flags);
}

void ModelInfo_clearAllGfx(ModelInfo *modelInfo) {
    for (Link_DisplayList i = 0; i < LINK_DL_MAX; ++i) {
        ModelInfo_unsetGfx(modelInfo, i);
    }
}

void ModelInfo_clearAllGfxOverrides(ModelInfo *modelInfo) {
    for (Link_DisplayList i = 0; i < LINK_DL_MAX; ++i) {
        ModelInfo_unsetGfxOverride(modelInfo, i);
    }
}

void ModelInfo_clearAllMtx(ModelInfo *modelInfo) {
    for (Link_EquipmentMatrix i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        ModelInfo_unsetMtx(modelInfo, i);
    }
}

void ModelInfo_clearAllMtxOverrides(ModelInfo *modelInfo) {
    for (Link_EquipmentMatrix i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        ModelInfo_unsetMtxOverride(modelInfo, i);
    }
}

void ModelInfo_clearAllEyesTextures(ModelInfo *modelInfo) {
    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        modelInfo->eyesTextures[i] = 0;
    }
}

void ModelInfo_clearAllMouthTextures(ModelInfo *modelInfo) {
    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        modelInfo->mouthTextures[i] = 0;
    }
}

void ModelInfo_clearAllFlags(ModelInfo *modelInfo) {
    modelInfo->flags = 0;
}

bool ModelInfo_isAnyFlagEnabled(ModelInfo *modelInfo, u64 flags) {
    return modelInfo->flags & flags;
}
