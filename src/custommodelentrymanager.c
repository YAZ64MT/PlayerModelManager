#include "global.h"
#include "custommodelentry.h"
#include "playermodelmanager_utils.h"
#include "recomputils.h"
#include "model_common.h"
#include "libc/string.h"
#include "custommodelentrymanager.h"
#include "recompdata.h"
#include "proxymm_kv_api.h"
#include "playermodelmanager_api.h"
#include "yazmtcorelib_api.h"
#include "z64recomp_interpolation.h"

static bool sShouldSkipInterpolation[PLAYER_FORM_MAX];

static U32HashsetHandle sValidModelEntries;

static YAZMTCore_StringU32Dictionary *sInternalNamesToEntries;

static YAZMTCore_DynamicU32Array *sModelEntries[LINK_CMC_MAX];

static ModelEntry *sCurrentModelEntries[LINK_CMC_MAX];

#define SAVED_INTERNAL_NAME_BUFFER_SIZE (INTERNAL_NAME_MAX_LENGTH + 1)

typedef struct {
    const char *key;
    char internalName[SAVED_INTERNAL_NAME_BUFFER_SIZE];
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
};

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
    return cat > LINK_CMC_FIERCE_DEITY && cat < LINK_CMC_MAX;
}

bool isFormCategory(Link_CustomModelCategory cat) {
    return cat <= LINK_CMC_FIERCE_DEITY && cat >= LINK_CMC_HUMAN;
}

bool isValidCategory(Link_CustomModelCategory cat) {
    return cat >= 0 && cat < LINK_CMC_MAX;
}

ModelEntry *CMEM_getCurrentEntry(Link_CustomModelCategory cat) {
    return sCurrentModelEntries[cat];
}

void CMEM_setCurrentEntry(Link_CustomModelCategory cat, ModelEntry *e) {
    sCurrentModelEntries[cat] = e;
}

static void pushEntry(YAZMTCore_DynamicU32Array *entryArr, void *entry) {
    ModelEntry *fme = entry;

    if (!YAZMTCore_StringU32Dictionary_contains(sInternalNamesToEntries, fme->internalName)) {
        YAZMTCore_DynamicU32Array_push(entryArr, (uintptr_t)entry);

        YAZMTCore_StringU32Dictionary_set(sInternalNamesToEntries, fme->internalName, (uintptr_t)entry);
    }
}

void initEntryManager() {
    for (int i = 0; i < ARRAY_COUNT(sModelEntries); ++i) {
        sModelEntries[i] = YAZMTCore_DynamicU32Array_new();
    }
}

static void pushMemoryEntry(Link_CustomModelCategory cat, ModelEntry *entry) {
    pushEntry(sModelEntries[cat], entry);
}

RECOMP_DECLARE_EVENT(_internal_onFormModelApplied(PlayerTransformation form));
RECOMP_DECLARE_EVENT(_internal_onEquipmentModelApplied(Link_EquipmentReplacement eq));

void CMEM_reapplyEntry(Link_CustomModelCategory cat) {
    ModelEntry *currEntry = CMEM_getCurrentEntry(cat);

    if (currEntry) {
        if (isFormCategory(cat)) {
            PlayerTransformation form = getFormFromCategory(cat);
            currEntry->applyToModelInfo(currEntry, &gLinkFormProxies[form].current);
            _internal_onFormModelApplied(form);
        } else if (isEquipmentCategory(cat)) {
            for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
                currEntry->applyToModelInfo(currEntry, &gLinkFormProxies[i].current);
            }

            // TODO: Reimplement in a less fragile way
            _internal_onEquipmentModelApplied(((ModelEntryEquipment *)currEntry)->equipType);
        }
    }
}

