#include "global.h"
#include "modelinfo.h"
#include "recompdata.h"
#include "logger.h"
#include "playermodelmanager_utils.h"

void ModelInfo_init(ModelInfo *modelInfo) {
    modelInfo->modelEntryForm = NULL;
    modelInfo->gfxOverrides = recomputil_create_u32_value_hashmap();
    modelInfo->mtxOverrides = recomputil_create_u32_value_hashmap();
}

bool ModelInfo_hasModelEntry(ModelInfo *modelInfo) {
    return modelInfo->modelEntryForm;
}

Gfx *ModelInfo_getGfx(ModelInfo *modelInfo, Link_DisplayList id) {
    if (id >= LINK_DL_MAX || id < 0) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getGfx called with invalid display list ID %d.", id);
    }

    uintptr_t gfx = 0;

    if (!recomputil_u32_value_hashmap_get(modelInfo->gfxOverrides, id, &gfx)) {
        if (modelInfo->modelEntryForm) {
            gfx = (uintptr_t)ModelEntry_getDisplayList(&modelInfo->modelEntryForm->modelEntry, id);
        }
    }

    return (Gfx *)gfx;
}

Mtx *ModelInfo_getMtx(ModelInfo *modelInfo, Link_EquipmentMatrix id) {
    if (id >= LINK_EQUIP_MATRIX_MAX || id < 0) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getMtx called with invalid matrix ID %d.", id);
        return NULL;
    }

    uintptr_t mtx = 0;

    if (!recomputil_u32_value_hashmap_get(modelInfo->mtxOverrides, id, &mtx)) {
        if (modelInfo->modelEntryForm) {
            mtx = (uintptr_t)ModelEntry_getMatrix(&modelInfo->modelEntryForm->modelEntry, id);
        }
    }

    return (Mtx *)mtx;
}

void ModelInfo_setModelEntryForm(ModelInfo *modelInfo, ModelEntryForm *modelEntry) {
    modelInfo->modelEntryForm = modelEntry;
}

ModelEntryForm *ModelInfo_getModelEntryForm(ModelInfo *modelInfo) {
    return modelInfo->modelEntryForm;
}

TexturePtr ModelInfo_getEyesTexture(ModelInfo *modelInfo, PlayerEyeIndex i) {
    if (i >= PLAYER_EYES_MAX || i < 0) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getEyesTexture called with invalid PlayerEyeIndex %d.", i);
        return (TexturePtr)NULL;
    }

    if (!modelInfo->modelEntryForm) {
        return (TexturePtr)NULL;
    }

    return ModelEntryForm_getEyesTexture(modelInfo->modelEntryForm, i);
}

TexturePtr ModelInfo_getMouthTexture(ModelInfo *modelInfo, PlayerMouthIndex i) {
    if (i >= PLAYER_MOUTH_MAX || i < 0) {
        Logger_printWarning("PlayerModelManager: ModelInfo_getMouthTexture called with invalid PlayerMouthIndex %d.", i);
        return (TexturePtr)NULL;
    }

    if (!modelInfo->modelEntryForm) {
        return (TexturePtr)NULL;
    }

    return ModelEntryForm_getMouthTexture(modelInfo->modelEntryForm, i);
}

FlexSkeletonHeader *ModelInfo_getSkeleton(ModelInfo *modelInfo) {
    return modelInfo->modelEntryForm ? modelInfo->modelEntryForm->skel : NULL;
}

FlexSkeletonHeader *ModelInfo_getShieldingSkeleton(ModelInfo *modelInfo) {
    return modelInfo->modelEntryForm ? modelInfo->modelEntryForm->shieldingSkel : NULL;
}

bool ModelInfo_setGfxOverride(ModelInfo *modelInfo, Link_DisplayList id, Gfx *dl) {
    return recomputil_u32_value_hashmap_insert(modelInfo->gfxOverrides, id, (uintptr_t)dl);
}

bool ModelInfo_unsetGfxOverride(ModelInfo *modelInfo, Link_DisplayList id) {
    return recomputil_u32_value_hashmap_erase(modelInfo->gfxOverrides, id);
}

bool ModelInfo_setMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id, Mtx *mtx) {
    return recomputil_u32_value_hashmap_insert(modelInfo->mtxOverrides, id, (uintptr_t)mtx);
}

bool ModelInfo_unsetMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id) {
    return recomputil_u32_value_hashmap_erase(modelInfo->mtxOverrides, id);
}

void ModelInfo_clearAllGfxOverrides(ModelInfo *modelInfo) {
    for (Link_DisplayList i = 0; i < LINK_DL_MAX; ++i) {
        ModelInfo_unsetGfxOverride(modelInfo, i);
    }
}

void ModelInfo_clearAllMtxOverrides(ModelInfo *modelInfo) {
    for (Link_EquipmentMatrix i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        ModelInfo_unsetMtxOverride(modelInfo, i);
    }
}

bool ModelInfo_isAnyFlagEnabled(ModelInfo *modelInfo, u64 flags) {
    return modelInfo->modelEntryForm ? (modelInfo->modelEntryForm->modelEntry.flags & flags) : false;
}
