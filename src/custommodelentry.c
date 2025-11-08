#include "global.h"
#include "custommodelentry.h"
#include "recomputils.h"
#include "playermodelmanager_utils.h"
#include "custommodelentrymanager.h"
#include "equipmentoverrides.h"
#include "model_common.h"
#include "yazmtcorelib_api.h"
#include "custommodelentrymanager.h"
#include "logger.h"

RECOMP_DECLARE_EVENT(_internal_onFormModelApplied(PlayerTransformation form));
RECOMP_DECLARE_EVENT(_internal_onEquipmentModelApplied(Link_EquipmentReplacement eq));

bool ModelEntry_applyToModelInfo(const ModelEntry *this) {
    Logger_printError("PlayerModelManager: Model Entry with internal name %s is using ModelEntry_applyToModelInfo (should have been overriden!)\n", this->internalName ? this->internalName : "[UNKNOWN]");
    tryCrashGame();
    return false;
}

bool ModelEntry_removeFromModelInfo(const ModelEntry *this) {
    Logger_printError("PlayerModelManager: Model Entry with internal name %s is using ModelEntry_removeFromModelInfo (should have been overriden!)\n", this->internalName ? this->internalName : "[UNKNOWN]");
    tryCrashGame();
    return false;
}

bool ModelEntry_init(ModelEntry *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {

    Link_CustomModelCategory cat = getCategoryFromModelType(type);

    if (!isValidCategory(cat)) {
        Logger_printError("PlayerModelmanager: Trying to initialize ModelEntry with invalid PlayerModelManagerModelType %d", type);
        tryCrashGame();
        return false;
    }

    if (!internalName) {
        Logger_printError("PlayerModelmanager: Trying to initialize ModelEntry with type %d with NULL internal name!\n", type);
        tryCrashGame();
        return false;
    }

    if (!handle) {
        Logger_printError("PlayerModelmanager: Trying to initialize ModelEntry with invalid PlayerModelManagerHandle %d", handle);
        tryCrashGame();
        return false;
    }

    entry->type = type;
    entry->category = cat;
    entry->displayName = NULL;
    entry->internalName = internalName;
    entry->authorName = NULL;
    entry->flags = 0;
    entry->callback = NULL;
    entry->callbackData = NULL;
    entry->handle = handle;
    entry->applyToModelInfo = ModelEntry_applyToModelInfo;
    entry->removeFromModelInfo = ModelEntry_removeFromModelInfo;
    entry->displayListPtrs = recomputil_create_u32_value_hashmap();
    entry->mtxPtrs = recomputil_create_u32_value_hashmap();
    entry->setDisplayList = ModelEntry_setDisplayList;
    entry->setMatrix = ModelEntry_setMatrix;

    return true;
}

Gfx *ModelEntry_getDisplayList(const ModelEntry *entry, Link_DisplayList id) {
    uintptr_t ret = 0;
    recomputil_u32_value_hashmap_get(entry->displayListPtrs, id, &ret);
    return (Gfx *)ret;
}

bool ModelEntry_setDisplayList(ModelEntry *entry, Link_DisplayList id, Gfx *dl) {
    if (id >= LINK_DL_MAX || id < 0) {
        return false;
    }

    recomputil_u32_value_hashmap_insert(entry->displayListPtrs, id, (uintptr_t)dl);
    return true;
}

Mtx *ModelEntry_getMatrix(const ModelEntry *entry, Link_EquipmentMatrix id) {
    uintptr_t ret = 0;
    recomputil_u32_value_hashmap_get(entry->mtxPtrs, id, &ret);
    return (Mtx *)ret;
}

bool ModelEntry_setMatrix(ModelEntry *entry, Link_EquipmentMatrix id, Mtx *mtx) {
    if (id >= LINK_EQUIP_MATRIX_MAX || id < 0) {
        return false;
    }

    recomputil_u32_value_hashmap_insert(entry->mtxPtrs, id, (uintptr_t)mtx);
    return true;
}

bool ModelEntryForm_applyToModelInfo(const ModelEntry *thisx) {
    PlayerTransformation form = getFormFromModelType(thisx->type);

    if (form >= PLAYER_FORM_MAX) {
        return false;
    }

    Link_ModelInfo *modelInfo = &gLinkFormProxies[form].current.modelInfo;

    clearLinkModelInfo(modelInfo);

    ModelEntryForm *this = (ModelEntryForm *)(thisx);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        modelInfo->models[i] = ModelEntry_getDisplayList(&this->modelEntry, i);
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        modelInfo->equipMtx[i] = ModelEntry_getMatrix(&this->modelEntry, i);
    }

    modelInfo->flags = this->modelEntry.flags;

    modelInfo->skeleton = this->skel;

    modelInfo->shieldingSkeleton = this->shieldingSkel;

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

    _internal_onFormModelApplied(form);

    return true;
}

bool ModelEntryForm_removeFromModelInfo(const ModelEntry *thisx) {
    PlayerTransformation form = getFormFromModelType(thisx->type);

    Link_FormProxy *proxy = &gLinkFormProxies[form];

    clearLinkModelInfo(&proxy->current.modelInfo);

    _internal_onFormModelApplied(form);

    return true;
}

