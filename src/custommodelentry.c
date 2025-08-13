#include "global.h"
#include "custommodelentry.h"
#include "recomputils.h"
#include "playermodelmanager_utils.h"
#include "custommodelentrymanager.h"
#include "equipmentoverrides.h"

void ModelEntry_init(ModelEntry *entry) {
    entry->displayName = NULL;
    entry->internalName = NULL;
    entry->authorName = NULL;
    entry->flags = 0;
    entry->callback = NULL;
    entry->callbackData = NULL;
    entry->handle = 0;
    entry->applyToModelInfo = NULL;
    entry->displayListPtrs = recomputil_create_u32_value_hashmap();
    entry->mtxPtrs = recomputil_create_u32_value_hashmap();
    entry->setDisplayList = ModelEntry_setDisplayList;
    entry->setMatrix = ModelEntry_setMatrix;
}

Gfx *ModelEntry_getDisplayList(const ModelEntry *entry, Link_DisplayList id) {
    uintptr_t ret = 0;
    recomputil_u32_value_hashmap_get(entry->displayListPtrs, id, &ret);
    return (Gfx *)ret;
}

bool ModelEntry_setDisplayList(ModelEntry *this, Link_DisplayList id, Gfx *dl) {
    if (id >= LINK_DL_MAX || id < 0) {
        return false;
    }

    recomputil_u32_value_hashmap_insert(this->displayListPtrs, id, (uintptr_t)dl);
    return true;
}

Mtx *ModelEntry_getMatrix(const ModelEntry *entry, Link_EquipmentMatrix id) {
    uintptr_t ret = 0;
    recomputil_u32_value_hashmap_get(entry->mtxPtrs, id, &ret);
    return (Mtx *)ret;
}

bool ModelEntry_setMatrix(ModelEntry *this, Link_EquipmentMatrix id, Mtx *mtx) {
    if (id >= LINK_EQUIP_MATRIX_MAX || id < 0) {
        return false;
    }

    recomputil_u32_value_hashmap_insert(this->mtxPtrs, id, (uintptr_t)mtx);
    return true;
}

bool ModelEntryForm_applyToModelInfo(ModelEntry *thisx, Link_ModelInfoCustom *modelInfoCustom) {
    Link_ModelInfo *modelInfo = &modelInfoCustom->modelInfo;

    clearLinkModelInfo(modelInfo);

    ModelEntryForm *this = (ModelEntryForm *)((void *)thisx);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        modelInfo->models[i] = ModelEntry_getDisplayList(&this->modelEntry, i);
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        modelInfo->equipMtx[i] = ModelEntry_getMatrix(&this->modelEntry, i);
    }

    modelInfo->flags = this->modelEntry.flags;

    modelInfo->skeleton = this->skel;

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        if (this->eyesTex[i]) {
            modelInfo->mouthTextures[i] = this->mouthTex[i];
        }
    }

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        if (this->eyesTex[i]) {
            modelInfo->eyesTextures[i] = this->eyesTex[i];
        }
    }

    modelInfo->shieldingSkeleton = this->shieldingSkel;

    return true;
}

void ModelEntryForm_init(ModelEntryForm *this) {
    ModelEntry_init(&this->modelEntry);

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        if (ModelEntry_getMatrix(&this->modelEntry, i)) {
            ModelEntry_setMatrix(&this->modelEntry, i, NULL);
        }
    }

    this->modelEntry.applyToModelInfo = ModelEntryForm_applyToModelInfo;

    this->skel = NULL;

    Lib_MemSet(this->mouthTex, 0, sizeof(this->mouthTex));

    Lib_MemSet(this->eyesTex, 0, sizeof(this->eyesTex));
}

bool ModelEntryEquipment_setDisplayList(ModelEntry *thisx, Link_DisplayList id, Gfx *dl) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)((void *)thisx);

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    // Count generally is very small (count < 10), so a simple linear search will do
    for (size_t i = 0; i < override->dl.count; ++i) {
        if (id == override->dl.overrides[i]) {
            return ModelEntry_setDisplayList(thisx, id, dl);
        }
    }

    return false;
}

bool ModelEntryEquipment_setMatrix(ModelEntry *thisx, Link_EquipmentMatrix id, Mtx *mtx) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)((void *)thisx);

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    // Count generally is very small (count < 10), so a simple linear search will do
    for (size_t i = 0; i < override->mtx.count; ++i) {
        if (id == override->mtx.overrides[i]) {
            return ModelEntry_setMatrix(thisx, id, mtx);
        }
    }

    return false;
}

bool ModelEntryEquipment_applyToModelInfo(ModelEntry *thisx, Link_ModelInfoCustom *modelInfoCustom) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)((void *)thisx);

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    for (size_t i = 0; i < override->dl.count; ++i) {
        Link_DisplayList id = override->dl.overrides[i];
        uintptr_t dl = 0;
        recomputil_u32_value_hashmap_get(this->modelEntry.displayListPtrs, id, &dl);
        recomputil_u32_value_hashmap_insert(modelInfoCustom->gfxOverrides, id, dl);
        
    }

    for (size_t i = 0; i < override->mtx.count; ++i) {
        Link_EquipmentMatrix id = override->mtx.overrides[i];
        uintptr_t dl = 0;
        recomputil_u32_value_hashmap_get(this->modelEntry.mtxPtrs, id, &dl);
        recomputil_u32_value_hashmap_insert(modelInfoCustom->mtxOverrides, id, dl);
    }

    return true;
}

void ModelEntryEquipment_init(ModelEntryEquipment *entry, Link_EquipmentReplacement type) {
    ModelEntry_init(&entry->modelEntry);

    entry->equipType = type;
    entry->modelEntry.setDisplayList = ModelEntryEquipment_setDisplayList;
    entry->modelEntry.setMatrix = ModelEntryEquipment_setMatrix;
}