bool CMEM_forceApplyEntry(Link_CustomModelCategory cat, ModelEntry *newEntry) {

    if (!isValidCategory(cat)) {
        recomp_printf("PlayerModelManager: Called CMEM_forceApplyEntry with invalid category %d\n", cat);
        return false;
    }

    ModelEntry *currEntry = CMEM_getCurrentEntry(cat);

    if (newEntry == NULL) {
        CMEM_removeModel(cat);
        return true;
    }

    if (isFormCategory(cat)) {
        PlayerTransformation form = getFormFromCategory(cat);

        Link_FormProxy *proxy = &gLinkFormProxies[form];

        if (newEntry->applyToModelInfo(newEntry, &proxy->current)) {
            if (currEntry && currEntry->callback) {
                currEntry->callback(currEntry->handle, PMM_EVENT_MODEL_REMOVED, currEntry->callbackData);
            }

            CMEM_setCurrentEntry(cat, newEntry);

            _internal_onFormModelApplied(form);

            if (newEntry->callback) {
                newEntry->callback(newEntry->handle, PMM_EVENT_MODEL_APPLIED, newEntry->callbackData);
            }

            sShouldSkipInterpolation[form] = true;

            return true;
        }
    } else if (isEquipmentCategory(cat)) {
        if (currEntry && currEntry->callback) {
            currEntry->callback(currEntry->handle, PMM_EVENT_MODEL_REMOVED, currEntry->callbackData);
        }

        CMEM_setCurrentEntry(cat, newEntry);

        for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
            newEntry->applyToModelInfo(newEntry, &gLinkFormProxies[i].current);
        }

        if (newEntry->callback) {
            newEntry->callback(newEntry->handle, PMM_EVENT_MODEL_APPLIED, newEntry->callbackData);
        }

        _internal_onEquipmentModelApplied(((ModelEntryEquipment *)(newEntry))->equipType);

        for (int i = 0; i < ARRAY_COUNT(sShouldSkipInterpolation); ++i) {
            sShouldSkipInterpolation[i] = true;
        }

        return true;
    }

    return false;
}

bool CMEM_tryApplyEntry(Link_CustomModelCategory cat, ModelEntry *newEntry) {
    ModelEntry *currEntry = CMEM_getCurrentEntry(cat);
    if (newEntry != currEntry) {
        return CMEM_forceApplyEntry(cat, newEntry);
    }

    return false;
}

ModelEntry **CMEM_getCategoryEntryData(Link_CustomModelCategory cat, size_t *count) {
    if (!isValidCategory(cat)) {
        return NULL;
    }

    size_t combinedLength = YAZMTCore_DynamicU32Array_size(sModelEntries[cat]);

    if (combinedLength == 0) {
        *count = 0;
        return NULL;
    }

    *count = combinedLength;

    return (ModelEntry **)(YAZMTCore_DynamicU32Array_data(sModelEntries[cat]));
}

void CMEM_removeModel(Link_CustomModelCategory cat) {

    if (!isValidCategory(cat)) {
        recomp_printf("PlayerModelManager: Called CMEM_removeModel with invalid category %d\n", cat);
        return;
    }

    ModelEntry *entryx = sCurrentModelEntries[cat];

    if (entryx) {
        if (entryx->callback) {
            entryx->callback(entryx->handle, PMM_EVENT_MODEL_REMOVED, entryx->callbackData);
        }

        CMEM_setCurrentEntry(cat, NULL);

        if (isFormCategory(cat)) {
            PlayerTransformation form = getFormFromCategory(cat);

            Link_FormProxy *proxy = &gLinkFormProxies[form];

            clearLinkModelInfo(&proxy->current.modelInfo);

            _internal_onFormModelApplied(form);

            sShouldSkipInterpolation[form] = true;

        } else if (isEquipmentCategory(cat)) {
            ModelEntryEquipment *entry = (ModelEntryEquipment *)entryx;

            const EquipmentOverride *override = &gEquipmentOverrideTable[entry->equipType];

            for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
                Link_FormProxy *fp = &gLinkFormProxies[i];

                for (size_t j = 0; j < override->dl.count; ++j) {
                    recomputil_u32_value_hashmap_erase(fp->current.gfxOverrides, override->dl.overrides[j]);
                }

                for (size_t j = 0; j < override->mtx.count; ++j) {
                    recomputil_u32_value_hashmap_erase(fp->current.mtxOverrides, override->mtx.overrides[j]);
                }
            }

            _internal_onEquipmentModelApplied(entry->equipType);

            for (int i = 0; i < ARRAY_COUNT(sShouldSkipInterpolation); ++i) {
                sShouldSkipInterpolation[i] = true;
            }
        }
    }
}

