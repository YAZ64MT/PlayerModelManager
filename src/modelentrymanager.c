#include "global.h"
#include "modelentry.h"
#include "utils.h"
#include "recomputils.h"
#include "libc/string.h"
#include "modelentrymanager.h"
#include "recompdata.h"
#include "recompconfig.h"
#include "playermodelmanager_api.h"
#include "yazmtcorelib_api.h"
#include "playerproxy.h"
#include "logger.h"
#include "repy_api.h"
#include "string.h"

static YAZMTCore_DynamicDataArray *sProxiesToSave;

typedef struct SavedProxyEntry {
    PlayerProxy *pp;
    const char *sectionName;
} SavedProxyEntry;

static U32SlotmapHandle sEntryHandles;

static YAZMTCore_StringU32Dictionary *sInternalNamesToEntries;

static YAZMTCore_DynamicU32Array *sModelEntries[PMM_MODEL_TYPE_MAX];

static U32HashsetHandle sHiddenModelEntries;

static char *sModDataFolderPath;

typedef struct SavedModelName {
    char *key;
    PlayerModelManagerModelType modelType;
} SavedModelName;

#define DECLARE_SAVED_MODEL_NAME(keyName, type) {.key = keyName, .modelType = type}

static SavedModelName sSavedModelNames[] = {
    DECLARE_SAVED_MODEL_NAME("child", PMM_MODEL_TYPE_CHILD),
    DECLARE_SAVED_MODEL_NAME("adult", PMM_MODEL_TYPE_ADULT),
    DECLARE_SAVED_MODEL_NAME("deku", PMM_MODEL_TYPE_DEKU),
    DECLARE_SAVED_MODEL_NAME("goron", PMM_MODEL_TYPE_GORON),
    DECLARE_SAVED_MODEL_NAME("zora", PMM_MODEL_TYPE_ZORA),
    DECLARE_SAVED_MODEL_NAME("fierce_deity", PMM_MODEL_TYPE_FIERCE_DEITY),
    DECLARE_SAVED_MODEL_NAME("sword1", PMM_MODEL_TYPE_SWORD1),
    DECLARE_SAVED_MODEL_NAME("sword2", PMM_MODEL_TYPE_SWORD2),
    DECLARE_SAVED_MODEL_NAME("sword3", PMM_MODEL_TYPE_SWORD3),
    DECLARE_SAVED_MODEL_NAME("sword4", PMM_MODEL_TYPE_SWORD4),
    DECLARE_SAVED_MODEL_NAME("sword5", PMM_MODEL_TYPE_SWORD5),
    DECLARE_SAVED_MODEL_NAME("shield1", PMM_MODEL_TYPE_SHIELD1),
    DECLARE_SAVED_MODEL_NAME("shield2", PMM_MODEL_TYPE_SHIELD2),
    DECLARE_SAVED_MODEL_NAME("shield3", PMM_MODEL_TYPE_SHIELD3),
    DECLARE_SAVED_MODEL_NAME("hookshot", PMM_MODEL_TYPE_HOOKSHOT),
    DECLARE_SAVED_MODEL_NAME("bow", PMM_MODEL_TYPE_BOW),
    DECLARE_SAVED_MODEL_NAME("slingshot", PMM_MODEL_TYPE_SLINGSHOT),
    DECLARE_SAVED_MODEL_NAME("bottle", PMM_MODEL_TYPE_BOTTLE),
    DECLARE_SAVED_MODEL_NAME("ocarina_fairy", PMM_MODEL_TYPE_OCARINA_FAIRY),
    DECLARE_SAVED_MODEL_NAME("ocarina_time", PMM_MODEL_TYPE_OCARINA_TIME),
    DECLARE_SAVED_MODEL_NAME("boomerang", PMM_MODEL_TYPE_BOOMERANG),
    DECLARE_SAVED_MODEL_NAME("hammer", PMM_MODEL_TYPE_HAMMER),
    DECLARE_SAVED_MODEL_NAME("stick", PMM_MODEL_TYPE_DEKU_STICK),
    DECLARE_SAVED_MODEL_NAME("pipes", PMM_MODEL_TYPE_PIPES),
    DECLARE_SAVED_MODEL_NAME("drums", PMM_MODEL_TYPE_DRUMS),
    DECLARE_SAVED_MODEL_NAME("guitar", PMM_MODEL_TYPE_GUITAR),
    DECLARE_SAVED_MODEL_NAME("mask_skull", PMM_MODEL_TYPE_MASK_SKULL),
    DECLARE_SAVED_MODEL_NAME("mask_spooky", PMM_MODEL_TYPE_MASK_SPOOKY),
    DECLARE_SAVED_MODEL_NAME("mask_gerudo", PMM_MODEL_TYPE_MASK_GERUDO),
    DECLARE_SAVED_MODEL_NAME("mask_truth", PMM_MODEL_TYPE_MASK_TRUTH),
    DECLARE_SAVED_MODEL_NAME("mask_kafei", PMM_MODEL_TYPE_MASK_KAFEIS_MASK),
    DECLARE_SAVED_MODEL_NAME("mask_all_night", PMM_MODEL_TYPE_MASK_ALL_NIGHT),
    DECLARE_SAVED_MODEL_NAME("mask_bunny", PMM_MODEL_TYPE_MASK_BUNNY),
    DECLARE_SAVED_MODEL_NAME("mask_keaton", PMM_MODEL_TYPE_MASK_KEATON),
    DECLARE_SAVED_MODEL_NAME("mask_garo", PMM_MODEL_TYPE_MASK_GARO),
    DECLARE_SAVED_MODEL_NAME("mask_romani", PMM_MODEL_TYPE_MASK_ROMANI),
    DECLARE_SAVED_MODEL_NAME("mask_circus_leader", PMM_MODEL_TYPE_MASK_CIRCUS_LEADER),
    DECLARE_SAVED_MODEL_NAME("mask_couple", PMM_MODEL_TYPE_MASK_COUPLE),
    DECLARE_SAVED_MODEL_NAME("mask_postman", PMM_MODEL_TYPE_MASK_POSTMAN),
    DECLARE_SAVED_MODEL_NAME("mask_great_fairy", PMM_MODEL_TYPE_MASK_GREAT_FAIRY),
    DECLARE_SAVED_MODEL_NAME("mask_gibdo", PMM_MODEL_TYPE_MASK_GIBDO),
    DECLARE_SAVED_MODEL_NAME("mask_don_gero", PMM_MODEL_TYPE_MASK_DON_GERO),
    DECLARE_SAVED_MODEL_NAME("mask_kamaro", PMM_MODEL_TYPE_MASK_KAMARO),
    DECLARE_SAVED_MODEL_NAME("mask_captain", PMM_MODEL_TYPE_MASK_CAPTAIN),
    DECLARE_SAVED_MODEL_NAME("mask_stone", PMM_MODEL_TYPE_MASK_STONE),
    DECLARE_SAVED_MODEL_NAME("mask_bremen", PMM_MODEL_TYPE_MASK_BREMEN),
    DECLARE_SAVED_MODEL_NAME("mask_blast", PMM_MODEL_TYPE_MASK_BLAST),
    DECLARE_SAVED_MODEL_NAME("mask_scents", PMM_MODEL_TYPE_MASK_SCENTS),
    DECLARE_SAVED_MODEL_NAME("mask_giant", PMM_MODEL_TYPE_MASK_GIANT),
    DECLARE_SAVED_MODEL_NAME("mask_deku", PMM_MODEL_TYPE_MASK_DEKU),
    DECLARE_SAVED_MODEL_NAME("mask_goron", PMM_MODEL_TYPE_MASK_GORON),
    DECLARE_SAVED_MODEL_NAME("mask_zora", PMM_MODEL_TYPE_MASK_ZORA),
    DECLARE_SAVED_MODEL_NAME("mask_fierce_deity", PMM_MODEL_TYPE_MASK_FIERCE_DEITY),
    DECLARE_SAVED_MODEL_NAME("bomb", PMM_MODEL_TYPE_BOMB),
    DECLARE_SAVED_MODEL_NAME("bombchu", PMM_MODEL_TYPE_BOMBCHU),
};

