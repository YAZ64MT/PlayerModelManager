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

static YAZMTCore_DynamicU32Array *sModelEntries[LINK_CMC_MAX];

static const ModelEntry *sCurrentModelEntries[LINK_CMC_MAX];

static U32HashsetHandle sHiddenModelEntries;

#define SAVED_INTERNAL_NAME_BUFFER_SIZE (INTERNAL_NAME_MAX_LENGTH + 1)

typedef struct {
    const char *key;
} SavedModelName;

static SavedModelName sSavedModelNames[LINK_CMC_MAX] = {
    {.key = "pmm_saved_child_name"},
    {.key = "pmm_saved_adult_name"},
    {.key = "pmm_saved_human_name"},
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

Link_CustomModelCategory getCategoryFromModelType(PlayerModelManagerModelType t) {
    switch (t) {
        case PMM_MODEL_TYPE_CHILD:
        case PMM_MODEL_TYPE_ADULT:
            return LINK_CMC_HUMAN;
            break;

        case PMM_MODEL_TYPE_DEKU:
            return LINK_CMC_DEKU;
            break;

        case PMM_MODEL_TYPE_GORON:
            return LINK_CMC_GORON;
            break;

        case PMM_MODEL_TYPE_ZORA:
            return LINK_CMC_ZORA;
            break;

        case PMM_MODEL_TYPE_FIERCE_DEITY:
            return LINK_CMC_FIERCE_DEITY;
            break;

        case PMM_MODEL_TYPE_SWORD1:
            return LINK_CMC_SWORD1;
            break;

        case PMM_MODEL_TYPE_SWORD2:
            return LINK_CMC_SWORD2;
            break;

        case PMM_MODEL_TYPE_SWORD3:
            return LINK_CMC_SWORD3;
            break;

        case PMM_MODEL_TYPE_SWORD4:
            return LINK_CMC_SWORD4;
            break;

        case PMM_MODEL_TYPE_SWORD5:
            return LINK_CMC_SWORD5;
            break;

        case PMM_MODEL_TYPE_SHIELD1:
            return LINK_CMC_SHIELD1;
            break;

        case PMM_MODEL_TYPE_SHIELD2:
            return LINK_CMC_SHIELD2;
            break;

        case PMM_MODEL_TYPE_SHIELD3:
            return LINK_CMC_SHIELD3;
            break;

        case PMM_MODEL_TYPE_HOOKSHOT:
            return LINK_CMC_HOOKSHOT;
            break;

        case PMM_MODEL_TYPE_BOW:
            return LINK_CMC_BOW;
            break;

        case PMM_MODEL_TYPE_SLINGSHOT:
            return LINK_CMC_SLINGSHOT;
            break;

        case PMM_MODEL_TYPE_BOTTLE:
            return LINK_CMC_BOTTLE;
            break;

        case PMM_MODEL_TYPE_OCARINA_FAIRY:
            return LINK_CMC_OCARINA_FAIRY;
            break;

        case PMM_MODEL_TYPE_OCARINA_TIME:
            return LINK_CMC_OCARINA_TIME;
            break;

        case PMM_MODEL_TYPE_BOOMERANG:
            return LINK_CMC_BOOMERANG;
            break;

        case PMM_MODEL_TYPE_HAMMER:
            return LINK_CMC_HAMMER;
            break;

        case PMM_MODEL_TYPE_DEKU_STICK:
            return LINK_CMC_DEKU_STICK;
            break;

        case PMM_MODEL_TYPE_PIPES:
            return LINK_CMC_PIPES;
            break;

        case PMM_MODEL_TYPE_DRUMS:
            return LINK_CMC_DRUMS;
            break;

        case PMM_MODEL_TYPE_GUITAR:
            return LINK_CMC_GUITAR;
            break;

        case PMM_MODEL_TYPE_MASK_SKULL:
            return LINK_CMC_MASK_SKULL;
            break;

        case PMM_MODEL_TYPE_MASK_SPOOKY:
            return LINK_CMC_MASK_SPOOKY;
            break;

        case PMM_MODEL_TYPE_MASK_GERUDO:
            return LINK_CMC_MASK_GERUDO;
            break;

        case PMM_MODEL_TYPE_MASK_TRUTH:
            return LINK_CMC_MASK_TRUTH;
            break;

        case PMM_MODEL_TYPE_MASK_KAFEIS_MASK:
            return LINK_CMC_MASK_KAFEIS_MASK;
            break;

        case PMM_MODEL_TYPE_MASK_ALL_NIGHT:
            return LINK_CMC_MASK_ALL_NIGHT;
            break;

        case PMM_MODEL_TYPE_MASK_BUNNY:
            return LINK_CMC_MASK_BUNNY;
            break;

        case PMM_MODEL_TYPE_MASK_KEATON:
            return LINK_CMC_MASK_KEATON;
            break;

        case PMM_MODEL_TYPE_MASK_GARO:
            return LINK_CMC_MASK_GARO;
            break;

        case PMM_MODEL_TYPE_MASK_ROMANI:
            return LINK_CMC_MASK_ROMANI;
            break;

        case PMM_MODEL_TYPE_MASK_CIRCUS_LEADER:
            return LINK_CMC_MASK_CIRCUS_LEADER;
            break;

        case PMM_MODEL_TYPE_MASK_COUPLE:
            return LINK_CMC_MASK_COUPLE;
            break;

        case PMM_MODEL_TYPE_MASK_POSTMAN:
            return LINK_CMC_MASK_POSTMAN;
            break;

        case PMM_MODEL_TYPE_MASK_GREAT_FAIRY:
            return LINK_CMC_MASK_GREAT_FAIRY;
            break;

        case PMM_MODEL_TYPE_MASK_GIBDO:
            return LINK_CMC_MASK_GIBDO;
            break;

        case PMM_MODEL_TYPE_MASK_DON_GERO:
            return LINK_CMC_MASK_DON_GERO;
            break;

        case PMM_MODEL_TYPE_MASK_KAMARO:
            return LINK_CMC_MASK_KAMARO;
            break;

        case PMM_MODEL_TYPE_MASK_CAPTAIN:
            return LINK_CMC_MASK_CAPTAIN;
            break;

        case PMM_MODEL_TYPE_MASK_STONE:
            return LINK_CMC_MASK_STONE;
            break;

        case PMM_MODEL_TYPE_MASK_BREMEN:
            return LINK_CMC_MASK_BREMEN;
            break;

        case PMM_MODEL_TYPE_MASK_BLAST:
            return LINK_CMC_MASK_BLAST;
            break;

        case PMM_MODEL_TYPE_MASK_SCENTS:
            return LINK_CMC_MASK_SCENTS;
            break;

        case PMM_MODEL_TYPE_MASK_GIANT:
            return LINK_CMC_MASK_GIANT;
            break;

        case PMM_MODEL_TYPE_MASK_DEKU:
            return LINK_CMC_MASK_DEKU;
            break;

        case PMM_MODEL_TYPE_MASK_GORON:
            return LINK_CMC_MASK_GORON;
            break;

        case PMM_MODEL_TYPE_MASK_ZORA:
            return LINK_CMC_MASK_ZORA;
            break;

        case PMM_MODEL_TYPE_MASK_FIERCE_DEITY:
            return LINK_CMC_MASK_FIERCE_DEITY;
            break;

        case PMM_MODEL_TYPE_MODEL_PACK:
            return LINK_CMC_MODEL_PACK;
            break;

        case PMM_MODEL_TYPE_BOMB:
            return LINK_CMC_BOMB;
            break;

        case PMM_MODEL_TYPE_BOMBCHU:
            return LINK_CMC_BOMBCHU;
            break;

        default:
            break;
    }

    return LINK_CMC_MAX;
}

PlayerTransformation getFormFromCategory(Link_CustomModelCategory cat) {
    switch (cat) {
        case LINK_CMC_HUMAN:
            return PLAYER_FORM_HUMAN;
            break;

        case LINK_CMC_DEKU:
            return PLAYER_FORM_DEKU;
            break;

        case LINK_CMC_GORON:
            return PLAYER_FORM_GORON;
            break;

        case LINK_CMC_ZORA:
            return PLAYER_FORM_ZORA;
            break;

        case LINK_CMC_FIERCE_DEITY:
            return PLAYER_FORM_FIERCE_DEITY;
            break;

        default:
            break;
    }

    return PLAYER_FORM_MAX;
}

Link_EquipmentReplacement getEquipmentReplacementFromCategory(Link_CustomModelCategory cat) {
    switch (cat) {
        case LINK_CMC_SWORD1:
            return LINK_DL_REPLACE_SWORD1;
            break;

        case LINK_CMC_SWORD2:
            return LINK_DL_REPLACE_SWORD2;
            break;

        case LINK_CMC_SWORD3:
            return LINK_DL_REPLACE_SWORD3;
            break;

        case LINK_CMC_SWORD4:
            return LINK_DL_REPLACE_SWORD4;
            break;

        case LINK_CMC_SWORD5:
            return LINK_DL_REPLACE_SWORD5;
            break;

        case LINK_CMC_SHIELD1:
            return LINK_DL_REPLACE_SHIELD1;
            break;

        case LINK_CMC_SHIELD2:
            return LINK_DL_REPLACE_SHIELD2;
            break;

        case LINK_CMC_SHIELD3:
            return LINK_DL_REPLACE_SHIELD3;
            break;

        case LINK_CMC_HOOKSHOT:
            return LINK_DL_REPLACE_HOOKSHOT;
            break;

        case LINK_CMC_BOW:
            return LINK_DL_REPLACE_BOW;
            break;

        case LINK_CMC_SLINGSHOT:
            return LINK_DL_REPLACE_SLINGSHOT;
            break;

        case LINK_CMC_BOTTLE:
            return LINK_DL_REPLACE_BOTTLE;
            break;

        case LINK_CMC_OCARINA_FAIRY:
            return LINK_DL_REPLACE_OCARINA_FAIRY;
            break;

        case LINK_CMC_OCARINA_TIME:
            return LINK_DL_REPLACE_OCARINA_TIME;
            break;

        case LINK_CMC_BOOMERANG:
            return LINK_DL_REPLACE_BOOMERANG;
            break;

        case LINK_CMC_HAMMER:
            return LINK_DL_REPLACE_HAMMER;
            break;

        case LINK_CMC_DEKU_STICK:
            return LINK_DL_REPLACE_DEKU_STICK;
            break;

        case LINK_CMC_PIPES:
            return LINK_DL_REPLACE_PIPES;
            break;

        case LINK_CMC_DRUMS:
            return LINK_DL_REPLACE_DRUMS;
            break;

        case LINK_CMC_GUITAR:
            return LINK_DL_REPLACE_GUITAR;
            break;

        case LINK_CMC_MASK_SKULL:
            return LINK_DL_REPLACE_MASK_SKULL;
            break;

        case LINK_CMC_MASK_SPOOKY:
            return LINK_DL_REPLACE_MASK_SPOOKY;
            break;

        case LINK_CMC_MASK_GERUDO:
            return LINK_DL_REPLACE_MASK_GERUDO;
            break;

        case LINK_CMC_MASK_TRUTH:
            return LINK_DL_REPLACE_MASK_TRUTH;
            break;

        case LINK_CMC_MASK_KAFEIS_MASK:
            return LINK_DL_REPLACE_MASK_KAFEIS_MASK;
            break;

        case LINK_CMC_MASK_ALL_NIGHT:
            return LINK_DL_REPLACE_MASK_ALL_NIGHT;
            break;

        case LINK_CMC_MASK_BUNNY:
            return LINK_DL_REPLACE_MASK_BUNNY;
            break;

        case LINK_CMC_MASK_KEATON:
            return LINK_DL_REPLACE_MASK_KEATON;
            break;

        case LINK_CMC_MASK_GARO:
            return LINK_DL_REPLACE_MASK_GARO;
            break;

        case LINK_CMC_MASK_ROMANI:
            return LINK_DL_REPLACE_MASK_ROMANI;
            break;

        case LINK_CMC_MASK_CIRCUS_LEADER:
            return LINK_DL_REPLACE_MASK_CIRCUS_LEADER;
            break;

        case LINK_CMC_MASK_COUPLE:
            return LINK_DL_REPLACE_MASK_COUPLE;
            break;

        case LINK_CMC_MASK_POSTMAN:
            return LINK_DL_REPLACE_MASK_POSTMAN;
            break;

        case LINK_CMC_MASK_GREAT_FAIRY:
            return LINK_DL_REPLACE_MASK_GREAT_FAIRY;
            break;

        case LINK_CMC_MASK_GIBDO:
            return LINK_DL_REPLACE_MASK_GIBDO;
            break;

        case LINK_CMC_MASK_DON_GERO:
            return LINK_DL_REPLACE_MASK_DON_GERO;
            break;

        case LINK_CMC_MASK_KAMARO:
            return LINK_DL_REPLACE_MASK_KAMARO;
            break;

        case LINK_CMC_MASK_CAPTAIN:
            return LINK_DL_REPLACE_MASK_CAPTAIN;
            break;

        case LINK_CMC_MASK_STONE:
            return LINK_DL_REPLACE_MASK_STONE;
            break;

        case LINK_CMC_MASK_BREMEN:
            return LINK_DL_REPLACE_MASK_BREMEN;
            break;

        case LINK_CMC_MASK_BLAST:
            return LINK_DL_REPLACE_MASK_BLAST;
            break;

        case LINK_CMC_MASK_SCENTS:
            return LINK_DL_REPLACE_MASK_SCENTS;
            break;

        case LINK_CMC_MASK_GIANT:
            return LINK_DL_REPLACE_MASK_GIANT;
            break;

        case LINK_CMC_MASK_DEKU:
            return LINK_DL_REPLACE_MASK_DEKU;
            break;

        case LINK_CMC_MASK_GORON:
            return LINK_DL_REPLACE_MASK_GORON;
            break;

        case LINK_CMC_MASK_ZORA:
            return LINK_DL_REPLACE_MASK_ZORA;
            break;

        case LINK_CMC_MASK_FIERCE_DEITY:
            return LINK_DL_REPLACE_MASK_FIERCE_DEITY;
            break;

        case LINK_CMC_BOMB:
            return LINK_DL_REPLACE_BOMB;
            break;

        case LINK_CMC_BOMBCHU:
            return LINK_DL_REPLACE_BOMBCHU;
            break;

        default:
            break;
    }

    return LINK_DL_REPLACE_MAX;
}

static void applyByInternalName(Link_CustomModelCategory cat, const char *name) {
    u32 entryPtr;

    if (YAZMTCore_StringU32Dictionary_get(sInternalNamesToEntries, name, &entryPtr)) {
        CMEM_tryApplyEntry(cat, (ModelEntry *)entryPtr);
    }
}

bool isEquipmentCategory(Link_CustomModelCategory cat) {
    return (cat > LINK_CMC_FIERCE_DEITY && cat < LINK_CMC_MODEL_PACK) || cat == LINK_CMC_BOMB || cat == LINK_CMC_BOMBCHU;
}

bool isFormCategory(Link_CustomModelCategory cat) {
    return cat <= LINK_CMC_FIERCE_DEITY && cat >= LINK_CMC_HUMAN;
}

bool isPackCategory(Link_CustomModelCategory cat) {
    return cat == LINK_CMC_MODEL_PACK;
}

bool isValidCategory(Link_CustomModelCategory cat) {
    return cat >= 0 && cat < LINK_CMC_MAX;
}

const ModelEntry *CMEM_getCurrentEntry(Link_CustomModelCategory cat) {
    if (!isValidCategory(cat)) {
        return NULL;
    }

    return sCurrentModelEntries[cat];
}

void CMEM_setCurrentEntry(Link_CustomModelCategory cat, const ModelEntry *e) {
    // Packs are special in that they are made up of ModelEntries
    if (!isPackCategory(cat)) {
        sCurrentModelEntries[cat] = e;
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

static FormProxy *getLocalFormProxyFromCategory(Link_CustomModelCategory cat) {
    FormProxyId fpId;

    switch (cat) {
        case LINK_CMC_DEKU:
            fpId = FORM_PROXY_ID_DEKU;
            break;

        case LINK_CMC_GORON:
            fpId = FORM_PROXY_ID_GORON;
            break;

        case LINK_CMC_ZORA:
            fpId = FORM_PROXY_ID_ZORA;
            break;

        case LINK_CMC_FIERCE_DEITY:
            fpId = FORM_PROXY_ID_FIERCE_DEITY;
            break;

        default:
            fpId = FORM_PROXY_ID_HUMAN;
            break;
    }

    return PlayerProxy_getFormProxy(gPlayer1Proxy, fpId);
}

void CMEM_reapplyEntry(Link_CustomModelCategory cat) {
    const ModelEntry *currEntry = CMEM_getCurrentEntry(cat);

    if (currEntry) {
        ModelEntry_applyToFormProxy(currEntry, getLocalFormProxyFromCategory(cat));
    }
}

bool CMEM_forceApplyEntry(Link_CustomModelCategory cat, const ModelEntry *newEntry) {
    const ModelEntry *currEntry = CMEM_getCurrentEntry(cat);

    if (newEntry == NULL) {
        CMEM_removeModel(cat);
        return true;
    }

    if (ModelEntry_applyToFormProxy(newEntry, getLocalFormProxyFromCategory(cat))) {
        if (currEntry) {
            ModelEntry_doCallback(newEntry, PMM_EVENT_MODEL_REMOVED);
        }

        CMEM_setCurrentEntry(cat, newEntry);

        if (newEntry) {
            ModelEntry_doCallback(newEntry, PMM_EVENT_MODEL_APPLIED);
        }

        return true;
    }

    return false;
}

bool CMEM_isEntryHidden(const ModelEntry *ModelEntry) {
    return recomputil_u32_hashset_contains(sHiddenModelEntries, (uintptr_t)ModelEntry);
}

void CMEM_setEntryHidden(const ModelEntry *modelEntry, bool isHidden) {
    if (isHidden) {
        recomputil_u32_hashset_insert(sHiddenModelEntries, (uintptr_t)modelEntry);
    } else {
        recomputil_u32_hashset_erase(sHiddenModelEntries, (uintptr_t)modelEntry);
    }
}

bool CMEM_tryApplyEntry(Link_CustomModelCategory cat, const ModelEntry *newEntry) {
    const ModelEntry *currEntry = CMEM_getCurrentEntry(cat);
    if (newEntry != currEntry) {
        return CMEM_forceApplyEntry(cat, newEntry);
    }

    return false;
}

const ModelEntry **CMEM_getCategoryEntryData(Link_CustomModelCategory cat, size_t *count) {
    if (!isValidCategory(cat)) {
        return NULL;
    }

    size_t combinedLength = YAZMTCore_DynamicU32Array_size(sModelEntries[cat]);

    if (combinedLength == 0) {
        *count = 0;
        return NULL;
    }

    *count = combinedLength;

    return (const ModelEntry **)(YAZMTCore_DynamicU32Array_data(sModelEntries[cat]));
}

void CMEM_removeModel(Link_CustomModelCategory cat) {
    if (!isValidCategory(cat)) {
        Logger_printError("Called CMEM_removeModel with invalid category %d\n", cat);
        Utils_tryCrashGame();
        return;
    }

    const ModelEntry *entry = sCurrentModelEntries[cat];

    if (entry) {

        ModelEntry_doCallback(entry, PMM_EVENT_MODEL_REMOVED);

        CMEM_setCurrentEntry(cat, NULL);

        ModelEntry_removeFromFormProxy(entry, getLocalFormProxyFromCategory(cat));
    }
}

PlayerModelManagerHandle CMEM_createMemoryHandle(PlayerModelManagerModelType type, char *internalName) {
    Link_CustomModelCategory cat = getCategoryFromModelType(type);

    if (!isValidCategory(cat)) {
        return 0;
    }

    bool isFormEntry = isFormCategory(cat);
    bool isEquipmentEntry = isEquipmentCategory(cat);
    bool isPackEntry = isPackCategory(cat);
    ModelEntry *entry = NULL;
    PlayerModelManagerHandle handle = recomputil_u32_slotmap_create(sEntryHandles);

    if (isFormEntry) {
        ModelEntryForm *formEntry = ModelEntryForm_new(handle, type, internalName);
        entry = ModelEntryForm_getModelEntry(formEntry);
    } else if (isEquipmentEntry) {
        if (getEquipmentReplacementFromCategory(cat) < LINK_DL_REPLACE_MAX) {
            ModelEntryEquipment *entryEquipment = ModelEntryEquipment_new(handle, type, internalName);
            entry = ModelEntryEquipment_getModelEntry(entryEquipment);
        }
    } else if (isPackEntry) {
        ModelEntryPack *entryPack = ModelEntryPack_new(handle, internalName);
        entry = ModelEntryPack_getModelEntry(entryPack);
    }

    if (!entry) {
        Logger_printWarning("CMEM_createMemoryHandle failed to initialize an entry!");
        recomputil_u32_slotmap_erase(sEntryHandles, handle);
        return 0;
    }

    recomputil_u32_slotmap_set(sEntryHandles, handle, (uintptr_t)entry);

    pushEntry(sModelEntries[cat], entry);

    return handle;
}

ModelEntry *CMEM_getEntry(PlayerModelManagerHandle h) {
    uintptr_t entry = 0;

    if (recomputil_u32_slotmap_contains(sEntryHandles, h)) {
        recomputil_u32_slotmap_get(sEntryHandles, h, &entry);
    }

    return (ModelEntry *)entry;
}

ModelEntry **CMEM_getEntries(Link_CustomModelCategory cat, size_t *count) {
    if (isValidCategory(cat)) {
        *count = YAZMTCore_DynamicU32Array_size(sModelEntries[cat]);
        return (ModelEntry **)(YAZMTCore_DynamicU32Array_data(sModelEntries[cat]));
    }

    return NULL;
}

void CMEM_saveCurrentEntry(Link_CustomModelCategory cat) {
    if (!isValidCategory(cat)) {
        return;
    }

    const char *key = sSavedModelNames[cat].key;

    KV_Global_Remove(key);

    if (sCurrentModelEntries[cat]) {
        // No need to initialize this since there will be a null terminator
        char tmpNameBuf[SAVED_INTERNAL_NAME_BUFFER_SIZE];

        strcpy(tmpNameBuf, ModelEntry_getInternalName(sCurrentModelEntries[cat]));
        KV_Global_Set(key, tmpNameBuf, INTERNAL_NAME_MAX_LENGTH);
    }
}

RECOMP_DECLARE_EVENT(_internal_onSavedModelsApplied());

void loadSavedModels() {
    static char retrievedName[SAVED_INTERNAL_NAME_BUFFER_SIZE];

    for (int i = 0; i < LINK_CMC_MAX; ++i) {
        if (KV_Global_Get(sSavedModelNames[i].key, retrievedName, INTERNAL_NAME_MAX_LENGTH)) {
            applyByInternalName(i, retrievedName);
        } else {
            CMEM_forceApplyEntry(i, NULL);
        }
    }

    _internal_onSavedModelsApplied();
}

RECOMP_HOOK_RETURN("ConsoleLogo_Destroy")
void applySavedModelsAtStart() {
    loadSavedModels();
}

RECOMP_CALLBACK(".", _internal_initHashObjects)
void initCMEMHash() {
    sEntryHandles = recomputil_create_u32_slotmap();
    sInternalNamesToEntries = YAZMTCore_StringU32Dictionary_new();
    sHiddenModelEntries = recomputil_create_u32_hashset();
    for (int i = 0; i < ARRAY_COUNT(sModelEntries); ++i) {
        sModelEntries[i] = YAZMTCore_DynamicU32Array_new();
    }
}
