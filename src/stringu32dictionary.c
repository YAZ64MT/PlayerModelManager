#include "global.h"
#include "stringu32dictionary.h"
#include "libc/string.h"
#include "recompdata.h"
#include "recomputils.h"
#include "dynmemarray.h"
#include "dynu32array.h"
#include "fnv.h"

u32 hasher(const void *data, size_t length) {
    return fnv_32a_buf(data, length, FNV1_32A_INIT);
}

typedef struct {
    char *key;
    u32 value;
} DictionaryEntrySlot;

typedef struct {
    U32MemoryHashmapHandle hashmap;
    DynamicU32Array keyHashes;
} Dictionary;

void destroySlotMembers(DictionaryEntrySlot *slot) {
    recomp_free(slot->key);
    slot->key = NULL;
}

static StringU32DictionaryHandle sNextDictHandle = 1;
static U32MemoryHashmapHandle sDictionaries;

StringU32DictionaryHandle StringU32Dictionary_create() {
    StringU32DictionaryHandle h = sNextDictHandle;
    sNextDictHandle++;

    if (recomputil_u32_memory_hashmap_create(sDictionaries, h)) {
        Dictionary *dict = recomputil_u32_memory_hashmap_get(sDictionaries, h);

        if (dict) {
            dict->hashmap = recomputil_create_u32_memory_hashmap(sizeof(DynamicMemoryArray));
            DynU32Arr_init(&dict->keyHashes);
        } else {
            return 0;
        }
    } else {
        return 0;
    }

    return h;
}

bool isValidDictionary(StringU32DictionaryHandle dictHandle) {
    return recomputil_u32_memory_hashmap_contains(sDictionaries, dictHandle);
}

Dictionary *getDictionary(StringU32DictionaryHandle dictHandle) {
    return recomputil_u32_memory_hashmap_get(sDictionaries, dictHandle);
}

void StringU32Dictionary_destroy(StringU32DictionaryHandle dictHandle) {
    Dictionary *dict = getDictionary(dictHandle);

    if (!dict) {
        return;
    }

    for (size_t i = 0; i < dict->keyHashes.count; ++i) {
        u32 hash = dict->keyHashes.data[i];

        DynamicMemoryArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

        if (slots) {
            for (size_t j = 0; slots->count; ++j) {
                DictionaryEntrySlot *slot = DynMemArr_get(slots, j);

                if (slot) {
                    destroySlotMembers(slot);
                }
            }

            DynMemArr_destroyMembers(slots);
        }
    }

    recomputil_u32_memory_hashmap_erase(sDictionaries, dictHandle);
}

bool StringU32Dictionary_set(StringU32DictionaryHandle dictHandle, const char *key, u32 value) {
    Dictionary *dict = getDictionary(dictHandle);

    if (!dict) {
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicMemoryArray *dynMemArr = NULL;

    if (recomputil_u32_memory_hashmap_create(dict->hashmap, hash)) {
        dynMemArr = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

        // collisions should be rare, so start with 1 slot
        DynMemArr_init(dynMemArr, sizeof(DictionaryEntrySlot), 1);
    }

    if (!dynMemArr) {
        dynMemArr = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);
    }

    DictionaryEntrySlot *slot = NULL;

    for (size_t i = 0; i < dynMemArr->count && !slot; ++i) {
        DictionaryEntrySlot *candidate = DynMemArr_get(dynMemArr, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            slot = candidate;
        }
    }

    bool wasElementCreated = !!slot;

    if (!wasElementCreated) {
        slot = DynMemArr_createElement(dynMemArr);
        slot->key = recomp_alloc((keyLen + 1) * sizeof(*key));
        strcpy(slot->key, key);
    }

    if (slot) {
        slot->value = value;

        DynU32Arr_push(&dict->keyHashes, hash);
    }

    return wasElementCreated;
}

bool StringU32Dictionary_get(StringU32DictionaryHandle dictHandle, const char *key, u32 *out) {
    Dictionary *dict = getDictionary(dictHandle);

    if (!dict) {
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicMemoryArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

    if (!slots) {
        return false;
    }

    for (size_t i = 0; i < slots->count; ++i) {
        DictionaryEntrySlot *candidate = DynMemArr_get(slots, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            *out = candidate->value;
            return true;
        }
    }

    return false;
}

bool StringU32Dictionary_has(StringU32DictionaryHandle dictHandle, const char *key) {
    Dictionary *dict = getDictionary(dictHandle);

    if (!dict) {
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicMemoryArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

    if (!slots) {
        return false;
    }

    for (size_t i = 0; i < slots->count; ++i) {
        DictionaryEntrySlot *candidate = DynMemArr_get(slots, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            return true;
        }
    }

    return false;
}

bool StringU32Dictionary_unset(StringU32DictionaryHandle dictHandle, const char *key) {
    Dictionary *dict = getDictionary(dictHandle);

    if (!dict) {
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicMemoryArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

    if (!slots) {
        return false;
    }

    for (size_t i = 0; i < slots->count; ++i) {
        DictionaryEntrySlot *candidate = DynMemArr_get(slots, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            destroySlotMembers(candidate);
            DynMemArr_remove(slots, i);
            return true;
        }
    }

    return false;
}

RECOMP_CALLBACK(".", _internal_initDictionary)
void initU32StringDictionaryHandleTracker() {
    sDictionaries = recomputil_create_u32_memory_hashmap(sizeof(Dictionary));
}
