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

#define ARRAY_GROWTH_FACTOR 3 / 2
#define ARRAY_STARTING_SIZE 16

static bool sShouldSkipInterpolation[PLAYER_FORM_MAX];

static MemorySlotmapHandle sHandleToMemoryEntry;

static YAZMTCore_StringU32Dictionary *sInternalNamesToEntries;

typedef struct {
    void **entries;
    size_t count;
    size_t capacity;
} FormModelEntries;

static FormModelEntries sMemoryEntries[PLAYER_FORM_MAX];

static FormModelEntry *sCurrentModelEntries[PLAYER_FORM_MAX];

#define SAVED_INTERNAL_NAME_BUFFER_SIZE (INTERNAL_NAME_MAX_LENGTH + 1)

typedef struct {
    const char *key;
    char internalName[SAVED_INTERNAL_NAME_BUFFER_SIZE];
} SavedModelName;

static SavedModelName sSavedModelNames[PLAYER_FORM_MAX] = {
    {.key = "pmm_saved_fierce_deity_name"},
    {.key = "pmm_saved_goron_name"},
    {.key = "pmm_saved_zora_name"},
    {.key = "pmm_saved_deku_name"},
    {.key = "pmm_saved_human_name"},
};

void applyByInternalName(PlayerTransformation form, const char *name) {
    u32 entryPtr;

    if (YAZMTCore_StringU32Dictionary_get(sInternalNamesToEntries, name, &entryPtr)) {
        CMEM_tryApplyEntry(form, (FormModelEntry *)entryPtr);
    }
}

FormModelEntry *CMEM_getCurrentEntry(PlayerTransformation form) {
    return sCurrentModelEntries[form];
}

void CMEM_setCurrentEntry(PlayerTransformation form, FormModelEntry *e) {
    sCurrentModelEntries[form] = e;
}

void initFormModelEntries(FormModelEntries *cme) {
    cme->capacity = ARRAY_STARTING_SIZE;
    cme->entries = recomp_alloc(sizeof(*cme->entries) * cme->capacity);
    cme->count = 0;
}

void increaseCapacity(FormModelEntries *cme) {
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

void pushEntry(FormModelEntries *cme, void *entry) {
    FormModelEntry *fme = entry;

    if (!YAZMTCore_StringU32Dictionary_contains(sInternalNamesToEntries, fme->internalName)) {
        size_t newCount = cme->count + 1;

        if (newCount > cme->capacity) {
            increaseCapacity(cme);
        }

        cme->entries[cme->count] = entry;
        cme->count++;

        YAZMTCore_StringU32Dictionary_set(sInternalNamesToEntries, fme->internalName, (uintptr_t)entry);
    }
}

void initEntryManager() {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        initFormModelEntries(&sMemoryEntries[i]);
    }
}

void pushMemoryEntry(PlayerTransformation form, FormModelMemoryEntry *entry) {
    pushEntry(&sMemoryEntries[form], entry);
}

RECOMP_DECLARE_EVENT(_internal_onModelApplied(PlayerTransformation form));

void CMEM_reapplyEntry(PlayerTransformation form) {
    FormModelEntry *currEntry = CMEM_getCurrentEntry(form);

    if (currEntry) {
        currEntry->applyToModelInfo(currEntry, &gLinkFormProxies[form].current);
    }
}

bool CMEM_forceApplyEntry(PlayerTransformation form, FormModelEntry *newEntry) {
    Link_FormProxy *proxy = &gLinkFormProxies[form];
    FormModelEntry *currEntry = CMEM_getCurrentEntry(form);

    if (newEntry == NULL) {
        CMEM_removeModel(form);
        return true;
    }

    if (newEntry->applyToModelInfo(newEntry, &proxy->current)) {
        if (currEntry && currEntry->callback) {
            currEntry->callback(currEntry->handle, PMM_EVENT_MODEL_REMOVED, currEntry->callbackData);
        }

        CMEM_setCurrentEntry(form, newEntry);

        _internal_onModelApplied(form);

        if (newEntry->callback) {
            newEntry->callback(newEntry->handle, PMM_EVENT_MODEL_APPLIED, newEntry->callbackData);
        }

        sShouldSkipInterpolation[form] = true;
        return true;
    }

    return false;
}

