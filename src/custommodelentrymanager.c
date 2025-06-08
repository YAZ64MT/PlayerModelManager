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

static CustomModelEntries sDiskEntries;
static CustomModelEntries sMemoryEntries;

static CustomModelEntry *sCurrentModelEntry;

static char *sZobjDir;

CustomModelEntry *CMEM_getCurrentEntry() {
    return sCurrentModelEntry;
}

void CMEM_setCurrentEntry(CustomModelEntry *e) {
    sCurrentModelEntry = e;
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
    initCustomModelEntries(&sDiskEntries);
    initCustomModelEntries(&sMemoryEntries);

    unsigned char *modFolderPath = recomp_get_mod_folder_path();
    sZobjDir = QDFL_getCombinedPath(2, modFolderPath, "zobj");
    recomp_free(modFolderPath);

    sHandleToMemoryEntry = recomputil_create_u32_memory_hashmap(sizeof(CustomModelMemoryEntry));
}

void pushMemoryEntry(CustomModelMemoryEntry *entry) {
    pushEntry(&sMemoryEntries, entry);
}

void pushDiskEntry(CustomModelDiskEntry *entry) {
    pushEntry(&sDiskEntries, entry);
}

void clearDiskEntries() {
    for (size_t i = 0; i < sDiskEntries.count; ++i) {
        CustomModelDiskEntry *curr = sDiskEntries.entries[i];
        if (&curr->modelEntry != sCurrentModelEntry) {
            CustomModelDiskEntry_freeMembers(curr);
            recomp_free(curr);
        } else {
            curr->isOrphaned = true;
        }

        sDiskEntries.entries[i] = NULL;
    }

    sDiskEntries.count = 0;
}

bool CMEM_tryApplyEntry(CustomModelEntry *newEntry, Link_FormProxy *proxy) {
    CustomModelEntry *currEntry = CMEM_getCurrentEntry();
    if (newEntry != currEntry) {
        if (newEntry->applyToModelInfo(newEntry, &proxy->current)) {
            if (newEntry->onModelLoad) {
                newEntry->onModelLoad(newEntry->onModelLoadData);
            }

            if (currEntry && currEntry->onModelUnload) {
                currEntry->onModelUnload(currEntry->onModelUnloadData);
            }

            CMEM_setCurrentEntry(newEntry);

            refreshFormProxy(proxy);
            refreshFaceTextures();

            gIsAgePropertyRefreshRequested = true;
            
            return true;
        }
    }

    return false;
}

CustomModelEntry **CMEM_getCombinedEntries(size_t *count) {
    size_t combinedLength = sDiskEntries.count + sMemoryEntries.count;

    if (combinedLength == 0) {
        return NULL;
    }

    CustomModelEntry **combined = recomp_alloc(sizeof(*combined) * combinedLength);

    for (size_t i = 0; i < sMemoryEntries.count; ++i) {
        combined[i] = sMemoryEntries.entries[i];
    }

    for (size_t i = 0; i < sDiskEntries.count; ++i) {
        combined[i + sMemoryEntries.count] = sDiskEntries.entries[i];
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

char* getBaseNameNoExt(const char *path) {
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
        }
        else if (path[i - 1] == '/' || path[i - 1] == '\\') {
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

void CMEM_refreshDiskEntries() {
    clearDiskEntries();

    unsigned long numFiles;
    QDFL_Status err = QDFL_getNumDirEntries(sZobjDir, &numFiles);

    if (err == QDFL_STATUS_OK) {
        for (size_t i = 0; i < numFiles; ++i) {
            char *path = NULL;

            QDFL_getDirEntryNameByIndex(sZobjDir, i, &path);

            char *fullPath = QDFL_getCombinedPath(2, sZobjDir, path);

            void *data = NULL;

            QDFL_loadFile(fullPath, &data);

            unsigned long fileSize = 0;
            
            QDFL_getFileSize(fullPath, &fileSize);

            if (isValidZobj(data, fileSize)) {
                CustomModelDiskEntry *entry = recomp_alloc(sizeof(*entry));
                CustomModelDiskEntry_init(entry);
                entry->filePath = fullPath;
                entry->modelEntry.internalName = path;
                entry->modelEntry.displayName = getBaseNameNoExt(path);
                pushDiskEntry(entry);
            }
            else {
                recomp_free(path);
                recomp_free(fullPath);
            }

            if (data) {
                recomp_free(data);
            }
        }
    }
}

void CMEM_removeModel(Link_FormProxy *proxy) {
    if (sCurrentModelEntry) {
        sCurrentModelEntry->onModelUnload(sCurrentModelEntry->onModelUnloadData);
        sCurrentModelEntry = NULL;

        clearLinkModelInfo(&proxy->current);

        refreshFormProxy(proxy);
        refreshFaceTextures();

        gIsAgePropertyRefreshRequested = true;
    }
}

ZPlayerModelHandle CMEM_createMemoryHandle() {
    ZPlayerModelHandle handle = sNextMemoryHandle;
    
    sNextMemoryHandle++;

    recomputil_u32_memory_hashmap_create(sHandleToMemoryEntry, handle);

    CustomModelMemoryEntry *entry = recomputil_u32_memory_hashmap_get(sHandleToMemoryEntry, handle);

    CustomModelMemoryEntry_init(entry);

    pushMemoryEntry(entry);

    return handle;
}

CustomModelDiskEntry *CMEM_getMemoryEntry(ZPlayerModelHandle h) {
    return recomputil_u32_memory_hashmap_get(sHandleToMemoryEntry, h);
}

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onReadyCMEM());

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyUI)
void initEntryManagerCallback() {
    initEntryManager();
    PlayerModelManager_internal_onReadyCMEM();
}
