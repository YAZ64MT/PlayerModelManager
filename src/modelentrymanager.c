#include "global.h"
#include "modelentry.h"
#include "utils.h"
#include "recomputils.h"
#include "libc/string.h"
#include "modelentrymanager.h"
#include "recompdata.h"
#include "proxymm_kv_api.h"
#include "playermodelmanager_api.h"
#include "yazmtcorelib_api.h"
#include "formproxy.h"
#include "playerproxy.h"
#include "logger.h"

static U32SlotmapHandle sEntryHandles;

static YAZMTCore_StringU32Dictionary *sInternalNamesToEntries;

static YAZMTCore_DynamicU32Array *sModelEntries[PMM_MODEL_TYPE_MAX];

static const ModelEntry *sCurrentModelEntries[PMM_MODEL_TYPE_MAX];

static U32HashsetHandle sHiddenModelEntries;

#define SAVED_INTERNAL_NAME_BUFFER_SIZE (INTERNAL_NAME_MAX_LENGTH + 1)

typedef struct {
    const char *key;
} SavedModelName;

static SavedModelName sSavedModelNames[PMM_MODEL_TYPE_MAX] = {
    {.key = "pmm_saved_none_name"}, // FAKE!
    {.key = "pmm_saved_child_name"},
    {.key = "pmm_saved_adult_name"},
    {.key = "pmm_saved_deku_name"},
    {.key = "pmm_saved_goron_name"},
    {.key = "pmm_saved_zora_name"},
    {.key = "pmm_saved_fierce_deity_name"},
    {.key = "pmm_saved_sword1"},
    {.key = "pmm_saved_sword2"},
    {.key = "pmm_saved_sword3"},
    {.key = "pmm_saved_sword4"},
    {.key = "pmm_saved_sword5"},
    {.key = "pmm_saved_shield1"},
    {.key = "pmm_saved_shield2"},
    {.key = "pmm_saved_shield3"},
    {.key = "pmm_saved_hookshot"},
    {.key = "pmm_saved_bow"},
    {.key = "pmm_saved_slingshot"},
    {.key = "pmm_saved_bottle"},
    {.key = "pmm_saved_ocarina_fairy"},
    {.key = "pmm_saved_ocarina_time"},
    {.key = "pmm_saved_boomerang"},
    {.key = "pmm_saved_hammer"},
    {.key = "pmm_saved_deku_stick"},
    {.key = "pmm_saved_pipes"},
    {.key = "pmm_saved_drums"},
    {.key = "pmm_saved_guitar"},
    {.key = "pmm_saved_mask_skull"},
    {.key = "pmm_saved_mask_spooky"},
    {.key = "pmm_saved_mask_gerudo"},
    {.key = "pmm_saved_mask_truth"},
    {.key = "pmm_saved_mask_kafeis_mask"},
    {.key = "pmm_saved_mask_all_night"},
    {.key = "pmm_saved_mask_bunny"},
    {.key = "pmm_saved_mask_keaton"},
    {.key = "pmm_saved_mask_garo"},
    {.key = "pmm_saved_mask_romani"},
    {.key = "pmm_saved_mask_circus_leader"},
    {.key = "pmm_saved_mask_couple"},
    {.key = "pmm_saved_mask_postman"},
    {.key = "pmm_saved_mask_great_fairy"},
    {.key = "pmm_saved_mask_gibdo"},
    {.key = "pmm_saved_mask_don_gero"},
    {.key = "pmm_saved_mask_kamaro"},
    {.key = "pmm_saved_mask_captain"},
    {.key = "pmm_saved_mask_stone"},
    {.key = "pmm_saved_mask_bremen"},
    {.key = "pmm_saved_mask_blast"},
    {.key = "pmm_saved_mask_scents"},
    {.key = "pmm_saved_mask_giant"},
    {.key = "pmm_saved_mask_deku"},
    {.key = "pmm_saved_mask_goron"},
    {.key = "pmm_saved_mask_zora"},
    {.key = "pmm_saved_mask_fierce_deity"},
    {.key = "pmm_saved_model_pack"}, // FAKE!
    {.key = "pmm_saved_bomb"},
    {.key = "pmm_saved_bombchu"},
};

