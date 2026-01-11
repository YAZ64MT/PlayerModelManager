#include "global.h"
#include "modelinfo.h"
#include "modelentry.h"
#include "recompdata.h"
#include "logger.h"
#include "utils.h"

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid pointer.")

static U32HashsetHandle sValidModelInfoPtrs;

static bool isValidModelInfoPtr(ModelInfo *modelInfo) {
    return recomputil_u32_hashset_contains(sValidModelInfoPtrs, (uintptr_t)modelInfo);
}

void ModelInfo_init(ModelInfo *modelInfo) {
    if (isValidModelInfoPtr(modelInfo)) {
        Logger_printWarning("Tried to init an already existing ModelInfo!");
        return;
    }

    recomputil_u32_hashset_insert(sValidModelInfoPtrs, (uintptr_t)modelInfo);

    modelInfo->modelEntryForm = NULL;
    modelInfo->gfxOverrides = recomputil_create_u32_value_hashmap();
    modelInfo->mtxOverrides = recomputil_create_u32_value_hashmap();
}

bool ModelInfo_hasModelEntry(ModelInfo *modelInfo) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return modelInfo->modelEntryForm;
}

Gfx *ModelInfo_getDL(ModelInfo *modelInfo, Link_DisplayList id) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (id >= LINK_DL_MAX || id < 0) {
        Logger_printWarning("Invalid display list ID %d.", id);
    }

    uintptr_t dl = 0;
    recomputil_u32_value_hashmap_get(modelInfo->gfxOverrides, id, &dl);

    if (!dl) {
        if (modelInfo->modelEntryForm) {
            dl = (uintptr_t)ModelEntry_getDisplayList(ModelEntryForm_getModelEntry(modelInfo->modelEntryForm), id);
        }
    }

    return (Gfx *)dl;
}

Mtx *ModelInfo_getMtx(ModelInfo *modelInfo, Link_EquipmentMatrix id) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (id >= LINK_EQUIP_MATRIX_MAX || id < 0) {
        Logger_printWarning("ModelInfo_getMtx called with invalid matrix ID %d.", id);
        return NULL;
    }

    uintptr_t mtx = 0;
    recomputil_u32_value_hashmap_get(modelInfo->mtxOverrides, id, &mtx);

    if (!mtx) {
        if (modelInfo->modelEntryForm) {
            mtx = (uintptr_t)ModelEntry_getMatrix(ModelEntryForm_getModelEntry(modelInfo->modelEntryForm), id);
        }
    }

    return (Mtx *)mtx;
}

void ModelInfo_setModelEntryForm(ModelInfo *modelInfo, ModelEntryForm *modelEntry) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    modelInfo->modelEntryForm = modelEntry;
}

ModelEntryForm *ModelInfo_getModelEntryForm(ModelInfo *modelInfo) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return modelInfo->modelEntryForm;
}

TexturePtr ModelInfo_getEyesTexture(ModelInfo *modelInfo, PlayerEyeIndex i) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return (TexturePtr)NULL;
    }

    if (i >= PLAYER_EYES_MAX || i < 0) {
        Logger_printWarning("ModelInfo_getEyesTexture called with invalid PlayerEyeIndex %d.", i);
        return (TexturePtr)NULL;
    }

    if (!modelInfo->modelEntryForm) {
        return (TexturePtr)NULL;
    }

    return ModelEntryForm_getEyesTexture(modelInfo->modelEntryForm, i);
}

TexturePtr ModelInfo_getMouthTexture(ModelInfo *modelInfo, PlayerMouthIndex i) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return (TexturePtr)NULL;
    }

    if (i >= PLAYER_MOUTH_MAX || i < 0) {
        Logger_printWarning("ModelInfo_getMouthTexture called with invalid PlayerMouthIndex %d.", i);
        return (TexturePtr)NULL;
    }

    if (!modelInfo->modelEntryForm) {
        return (TexturePtr)NULL;
    }

    return ModelEntryForm_getMouthTexture(modelInfo->modelEntryForm, i);
}

FlexSkeletonHeader *ModelInfo_getSkeleton(ModelInfo *modelInfo) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return modelInfo->modelEntryForm ? ModelEntryForm_getSkeleton(modelInfo->modelEntryForm) : NULL;
}

FlexSkeletonHeader *ModelInfo_getShieldingSkeleton(ModelInfo *modelInfo) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return modelInfo->modelEntryForm ? ModelEntryForm_getShieldingSkeleton(modelInfo->modelEntryForm) : NULL;
}

bool ModelInfo_setGfxOverride(ModelInfo *modelInfo, Link_DisplayList id, Gfx *dl) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!dl) {
        return ModelInfo_unsetGfxOverride(modelInfo, id);
    }

    return recomputil_u32_value_hashmap_insert(modelInfo->gfxOverrides, id, (uintptr_t)dl);
}

bool ModelInfo_unsetGfxOverride(ModelInfo *modelInfo, Link_DisplayList id) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return recomputil_u32_value_hashmap_erase(modelInfo->gfxOverrides, id);
}

bool ModelInfo_setMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id, Mtx *mtx) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return recomputil_u32_value_hashmap_insert(modelInfo->mtxOverrides, id, (uintptr_t)mtx);
}

bool ModelInfo_unsetMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return recomputil_u32_value_hashmap_erase(modelInfo->mtxOverrides, id);
}

void ModelInfo_clearAllGfxOverrides(ModelInfo *modelInfo) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    for (Link_DisplayList i = 0; i < LINK_DL_MAX; ++i) {
        ModelInfo_unsetGfxOverride(modelInfo, i);
    }
}

void ModelInfo_clearAllMtxOverrides(ModelInfo *modelInfo) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    for (Link_EquipmentMatrix i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        ModelInfo_unsetMtxOverride(modelInfo, i);
    }
}

bool ModelInfo_isAnyFlagEnabled(ModelInfo *modelInfo, u64 flags) {
    if (!isValidModelInfoPtr(modelInfo)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return modelInfo->modelEntryForm ? (ModelEntry_isAnyFlagEnabled(ModelEntryForm_getModelEntry(modelInfo->modelEntryForm), flags)) : false;
}

RECOMP_CALLBACK(".", _internal_initHashObjects) void initModelInfoObjects(void) {
    sValidModelInfoPtrs = recomputil_create_u32_hashset();
}
