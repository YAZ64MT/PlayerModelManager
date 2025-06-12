#include "global.h"
#include "custommodelentry.h"
#include "playermodelmanager_utils.h"
#include "recomputils.h"
#include "model_common.h"
#include "defines_z64o.h"
#include "qdfileloader_api.h"
#include "libc/string.h"
#include "custommodelentrymanager.h"
#include "recompdata.h"
#include "defines_mmo.h"
#include "defines_ooto.h"
#include "proxymm_kv_api.h"

#define ARRAY_GROWTH_FACTOR 3 / 2
#define ARRAY_STARTING_SIZE 16

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());

static U32MemoryHashmapHandle sHandleToMemoryEntry;
static ZPlayerModelHandle sNextMemoryHandle = 1;

typedef struct {
    void **entries;
    size_t count;
    size_t capacity;
} CustomModelEntries;

static CustomModelEntries sDiskEntries[PLAYER_FORM_MAX];
static CustomModelEntries sMemoryEntries[PLAYER_FORM_MAX];

static CustomModelEntry *sCurrentModelEntries[PLAYER_FORM_MAX];

static char *sZobjDir;

typedef struct {
    void *buffer;
    size_t capacity;
} DiskEntryBuffer;

#define SAVED_INTERNAL_NAME_BUFFER_SIZE (INTERNAL_NAME_MAX_LENGTH + 1)

typedef struct {
    const char *key;
    char internalName[SAVED_INTERNAL_NAME_BUFFER_SIZE];
} SavedModelName;

static SavedModelName sSavedModelNames[PLAYER_FORM_MAX] = {
    {.key = "zpm_saved_fierce_deity_name"},
    {.key = "zpm_saved_goron_name"},
    {.key = "zpm_saved_zora_name"},
    {.key = "zpm_saved_deku_name"},
    {.key = "zpm_saved_human_name"},
};

static DiskEntryBuffer sDiskBuffers[PLAYER_FORM_MAX] = {0};

void increaseDiskBufferSizeIfNeeded(PlayerTransformation form, size_t minSize) {
    if (minSize > sDiskBuffers[form].capacity) {
        if (sDiskBuffers[form].buffer) {
            recomp_free(sDiskBuffers[form].buffer);
        }

        sDiskBuffers[form].buffer = recomp_alloc(minSize);
        sDiskBuffers[form].capacity = minSize;
    }
}

void applyByInternalName(PlayerTransformation form, const char *name) {
    for (size_t i = 0; i < sMemoryEntries[form].count; ++i) {
        CustomModelMemoryEntry *e = sMemoryEntries[form].entries[i];

        if (strcmp(name, e->modelEntry.internalName) == 0) {
            CMEM_tryApplyEntry(form, &e->modelEntry);
            return;
        }
    }

    for (size_t i = 0; i < sDiskEntries[form].count; ++i) {
        CustomModelDiskEntry *e = sDiskEntries[form].entries[i];

        if (strcmp(name, e->modelEntry.internalName) == 0) {
            CMEM_tryApplyEntry(form, &e->modelEntry);
            return;
        }
    }
}

void *CMEM_loadFromDisk(PlayerTransformation form, const char *path) {
    unsigned long fileSize = 0;

    QDFL_getFileSize(path, &fileSize);

    increaseDiskBufferSizeIfNeeded(form, fileSize);

    QDFL_Status s = QDFL_loadFileIntoBuffer(path, sDiskBuffers[form].buffer, sDiskBuffers[form].capacity);

    if (s != QDFL_STATUS_OK) {
        return NULL;
    }

    return sDiskBuffers[form].buffer;
}

CustomModelEntry *CMEM_getCurrentEntry(PlayerTransformation form) {
    return sCurrentModelEntries[form];
}

void CMEM_setCurrentEntry(PlayerTransformation form, CustomModelEntry *e) {
    sCurrentModelEntries[form] = e;
}

void initCustomModelEntries(CustomModelEntries *cme) {
    cme->capacity = ARRAY_STARTING_SIZE;
    cme->entries = recomp_alloc(sizeof(*cme->entries) * cme->capacity);
    cme->count = 0;
}

void increaseCapacity(CustomModelEntries *cme) {
    void **newArray;
    size_t newCapacity = cme->capacity * ARRAY_GROWTH_FACTOR;
    newArray = recomp_alloc(sizeof(*newArray) * newCapacity);

    for (size_t i = 0; i < cme->count; ++i) {
        newArray[i] = cme->entries[i];
    }

    recomp_free(cme->entries);
    cme->entries = newArray;
    cme->capacity = newCapacity;
}