PlayerModelManagerHandle CMEM_createMemoryHandle(PlayerModelManagerModelType type, char *internalName) {
    Link_CustomModelCategory cat = getCategoryFromModelType(type);

    if (!isValidCategory(cat)) {
        return 0;
    }

    size_t handleSize;
    bool isFormEntry = isFormCategory(cat);
    bool isEquipmentEntry = isEquipmentCategory(cat);

    if (isFormEntry) {
        handleSize = sizeof(ModelEntryForm);
    } else if (isEquipmentEntry) {
        handleSize = sizeof(ModelEntryEquipment);

        if (getEquipmentReplacementFromCategory(cat) >= LINK_DL_REPLACE_MAX) {
            return 0;
        }
    } else {
        return 0;
    }

    ModelEntry *entry = recomp_alloc(handleSize);

    if (!entry) {
        return 0;
    }

    PlayerModelManagerHandle handle = (PlayerModelManagerHandle)entry;

    if (isFormEntry) {
        ModelEntryForm_init((ModelEntryForm *)entry);
    } else if (isEquipmentEntry) {
        ModelEntryEquipment_init((ModelEntryEquipment *)entry, getEquipmentReplacementFromCategory(cat));
    }

    entry->internalName = internalName;
    entry->handle = handle;
    entry->type = type;

    recomputil_u32_hashset_insert(sValidModelEntries, handle);

    pushMemoryEntry(cat, entry);

    return handle;
}

ModelEntry *CMEM_getEntry(PlayerModelManagerHandle h) {
    if (recomputil_u32_hashset_contains(sValidModelEntries, h)) {
        return (ModelEntry *)h;
    }

    return NULL;
}

ModelEntry **CMEM_getEntries(Link_CustomModelCategory cat, size_t *count) {
    if (isValidCategory(cat)) {
        *count = YAZMTCore_DynamicU32Array_size(sModelEntries[cat]);
        return (ModelEntry **)(YAZMTCore_DynamicU32Array_data(sModelEntries[cat]));
    }

    return NULL;
}

void CMEM_saveCurrentEntry(Link_CustomModelCategory cat) {
    if (cat >= LINK_CMC_MAX || cat < 0) {
        return;
    }

    const char *key = sSavedModelNames[cat].key;

    KV_Global_Remove(key);

    if (sCurrentModelEntries[cat]) {
        strcpy(sSavedModelNames[cat].internalName, sCurrentModelEntries[cat]->internalName);
        KV_Global_Set(key, sSavedModelNames[cat].internalName, INTERNAL_NAME_MAX_LENGTH);
    }
}

RECOMP_DECLARE_EVENT(_internal_onSavedModelsApplied());

void loadSavedModels() {
    static char retrievedName[SAVED_INTERNAL_NAME_BUFFER_SIZE];

    for (int i = 0; i < LINK_CMC_MAX; ++i) {
        if (KV_Global_Has(sSavedModelNames[i].key)) {
            if (KV_Global_Get(sSavedModelNames[i].key, retrievedName, INTERNAL_NAME_MAX_LENGTH)) {
                applyByInternalName(i, retrievedName);
            }
        } else {
            CMEM_forceApplyEntry(i, NULL);
        }
    }

    _internal_onSavedModelsApplied();
}

RECOMP_DECLARE_EVENT(_internal_onReadyCMEM());

RECOMP_CALLBACK(".", _internal_onReadyUI)
void initEntryManagerCallback() {
    initEntryManager();
    _internal_onReadyCMEM();
}

RECOMP_HOOK_RETURN("TitleSetup_SetupTitleScreen")
void applySavedModelOnTitleScreen() {
    loadSavedModels();
}

RECOMP_CALLBACK(".", _internal_initHashObjects)
void initCMEMHash() {
    sValidModelEntries = recomputil_create_u32_hashset();
    sInternalNamesToEntries = YAZMTCore_StringU32Dictionary_new();
}

RECOMP_CALLBACK("*", recomp_on_play_main)
void skipInterpolationOnPlay(PlayState *play) {
    Player *player = GET_PLAYER(play);

    while (player) {
        if (sShouldSkipInterpolation[player->transformation] && player->actor.scale.y >= 0.0f) {
            actor_set_interpolation_skipped(&player->actor);
        }

        player = (Player *)player->actor.next;
    }

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        sShouldSkipInterpolation[i] = false;
    }
}
