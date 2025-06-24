#include "global.h"
#include "u32valuedictionary.h"
#include "crc32.h"
#include "libc/string.h"
#include "recompdata.h"
#include "recomputils.h"
#include "dynmemarray.h"

typedef struct {
    char *key;
    u32 value;
} DictionaryEntrySlot;

void destroySlotMembers(DictionaryEntrySlot *slot) {
    recomp_free(slot->key);
    slot->key = NULL;
}

U32ValueDictionaryHandle U32ValueDictionary_create() {
    return recomputil_create_u32_memory_hashmap(sizeof(DynamicMemoryArray));
}

bool U32ValueDictionary_set(U32ValueDictionaryHandle dict, const char *key, u32 value) {
    size_t keyLen = strlen(key);

    u32 hash = crc32(key, keyLen);

    DynamicMemoryArray *dynMemArr = NULL;

    if (recomputil_u32_memory_hashmap_create(dict, hash)) {
        dynMemArr = recomputil_u32_memory_hashmap_get(dict, hash);

        // collisions should be rare, so start with 1 slot
        DynMemArr_init(dynMemArr, sizeof(DictionaryEntrySlot), 1);
    }

    if (!dynMemArr) {
        dynMemArr = recomputil_u32_memory_hashmap_get(dict, hash);
    }

    DictionaryEntrySlot *slot = NULL;

    for (size_t i = 0; i < dynMemArr->count && !slot; ++i) {
        DictionaryEntrySlot *candidate = DynMemArr_get(dynMemArr, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            slot = candidate;
        }
    }

    bool wasElementCreated = !!slot;

    if (!slot) {
        slot = DynMemArr_createElement(dynMemArr);
        slot->key = recomp_alloc((keyLen + 1) * sizeof(*key));
        strcpy(slot->key, key);
    }

    if (slot) {
        slot->value = value;
    }

    return wasElementCreated;
}

bool U32ValueDictionary_get(U32ValueDictionaryHandle dict, const char *key, u32 *out) {
    size_t keyLen = strlen(key);

    u32 hash = crc32(key, keyLen);

    DynamicMemoryArray *slots = recomputil_u32_memory_hashmap_get(dict, hash);

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

bool U32ValueDictionary_has(U32ValueDictionaryHandle dict, const char *key) {
    size_t keyLen = strlen(key);

    u32 hash = crc32(key, keyLen);

    DynamicMemoryArray *slots = recomputil_u32_memory_hashmap_get(dict, hash);

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

bool U32ValueDictionary_unset(U32ValueDictionaryHandle dict, const char *key) {
    size_t keyLen = strlen(key);

    u32 hash = crc32(key, keyLen);

    DynamicMemoryArray *slots = recomputil_u32_memory_hashmap_get(dict, hash);

    if (!slots) {
        return false;
    }
    
    for (size_t i = 0; i < slots->count; ++i) {
        DictionaryEntrySlot *candidate = DynMemArr_get(slots, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            DynMemArr_remove(slots, i);
            return true;
        }
    }

    return false;
}