void pushEntry(CustomModelEntries *cme, void *entry) {
    size_t newCount = cme->count + 1;

    if (newCount > cme->capacity) {
        increaseCapacity(cme);
    }

    cme->entries[cme->count] = entry;
    cme->count++;
}

void initEntryManager() {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        initCustomModelEntries(&sDiskEntries[i]);
        initCustomModelEntries(&sMemoryEntries[i]);
    }

    unsigned char *modFolderPath = recomp_get_mod_folder_path();
    sZobjDir = QDFL_getCombinedPath(2, modFolderPath, "zobj");
    recomp_free(modFolderPath);

    sHandleToMemoryEntry = recomputil_create_u32_memory_hashmap(sizeof(CustomModelMemoryEntry));
}

void pushMemoryEntry(PlayerTransformation form, CustomModelMemoryEntry *entry) {
    pushEntry(&sMemoryEntries[form], entry);
}

void pushDiskEntry(PlayerTransformation form, CustomModelDiskEntry *entry) {
    pushEntry(&sDiskEntries[form], entry);
}

void clearDiskEntries(PlayerTransformation form) {
    for (size_t i = 0; i < sDiskEntries[form].count; ++i) {
        CustomModelDiskEntry *curr = sDiskEntries[form].entries[i];
        CustomModelDiskEntry_freeMembers(curr);
        recomp_free(curr);
        sDiskEntries[form].entries[i] = NULL;
    }

    sDiskEntries[form].count = 0;
}

bool CMEM_tryApplyEntry(PlayerTransformation form, CustomModelEntry *newEntry) {
    Link_FormProxy *proxy = &gLinkFormProxies[form];
    CustomModelEntry *currEntry = CMEM_getCurrentEntry(form);
    if (newEntry != currEntry) {
        if (newEntry == NULL) {
            CMEM_removeModel(form);
            return true;
        }

        if (newEntry->applyToModelInfo(newEntry, &proxy->current)) {
            if (currEntry && currEntry->onModelUnload) {
                currEntry->onModelUnload(currEntry->onModelUnloadData);
            }

            if (newEntry->onModelLoad) {
                newEntry->onModelLoad(newEntry->onModelLoadData);
            }

            CMEM_setCurrentEntry(form, newEntry);

            refreshFormProxy(proxy);
            refreshFaceTextures();

            gIsAgePropertyRefreshRequested = true;

            return true;
        }
    }

    return false;
}

CustomModelEntry **CMEM_getCombinedEntries(PlayerTransformation form, size_t *count) {
    size_t combinedLength = sDiskEntries[form].count + sMemoryEntries[form].count;

    if (combinedLength == 0) {
        return NULL;
    }

    CustomModelEntry **combined = recomp_alloc(sizeof(*combined) * combinedLength);

    for (size_t i = 0; i < sMemoryEntries[form].count; ++i) {
        combined[i] = sMemoryEntries[form].entries[i];
    }

    for (size_t i = 0; i < sDiskEntries[form].count; ++i) {
        combined[i + sMemoryEntries[form].count] = sDiskEntries[form].entries[i];
    }

    *count = combinedLength;
    return combined;
}

bool isValidZobj(void *data, size_t size) {
    // playas zobjs always have an alias table of at least 0x800 bytes at offset 0x5000
    bool isValid = data != NULL && size >= 0x5800;

    if (isValid) {
        const char ML64_HEADER[] = "MODLOADER64";

        isValid = true;

        char *dataBytes = data;

        for (size_t i = 0; isValid && i < ARRAY_COUNT(ML64_HEADER) - 1; ++i) {
            if (dataBytes[i + Z64O_MODLOADER_HEADER] != ML64_HEADER[i]) {
                isValid = false;
            }
        }
    }

    return isValid;
}

char *getBaseNameNoExt(const char *path) {
    if (!path || *path == '\0') {
        return NULL;
    }

    size_t len = strlen(path);

    size_t start = 0;
    size_t end = len;

    for (size_t i = len - 1; i > start; --i) {
        if (path[i] == '.') {
            if (end == len) {
                end = i;
            }
        } else if (path[i - 1] == '/' || path[i - 1] == '\\') {
            start = i;
        }
    }

    size_t baseLen = end - start;

    if (baseLen == 0) {
        return NULL;
    }

    char *base = recomp_alloc((baseLen + 1) * sizeof(char));

    for (size_t i = 0; i < baseLen; ++i) {
        base[i] = path[i + start];
    }

    base[baseLen] = '\0';

    return base;
}