// Returns PLAYER_FORM_MAX on invalid type passed in
PlayerTransformation getFormFromModelType(PlayerModelManagerModelType t) {
    switch (t) {
        case PMM_MODEL_TYPE_CHILD:
        case PMM_MODEL_TYPE_ADULT:
            return PLAYER_FORM_HUMAN;
            break;

        case PMM_MODEL_TYPE_DEKU:
            return PLAYER_FORM_DEKU;
            break;

        case PMM_MODEL_TYPE_GORON:
            return PLAYER_FORM_GORON;
            break;

        case PMM_MODEL_TYPE_ZORA:
            return PLAYER_FORM_ZORA;
            break;

        case PMM_MODEL_TYPE_FIERCE_DEITY:
            return PLAYER_FORM_FIERCE_DEITY;
            break;

        default:
            break;
    }

    return PLAYER_FORM_MAX;
}

static void applyByInternalName(PlayerModelManagerModelType modelType, const char *name) {
    u32 entryPtr;

    if (YAZMTCore_StringU32Dictionary_get(sInternalNamesToEntries, name, &entryPtr)) {
        ModelEntryManager_tryApplyEntry(modelType, (ModelEntry *)entryPtr);
    }
}

bool isEquipmentModelType(PlayerModelManagerModelType modelType) {
    return (modelType > PMM_MODEL_TYPE_FIERCE_DEITY && modelType < PMM_MODEL_TYPE_MODEL_PACK) || modelType == PMM_MODEL_TYPE_BOMB || modelType == PMM_MODEL_TYPE_BOMBCHU;
}

bool isFormModelType(PlayerModelManagerModelType modelType) {
    return modelType <= PMM_MODEL_TYPE_FIERCE_DEITY && modelType >= PMM_MODEL_TYPE_CHILD;
}

bool isPackModelType(PlayerModelManagerModelType modelType) {
    return modelType == PMM_MODEL_TYPE_MODEL_PACK;
}

bool isValidModelType(PlayerModelManagerModelType modelType) {
    return modelType >= 0 && modelType < PMM_MODEL_TYPE_MAX;
}

const ModelEntry *ModelEntryManager_getCurrentEntry(PlayerModelManagerModelType modelType) {
    if (!isValidModelType(modelType)) {
        return NULL;
    }

    return sCurrentModelEntries[modelType];
}

void ModelEntryManager_setCurrentEntry(PlayerModelManagerModelType modelType, const ModelEntry *e) {
    // Packs are special in that they are made up of ModelEntries
    if (isValidModelType(modelType) && !isPackModelType(modelType)) {
        sCurrentModelEntries[modelType] = e;
    }
}

static void pushEntry(YAZMTCore_DynamicU32Array *entryArr, ModelEntry *entry) {
    ModelEntry *modelEntry = entry;
    const char *internalName = ModelEntry_getInternalName(entry);

    if (!YAZMTCore_StringU32Dictionary_contains(sInternalNamesToEntries, internalName)) {
        YAZMTCore_DynamicU32Array_push(entryArr, (uintptr_t)entry);
        YAZMTCore_StringU32Dictionary_set(sInternalNamesToEntries, internalName, (uintptr_t)entry);
    }
}

static FormProxy *getLocalFormProxyFromCategory(PlayerModelManagerModelType modelType) {
    FormProxyId fpId;

    switch (modelType) {
        case PMM_MODEL_TYPE_DEKU:
            fpId = FORM_PROXY_ID_DEKU;
            break;

        case PMM_MODEL_TYPE_GORON:
            fpId = FORM_PROXY_ID_GORON;
            break;

        case PMM_MODEL_TYPE_ZORA:
            fpId = FORM_PROXY_ID_ZORA;
            break;

        case PMM_MODEL_TYPE_FIERCE_DEITY:
            fpId = FORM_PROXY_ID_FIERCE_DEITY;
            break;

        default:
            fpId = FORM_PROXY_ID_HUMAN;
            break;
    }

    return PlayerProxy_getFormProxy(gPlayer1Proxy, fpId);
}