bool ModelEntryForm_init(ModelEntryForm *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {
    if (!ModelEntry_init(&entry->modelEntry, handle, type, internalName)) {
        return false;
    }

    if (!isFormCategory(entry->modelEntry.category)) {
        Logger_printError("PlayerModelManager: ModelEntryForm with internal name %s had non-form type %d passed in!\n", internalName, type);
        tryCrashGame();
        return false;
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        ModelEntry_setMatrix(&entry->modelEntry, i, NULL);
    }

    entry->modelEntry.applyToModelInfo = ModelEntryForm_applyToModelInfo;
    entry->modelEntry.removeFromModelInfo = ModelEntryForm_removeFromModelInfo;
    entry->skel = NULL;
    entry->shieldingSkel = NULL;
    Lib_MemSet(entry->mouthTex, 0, sizeof(entry->mouthTex));
    Lib_MemSet(entry->eyesTex, 0, sizeof(entry->eyesTex));

    return true;
}

TexturePtr ModelEntryForm_getEyesTexture(ModelEntryForm *entry, PlayerEyeIndex i) {
    return entry->eyesTex[i];
}

TexturePtr ModelEntryForm_getMouthTexture(ModelEntryForm *entry, PlayerMouthIndex i) {
    return entry->mouthTex[i];
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

bool ModelEntryEquipment_applyToModelInfo(const ModelEntry *thisx) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)((void *)thisx);

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_ModelInfoCustom *modelInfoCustom = &gLinkFormProxies[i].current;

        for (size_t i = 0; i < override->dl.count; ++i) {
            Link_DisplayList id = override->dl.overrides[i];
            uintptr_t dl = 0;
            recomputil_u32_value_hashmap_get(this->modelEntry.displayListPtrs, id, &dl);
            recomputil_u32_value_hashmap_insert(modelInfoCustom->gfxOverrides, id, dl);
        }

        for (size_t i = 0; i < override->mtx.count; ++i) {
            Link_EquipmentMatrix id = override->mtx.overrides[i];
            uintptr_t mtx = 0;
            recomputil_u32_value_hashmap_get(this->modelEntry.mtxPtrs, id, &mtx);
            recomputil_u32_value_hashmap_insert(modelInfoCustom->mtxOverrides, id, mtx);
        }
    }

    _internal_onEquipmentModelApplied(this->equipType);

    return true;
}

bool ModelEntryEquipment_removeFromModelInfo(const ModelEntry *thisx) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)thisx;

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_FormProxy *fp = &gLinkFormProxies[i];

        for (size_t j = 0; j < override->dl.count; ++j) {
            recomputil_u32_value_hashmap_erase(fp->current.gfxOverrides, override->dl.overrides[j]);
        }

        for (size_t j = 0; j < override->mtx.count; ++j) {
            recomputil_u32_value_hashmap_erase(fp->current.mtxOverrides, override->mtx.overrides[j]);
        }
    }

    _internal_onEquipmentModelApplied(this->equipType);

    return true;
}

bool ModelEntryEquipment_init(ModelEntryEquipment *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {
    if (!ModelEntry_init(&entry->modelEntry, handle, type, internalName)) {
        return false;
    }

    if (!isEquipmentCategory(entry->modelEntry.category)) {
        Logger_printError("PlayerModelManager: ModelEntryEquipment with internal name %s had non-equipment type %d passed in!\n", internalName, type);
        tryCrashGame();
        return false;
    }

    entry->equipType = getEquipmentReplacementFromCategory(entry->modelEntry.category);
    entry->modelEntry.setDisplayList = ModelEntryEquipment_setDisplayList;
    entry->modelEntry.setMatrix = ModelEntryEquipment_setMatrix;
    entry->modelEntry.applyToModelInfo = ModelEntryEquipment_applyToModelInfo;
    entry->modelEntry.removeFromModelInfo = ModelEntryEquipment_removeFromModelInfo;

    return true;
}

bool ModelEntryPack_setDisplayList(ModelEntry *thisx, Link_DisplayList id, Gfx *dl) {
    return false;
}

bool ModelEntryPack_setMatrix(ModelEntry *thisx, Link_EquipmentMatrix id, Mtx *mtx) {
    return false;
}

bool ModelEntryPack_applyToModelInfo(const ModelEntry *thisx) {
    const ModelEntryPack *this = (const ModelEntryPack *)thisx;

    size_t numEntries = ModelEntryPack_getModelEntriesCount(this);
    ModelEntry const *const *modelEntries = ModelEntryPack_getModelEntries(this);

    for (size_t i = 0; i < numEntries; ++i) {
        CMEM_tryApplyEntry(modelEntries[i]->category, modelEntries[i]);
    }

    return true;
}

bool ModelEntryPack_removeFromModelInfo(const ModelEntry *this) {
    return true;
}

bool ModelEntryPack_init(ModelEntryPack *entry, PlayerModelManagerHandle handle, char *internalName) {
    if (!ModelEntry_init(&entry->modelEntry, handle, PMM_MODEL_TYPE_MODEL_PACK, internalName)) {
        return false;
    }

    entry->modelEntry.setDisplayList = ModelEntryPack_setDisplayList;
    entry->modelEntry.setMatrix = ModelEntryPack_setMatrix;
    entry->modelEntry.applyToModelInfo = ModelEntryPack_applyToModelInfo;
    entry->modelEntry.removeFromModelInfo = ModelEntryPack_removeFromModelInfo;
    entry->modelEntries = YAZMTCore_IterableU32Set_new();

    return true;
}

ModelEntry const *const *ModelEntryPack_getModelEntries(const ModelEntryPack *entry) {
    return (ModelEntry const *const *)YAZMTCore_IterableU32Set_values(entry->modelEntries);
}

size_t ModelEntryPack_getModelEntriesCount(const ModelEntryPack *entry) {
    return YAZMTCore_IterableU32Set_size(entry->modelEntries);
}