const char DISKENTRY_MODEL_INFO_HEADER[] = "PLAYERMODELINFO";
#define DISKENTRY_INTERNAL_NAME_FIELD_SIZE 64
#define DISKENTRY_DISPLAY_NAME_FIELD_SIZE 32
#define DISKENTRY_AUTHOR_NAME_FIELD_SIZE 64
#define DISKENTRY_MODEL_INFO_LOCATION 0x5500

typedef struct {
    char header[sizeof(DISKENTRY_MODEL_INFO_HEADER) - 1];
    char embedVersion;
    char internalName[DISKENTRY_INTERNAL_NAME_FIELD_SIZE];
    char displayName[DISKENTRY_DISPLAY_NAME_FIELD_SIZE];
    char authorName[DISKENTRY_AUTHOR_NAME_FIELD_SIZE];
} CustomModelDiskEntryEmbeddedInfo;

bool isEmbeddedVersionSupported(CustomModelDiskEntryEmbeddedInfo *info) {
    return info->embedVersion == 1;
}

CustomModelDiskEntryEmbeddedInfo* getEmbeddedInfo(void *zobj) {
    if (zobj == NULL) {
        return NULL;
    }

    char *data = zobj;

    for (size_t i = 0; i < sizeof(DISKENTRY_MODEL_INFO_HEADER) - 1; ++i) {
        if (data[DISKENTRY_MODEL_INFO_LOCATION + i] != DISKENTRY_MODEL_INFO_HEADER[i]) {
            return NULL;
        }
    }

    CustomModelDiskEntryEmbeddedInfo *info = (void *)&data[DISKENTRY_MODEL_INFO_LOCATION];

    if (!isEmbeddedVersionSupported(info)) {
        return NULL;
    }

    return info;
}

char *getTruncatedStringCopy(const char s[], size_t maxLength) {
    size_t length = 0;

    const char *p = s;
    while (*p != '\0' && length < maxLength) {
        length++;
        p++;
    }

    char *sCopy = recomp_alloc(length + 1);

    sCopy[length] = '\0';

    for (size_t i = 0; i < length; ++i) {
        sCopy[i] = s[i];
    }

    return sCopy;
}

void CMEM_refreshDiskEntries(PlayerTransformation form) {
    CustomModelEntry *currentEntry = CMEM_getCurrentEntry(form);

    char *entryName = NULL;

    if (currentEntry) {
        entryName = recomp_alloc(strlen(currentEntry->internalName) + 1);
        strcpy(entryName, currentEntry->internalName);
    }

    CMEM_removeModel(form);

    clearDiskEntries(form);

    unsigned long numFiles;
    QDFL_Status err = QDFL_getNumDirEntries(sZobjDir, &numFiles);

    if (err == QDFL_STATUS_OK) {
        for (size_t i = 0; i < numFiles; ++i) {
            char *path = NULL;
            char *fullPath = NULL;

            bool isValid = QDFL_getDirEntryNameByIndex(sZobjDir, i, &path) == QDFL_STATUS_OK;

            if (isValid) {
                fullPath = QDFL_getCombinedPath(2, sZobjDir, path);

                unsigned long fileSize = 0;

                QDFL_getFileSize(fullPath, &fileSize);

                void *zobj = CMEM_loadFromDisk(form, fullPath);

                isValid = isValidZobj(zobj, fileSize);

                if (isValid) {
                    CustomModelDiskEntry *entry = recomp_alloc(sizeof(*entry));

                    CustomModelType modelType = CUSTOM_MODEL_TYPE_NONE;

                    u8 *data = sDiskBuffers[form].buffer;

                    switch (data[Z64O_FORM_BYTE]) {
                        case MMO_FORM_BYTE_CHILD:
                        case OOTO_FORM_BYTE_CHILD:
                            if (form == PLAYER_FORM_HUMAN) {
                                modelType = CUSTOM_MODEL_TYPE_CHILD;
                            } else {
                                isValid = false;
                            }

                            break;

                        case MMO_FORM_BYTE_ADULT:
                        case OOTO_FORM_BYTE_ADULT:
                            if (form == PLAYER_FORM_HUMAN) {
                                modelType = CUSTOM_MODEL_TYPE_ADULT;
                            } else if (form == PLAYER_FORM_FIERCE_DEITY) {
                                modelType = CUSTOM_MODEL_TYPE_FIERCE_DEITY;
                            } else {
                                isValid = false;
                            }
                            break;

                        default:
                            isValid = false;
                            break;
                    }

                    if (isValid) {
                        CustomModelDiskEntry *entry = recomp_alloc(sizeof(*entry));
                        CustomModelDiskEntry_init(entry, modelType);
                        entry->filePath = fullPath;

                        CustomModelDiskEntryEmbeddedInfo *embed = getEmbeddedInfo(zobj);

                        if (embed) {
                            entry->modelEntry.internalName = getTruncatedStringCopy(embed->internalName, sizeof(embed->internalName) - 1);

                            entry->modelEntry.displayName = getTruncatedStringCopy(embed->displayName, sizeof(embed->displayName) - 1);

                            entry->modelEntry.authorName = getTruncatedStringCopy(embed->authorName, sizeof(embed->authorName) - 1);
                        }
                        else {
                            entry->modelEntry.internalName = getTruncatedStringCopy(path, INTERNAL_NAME_MAX_LENGTH);
                            entry->modelEntry.displayName = getBaseNameNoExt(path);
                        }

                        pushDiskEntry(form, entry);
                    }
                }
            }

            recomp_free(path);
            path = NULL;

            if (!isValid) {
                recomp_free(fullPath);
                fullPath = NULL;
            }
        }
    }

    if (entryName) {
        applyByInternalName(form, entryName);
        recomp_free(entryName);
    }
}