void ModelEntryManager_reapplyEntry(PlayerModelManagerModelType modelType) {
    const ModelEntry *currEntry = ModelEntryManager_getCurrentEntry(modelType);

    if (currEntry) {
        ModelEntry_applyToFormProxy(currEntry, getLocalFormProxyFromCategory(modelType));
    }
}

static void reapplyAllEquipmentEntries(void) {
    for (PlayerModelManagerModelType i = 0; i < PMM_MODEL_TYPE_MAX; ++i) {
        if (isEquipmentModelType(i)) {
            const ModelEntry *tmp = ModelEntryManager_getCurrentEntry(i);
            ModelEntryManager_removeModel(i);
            ModelEntryManager_tryApplyEntry(i, tmp);
        }
    }
}

bool ModelEntryManager_forceApplyEntry(PlayerModelManagerModelType modelType, const ModelEntry *newEntry) {
    const ModelEntry *currEntry = ModelEntryManager_getCurrentEntry(modelType);

    if (newEntry == NULL) {
        ModelEntryManager_removeModel(modelType);
        return true;
    }

    if (ModelEntry_applyToFormProxy(newEntry, getLocalFormProxyFromCategory(modelType))) {
        if (currEntry) {
            ModelEntry_doCallback(newEntry, PMM_EVENT_MODEL_REMOVED);
        }

        ModelEntryManager_setCurrentEntry(modelType, newEntry);

        if (newEntry) {
            ModelEntry_doCallback(newEntry, PMM_EVENT_MODEL_APPLIED);
        }

        if (modelType == PMM_MODEL_TYPE_CHILD || modelType == PMM_MODEL_TYPE_ADULT) {
            reapplyAllEquipmentEntries();
        }

        return true;
    }

    return false;
}

bool ModelEntryManager_isEntryHidden(const ModelEntry *ModelEntry) {
    return recomputil_u32_hashset_contains(sHiddenModelEntries, (uintptr_t)ModelEntry);
}

void ModelEntryManager_setEntryHidden(const ModelEntry *modelEntry, bool isHidden) {
    if (isHidden) {
        recomputil_u32_hashset_insert(sHiddenModelEntries, (uintptr_t)modelEntry);
    } else {
        recomputil_u32_hashset_erase(sHiddenModelEntries, (uintptr_t)modelEntry);
    }
}

bool ModelEntryManager_tryApplyEntry(PlayerModelManagerModelType modelType, const ModelEntry *newEntry) {
    const ModelEntry *currEntry = ModelEntryManager_getCurrentEntry(modelType);
    if (newEntry != currEntry) {
        return ModelEntryManager_forceApplyEntry(modelType, newEntry);
    }

    return false;
}

const ModelEntry **ModelEntryManager_getCategoryEntryData(PlayerModelManagerModelType modelType, size_t *count) {
    if (!isValidModelType(modelType)) {
        return NULL;
    }

    size_t combinedLength = YAZMTCore_DynamicU32Array_size(sModelEntries[modelType]);

    if (combinedLength == 0) {
        *count = 0;
        return NULL;
    }

    *count = combinedLength;

    return (const ModelEntry **)(YAZMTCore_DynamicU32Array_data(sModelEntries[modelType]));
}

void ModelEntryManager_removeModel(PlayerModelManagerModelType modelType) {
    if (!isValidModelType(modelType)) {
        Logger_printError("Called with invalid category %d\n", modelType);
        Utils_tryCrashGame();
        return;
    }

    const ModelEntry *entry = ModelEntryManager_getCurrentEntry(modelType);

    if (entry) {
        ModelEntry_doCallback(entry, PMM_EVENT_MODEL_REMOVED);

        ModelEntryManager_setCurrentEntry(modelType, NULL);

        ModelEntry_removeFromFormProxy(entry, getLocalFormProxyFromCategory(modelType));

        if (modelType == PMM_MODEL_TYPE_CHILD) {
            reapplyAllEquipmentEntries();
        }
    }
}