#undef DECLARE_SAVED_MODEL_NAME

static bool applyByInternalName(PlayerProxy *pp, PlayerModelManagerModelType modelType, const char *name) {
    u32 entryPtr;

    if (YAZMTCore_StringU32Dictionary_get(sInternalNamesToEntries, name, &entryPtr)) {
        return PlayerProxy_tryApplyEntry(pp, modelType, (ModelEntry *)entryPtr);
    }

    return false;
}

static void pushEntry(YAZMTCore_DynamicU32Array *entryArr, ModelEntry *entry) {
    ModelEntry *modelEntry = entry;
    const char *internalName = ModelEntry_getInternalName(entry);

    if (!YAZMTCore_StringU32Dictionary_contains(sInternalNamesToEntries, internalName)) {
        YAZMTCore_DynamicU32Array_push(entryArr, (uintptr_t)entry);
        YAZMTCore_StringU32Dictionary_set(sInternalNamesToEntries, internalName, (uintptr_t)entry);
    }
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

const ModelEntry **ModelEntryManager_getCategoryEntryData(PlayerModelManagerModelType modelType, size_t *count) {
    if (!Utils_isValidModelType(modelType)) {
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

PlayerModelManagerHandle ModelEntryManager_createMemoryHandle(PlayerModelManagerModelType type, const char *internalName) {
    if (YAZMTCore_StringU32Dictionary_contains(sInternalNamesToEntries, internalName)) {
        Logger_printError("Entry with internal name %s already exists!", internalName);
        return 0;
    }

    if (!Utils_isValidModelType(type)) {
        return 0;
    }

    bool isFormEntry = Utils_isFormModelType(type);
    bool isEquipmentEntry = Utils_isEquipmentModelType(type);
    bool isPackEntry = Utils_isPackModelType(type);
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
    if (Utils_isValidModelType(modelType)) {
        *count = YAZMTCore_DynamicU32Array_size(sModelEntries[modelType]);
        return (ModelEntry **)(YAZMTCore_DynamicU32Array_data(sModelEntries[modelType]));
    }

    return NULL;
}

bool ModelEntryManager_saveModelsToDisk(void) {
    if (!sModDataFolderPath) {
        Logger_printError("sModDataFolderPath has not been set!");
        return false;
    }

    if (!sProxiesToSave) {
        Logger_printError("sProxiesToSave has not been set!");
        return false;
    }

    bool ret = false;

    REPY_FN_SETUP;
    REPY_FN_IMPORT("configparser");
    REPY_FN_IMPORT("pathlib");

    size_t numProxies = YAZMTCore_DynamicDataArray_size(sProxiesToSave);
    SavedProxyEntry *savedProxies = YAZMTCore_DynamicDataArray_data(sProxiesToSave);

    REPY_Handle savedProxiesDict = REPY_CreateDict(0);
    REPY_FN_DEFER_RELEASE(savedProxiesDict);

    for (size_t i = 0; i < numProxies; ++i) {
        PlayerProxy *pp = savedProxies[i].pp;
        char *sectionName = (char *)(savedProxies[i].sectionName);

        REPY_Handle d = REPY_CreateDict(0);
        REPY_FN_DEFER_RELEASE(d);

        REPY_DictSetCStr(savedProxiesDict, sectionName, d);

        for (int i = 0; i < ARRAY_COUNT(sSavedModelNames); ++i) {
            SavedModelName *curr = &sSavedModelNames[i];

            const ModelEntry *entry = PlayerProxy_getCurrentEntry(pp, curr->modelType);

            if (entry) {
                REPY_DictSetCStr(d, curr->key, REPY_CreateStr_SUH(ModelEntry_getInternalName(entry)));
            } else {
                REPY_DictSetCStr(d, curr->key, REPY_CreateStr_SUH(""));
            }
        }
    }

    REPY_FN_SET_STR("mod_folder_path", sModDataFolderPath);
    REPY_FN_SET("saved_proxies", savedProxiesDict);

    REPY_FN_EXEC_CACHE(
        saveModelsToDiskExec,
        "cfg_path = pathlib.Path(mod_folder_path) / 'models.ini'\n"
        "config = configparser.ConfigParser()\n"
        "for section_name,models in saved_proxies.items():\n"
        "   config[section_name] = {}\n"
        "   for key,value in models.items():\n"
        "      config[section_name][key] = value\n"
        "with open(cfg_path, 'w') as configfile:\n"
        "   config.write(configfile)\n"
        "\n");

    ret = saveModelsToDiskExec_success;

    REPY_FN_CLEANUP;

    return ret;
}

void ModelEntryManager_applySavedEntriesToProxy(PlayerProxy *pp, const char *id) {
    if (!id) {
        Logger_printError("id was NULL!");
        return;
    }

    if (!pp) {
        Logger_printError("pp was NULL!");
        return;
    }

    if (!sModDataFolderPath) {
        Logger_printError("sModDataFolderPath is not set!");
        return;
    }

    REPY_FN_SETUP;
    REPY_FN_IMPORT("configparser");
    REPY_FN_IMPORT("pathlib");

    REPY_Handle entriesDict = REPY_CreateDict(0);
    REPY_FN_DEFER_RELEASE(entriesDict);

    REPY_FN_SET("saved_entries", entriesDict);

    REPY_FN_SET_STR("section_name", id);
    REPY_FN_SET_STR("mod_data_folder", sModDataFolderPath);

    REPY_FN_EXEC_CACHE(
        readModelsExec1,
        "config = configparser.ConfigParser()\n"
        "try:\n"
        "    cfg_path = pathlib.Path(mod_data_folder) / 'models.ini'\n"
        "    config.read(cfg_path)\n"
        "    if config.has_section(section_name):\n"
        "         for key,val in config[section_name].items():\n"
        "             saved_entries[key] = str(val)\n"
        "except IOError:\n"
        "   pass\n"
        "\n");

    for (int i = 0; i < ARRAY_COUNT(sSavedModelNames); ++i) {
        SavedModelName *curr = &sSavedModelNames[i];
        REPY_FN_SET_STR("key", curr->key);
        REPY_FN_EXEC_CACHE(
            readModelsExec2,
            "internal_name = ''\n"
            "is_model_found = False\n"
            "if key in saved_entries and saved_entries[key]:\n"
            "   internal_name = saved_entries[key]\n"
            "   is_model_found = True\n"
            "\n");

        bool isModelFound = REPY_FN_GET_BOOL("is_model_found");

        if (isModelFound) {
            char *modelInternalName = REPY_FN_GET_STR("internal_name");

            if (modelInternalName) {
                if (!applyByInternalName(pp, curr->modelType, modelInternalName)) {
                    PlayerProxy_forceApplyEntry(pp, curr->modelType, NULL);
                }

                recomp_free(modelInternalName);
            } else {
                PlayerProxy_forceApplyEntry(pp, curr->modelType, NULL);
            }
        }
    }

    REPY_FN_CLEANUP;
}

RECOMP_CALLBACK(".", _internal_initHashObjects) void initCMEMHash(void) {
    sEntryHandles = recomputil_create_u32_slotmap();
    sInternalNamesToEntries = YAZMTCore_StringU32Dictionary_new();
    sHiddenModelEntries = recomputil_create_u32_hashset();
    for (int i = 0; i < ARRAY_COUNT(sModelEntries); ++i) {
        sModelEntries[i] = YAZMTCore_DynamicU32Array_new();
    }

    char *modFolderPath = (char *)recomp_get_mod_folder_path();

    if (modFolderPath) {
        REPY_FN_SETUP;
        REPY_FN_SET_STR("mods_folder_str", modFolderPath);
        REPY_FN_IMPORT("pathlib");

        REPY_FN_EXEC_CSTR(
            "dir_exists = False\n"
            "mod_folder_path = pathlib.Path(mods_folder_str)\n"
            "mod_data_folder_path = mod_folder_path.parents[0] / 'mod_data' / 'yazmt_z64_playermodelmanager'\n"
            "mod_data_folder_path.mkdir(parents=True, exist_ok=True)\n"
            "dir_exists = mod_data_folder_path.is_dir()\n"
            "data_folder_str = (mod_data_folder_path).as_posix()\n"
            "\n");

        if (REPY_FN_GET_BOOL("dir_exists")) {
            sModDataFolderPath = REPY_FN_GET_STR("data_folder_str");
        } else {
            Logger_printError("Could not create mod data folder!");
        }

        REPY_FN_CLEANUP;
        recomp_free(modFolderPath);
    } else {
        Logger_printError("recomp_get_mod_folder_path returned NULL! Cannot read saved models!");
    }

    sProxiesToSave = YAZMTCore_DynamicDataArray_new(sizeof(SavedProxyEntry));
}

void ModelEntryManager_registerProxyToSave(PlayerProxy *pp, const char *sectionName) {
    if (pp && sectionName) {
        SavedProxyEntry *spe = YAZMTCore_DynamicDataArray_createElement(sProxiesToSave);
        if (spe) {
            spe->pp = pp;
            spe->sectionName = YAZMTCore_Utils_StrDup(sectionName);
        }
    }
}