void CMEM_removeModel(PlayerTransformation form) {
    Link_FormProxy *proxy = &gLinkFormProxies[form];

    if (sCurrentModelEntries[form]) {
        sCurrentModelEntries[form]->onModelUnload(sCurrentModelEntries[form]->onModelUnloadData);
        sCurrentModelEntries[form] = NULL;

        clearLinkModelInfo(&proxy->current);

        refreshFormProxy(proxy);
        refreshFaceTextures();

        gIsAgePropertyRefreshRequested = true;
    }
}

ZPlayerModelHandle CMEM_createMemoryHandle(PlayerTransformation form) {
    ZPlayerModelHandle handle = sNextMemoryHandle;

    sNextMemoryHandle++;

    recomputil_u32_memory_hashmap_create(sHandleToMemoryEntry, handle);

    CustomModelMemoryEntry *entry = recomputil_u32_memory_hashmap_get(sHandleToMemoryEntry, handle);

    CustomModelMemoryEntry_init(entry);

    pushMemoryEntry(form, entry);

    return handle;
}

CustomModelMemoryEntry *CMEM_getMemoryEntry(ZPlayerModelHandle h) {
    return recomputil_u32_memory_hashmap_get(sHandleToMemoryEntry, h);
}

void CMEM_saveCurrentEntry(PlayerTransformation form) {
    if (form > PLAYER_FORM_MAX || form < 0) {
        return;
    }

    const char *key = sSavedModelNames[form].key;

    KV_Global_Remove(key);

    if (sCurrentModelEntries[form]) {
        strcpy(sSavedModelNames[form].internalName, sCurrentModelEntries[form]->internalName);
        KV_Global_Set(key, sSavedModelNames[form].internalName, INTERNAL_NAME_MAX_LENGTH);
    }
}

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onSavedModelsApplied());

void loadSavedModels() {
    static char retrievedName[SAVED_INTERNAL_NAME_BUFFER_SIZE];

    if (KV_Global_Has(sSavedModelNames[PLAYER_FORM_HUMAN].key)) {
        if (KV_Global_Get(sSavedModelNames[PLAYER_FORM_HUMAN].key, retrievedName, INTERNAL_NAME_MAX_LENGTH)) {
            applyByInternalName(PLAYER_FORM_HUMAN, retrievedName);
        }
    }

    PlayerModelManager_internal_onSavedModelsApplied();
}

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onReadyCMEM());

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyUI)
void initEntryManagerCallback() {
    initEntryManager();
    PlayerModelManager_internal_onReadyCMEM();
}

RECOMP_HOOK_RETURN("TitleSetup_SetupTitleScreen")
void applySavedModelOnTitleScreen() {
    loadSavedModels();
}