PlayerModelManagerHandle ModelEntryManager_createMemoryHandle(PlayerModelManagerModelType type, const char *internalName) {
    if (YAZMTCore_StringU32Dictionary_contains(sInternalNamesToEntries, internalName)) {
        Logger_printError("Entry with internal name %s already exists!", internalName);
        return 0;
    }

    if (!isValidModelType(type)) {
        return 0;
    }

    bool isFormEntry = isFormModelType(type);
    bool isEquipmentEntry = isEquipmentModelType(type);
    bool isPackEntry = isPackModelType(type);
    ModelEntry *entry = NULL;
    PlayerModelManagerHandle handle = recomputil_u32_slotmap_create(sEntryHandles);

    if (isFormEntry) {
        ModelEntryForm *formEntry = ModelEntryForm_new(handle, type, internalName);
        entry = ModelEntryForm_getModelEntry(formEntry);
    } else if (isEquipmentEntry) {
        ModelEntryEquipment *entryEquipment = ModelEntryEquipment_new(handle, type, internalName);
        entry = ModelEntryEquipment_getModelEntry(entryEquipment);
    } else if (isPackEntry) {
        ModelEntryPack *entryPack = ModelEntryPack_new(handle, internalName);
        entry = ModelEntryPack_getModelEntry(entryPack);
    }

    if (!entry) {
        Logger_printWarning("ModelEntryManager_createMemoryHandle failed to initialize an entry!");
        recomputil_u32_slotmap_erase(sEntryHandles, handle);
        return 0;
    }

    recomputil_u32_slotmap_set(sEntryHandles, handle, (uintptr_t)entry);

    pushEntry(sModelEntries[type], entry);

    // In MM, adult models are classified as "child" models
    // This won't be done in OoT
    if (type == PMM_MODEL_TYPE_ADULT) {
        YAZMTCore_DynamicU32Array_push(sModelEntries[PMM_MODEL_TYPE_CHILD], (uintptr_t)entry);
    }

    return handle;
}

ModelEntry *ModelEntryManager_getEntry(PlayerModelManagerHandle h) {
    uintptr_t entry = 0;

    if (recomputil_u32_slotmap_contains(sEntryHandles, h)) {
        recomputil_u32_slotmap_get(sEntryHandles, h, &entry);
    }

    return (ModelEntry *)entry;
}

ModelEntry **ModelEntryManager_getEntries(PlayerModelManagerModelType modelType, size_t *count) {
    if (isValidModelType(modelType)) {
        *count = YAZMTCore_DynamicU32Array_size(sModelEntries[modelType]);
        return (ModelEntry **)(YAZMTCore_DynamicU32Array_data(sModelEntries[modelType]));
    }

    return NULL;
}

void ModelEntryManager_saveCurrentEntry(PlayerModelManagerModelType modelType) {
    if (!isValidModelType(modelType)) {
        return;
    }

    const char *key = sSavedModelNames[modelType].key;

    KV_Global_Remove(key);

    const ModelEntry *curr = ModelEntryManager_getCurrentEntry(modelType);
    if (curr) {
        // No need to initialize this since there will be a null terminator
        char tmpNameBuf[SAVED_INTERNAL_NAME_BUFFER_SIZE];

        strcpy(tmpNameBuf, ModelEntry_getInternalName(curr));
        KV_Global_Set(key, tmpNameBuf, INTERNAL_NAME_MAX_LENGTH);
    }
}

void ModelEntryManager_applyModelsFromDisk(void) {
    static char retrievedName[SAVED_INTERNAL_NAME_BUFFER_SIZE];

    for (PlayerModelManagerModelType i = 0; i < PMM_MODEL_TYPE_MAX; ++i) {
        if (KV_Global_Get(sSavedModelNames[i].key, retrievedName, INTERNAL_NAME_MAX_LENGTH)) {
            applyByInternalName(i, retrievedName);
        } else {
            ModelEntryManager_forceApplyEntry(i, NULL);
        }
    }
}

RECOMP_CALLBACK(".", _internal_initHashObjects) void initCMEMHash(void) {
    sEntryHandles = recomputil_create_u32_slotmap();
    sInternalNamesToEntries = YAZMTCore_StringU32Dictionary_new();
    sHiddenModelEntries = recomputil_create_u32_hashset();
    for (int i = 0; i < ARRAY_COUNT(sModelEntries); ++i) {
        sModelEntries[i] = YAZMTCore_DynamicU32Array_new();
    }
}
