#include "global.h"
#include "custommodelentry.h"
#include "recomputils.h"

#define ARRAY_GROWTH_FACTOR 3 / 2
#define ARRAY_STARTING_SIZE 16

typedef struct {
    void **entries;
    size_t count;
    size_t capacity;
} CustomModelEntries;

static CustomModelEntries sDiskEntries;
static CustomModelEntries sMemoryEntries;

static CustomModelEntry *sCurrentModelEntry;

CustomModelEntry *CMEM_getCurrentEntry() {
    return sCurrentModelEntry;
}

void CMEM_setCurrentEntry(CustomModelEntry *e) {
    sCurrentModelEntry = e;
}

void initCustomModelEntries(CustomModelEntries *cme) {
    cme->capacity = ARRAY_STARTING_SIZE;
    cme->entries = recomp_alloc(sizeof(cme->entries) * cme->capacity);
    cme->count = 0;
}

void increaseCapacity(CustomModelEntries *cme) {
    void **newArray;
    size_t newCapacity = cme->capacity * ARRAY_GROWTH_FACTOR;
    newArray = recomp_alloc(sizeof(newArray) * newCapacity);

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
}

void CMEM_pushMemoryEntry(CustomModelMemoryEntry *entry) {
    pushEntry(&sMemoryEntries, entry);
}

void CMEM_pushDiskEntry(CustomModelDiskEntry *entry) {
    pushEntry(&sMemoryEntries, entry);
}

void CMEM_clearDiskEntries() {
    sDiskEntries.count = 0;
}

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyFormProxies)
void initEntryManagerCallback() {
    initEntryManager();
}