bool CMEM_tryApplyEntry(PlayerTransformation form, FormModelEntry *newEntry) {
    FormModelEntry *currEntry = CMEM_getCurrentEntry(form);
    if (newEntry != currEntry) {
        return CMEM_forceApplyEntry(form, newEntry);
    }

    return false;
}

FormModelEntry **CMEM_getCombinedEntries(PlayerTransformation form, size_t *count) {
    size_t combinedLength = sMemoryEntries[form].count;

    if (combinedLength == 0) {
        *count = 0;
        return NULL;
    }

    FormModelEntry **combined = recomp_alloc(sizeof(*combined) * combinedLength);

    for (size_t i = 0; i < sMemoryEntries[form].count; ++i) {
        combined[i] = sMemoryEntries[form].entries[i];
    }

    *count = combinedLength;
    return combined;
}

void CMEM_removeModel(PlayerTransformation form) {
    Link_FormProxy *proxy = &gLinkFormProxies[form];

    FormModelEntry *currentEntry = sCurrentModelEntries[form];

    if (currentEntry) {
        if (currentEntry->callback) {
            currentEntry->callback(currentEntry->handle, PMM_EVENT_MODEL_REMOVED, currentEntry->callbackData);
        }

        sCurrentModelEntries[form] = NULL;

        clearLinkModelInfo(&proxy->current);

        _internal_onModelApplied(form);

        sShouldSkipInterpolation[form] = true;
    }
}

PlayerTransformation getFormFromModelType(FormModelType t) {
    if (t >= PMM_MODEL_TYPE_MAX) {
        return PLAYER_FORM_MAX;
    }

    static const PlayerTransformation FORM_MODEL_TO_FORM[PMM_MODEL_TYPE_MAX] = {
        [PMM_MODEL_TYPE_NONE] = PLAYER_FORM_MAX,
        [PMM_MODEL_TYPE_CHILD] = PLAYER_FORM_HUMAN,
        [PMM_MODEL_TYPE_ADULT] = PLAYER_FORM_HUMAN,
        [PMM_MODEL_TYPE_DEKU] = PLAYER_FORM_DEKU,
        [PMM_MODEL_TYPE_GORON] = PLAYER_FORM_GORON,
        [PMM_MODEL_TYPE_ZORA] = PLAYER_FORM_ZORA,
        [PMM_MODEL_TYPE_FIERCE_DEITY] = PLAYER_FORM_FIERCE_DEITY,
    };

    return FORM_MODEL_TO_FORM[t];
}

void *slotmapGet(MemorySlotmapHandle slotmap, collection_key_t k) {
    void *result = NULL;

    recomputil_memory_slotmap_get(slotmap, k, &result);

    return result;
}

PlayerModelManagerHandle CMEM_createMemoryHandle(PlayerTransformation form, char* internalName) {
    PlayerModelManagerHandle handle = recomputil_memory_slotmap_create(sHandleToMemoryEntry);

    FormModelMemoryEntry *entry = slotmapGet(sHandleToMemoryEntry, handle);

    if (entry) {
        FormModelMemoryEntry_init(entry);

        entry->modelEntry.handle = handle;
        entry->modelEntry.internalName = internalName;

        pushMemoryEntry(form, entry);
    }

    return handle;
}

FormModelMemoryEntry *CMEM_getMemoryEntry(PlayerModelManagerHandle h) {
    void *entry = NULL;
    recomputil_memory_slotmap_get(sHandleToMemoryEntry, h, &entry);
    return entry;
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

RECOMP_DECLARE_EVENT(_internal_onSavedModelsApplied());

void loadSavedModels() {
    static char retrievedName[SAVED_INTERNAL_NAME_BUFFER_SIZE];

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
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
    sHandleToMemoryEntry = recomputil_create_memory_slotmap(sizeof(FormModelMemoryEntry));
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
