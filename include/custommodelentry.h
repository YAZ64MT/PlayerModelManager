#ifndef __CUSTOMMODELENTRY_H__
#define __CUSTOMMODELENTRY_H__

#include "global.h"
#include "playermodelmanager.h"
#include "playermodelmanager_api.h"
#include "equipmentoverrides.h"
#include "recompdata.h"
#include "yazmtcorelib_api.h"

// This is the max file name length for most Linux distros and MacOS
#define INTERNAL_NAME_MAX_LENGTH 255

typedef struct ModelEntry {
    PlayerModelManagerModelType type;
    Link_CustomModelCategory category;
    char *displayName;
    char *internalName;
    char *authorName;
    bool (*applyToModelInfo)(const struct ModelEntry *this);
    bool (*removeFromModelInfo)(const struct ModelEntry *this);
    bool (*setDisplayList)(struct ModelEntry *this, Link_DisplayList id, Gfx *dl);
    bool (*setMatrix)(struct ModelEntry *this, Link_EquipmentMatrix id, Mtx *mtx);
    PlayerModelManagerEventHandler* callback;
    void *callbackData;
    PlayerModelManagerHandle handle;
    u64 flags;
    U32ValueHashmapHandle displayListPtrs;
    U32ValueHashmapHandle mtxPtrs;
} ModelEntry;

typedef struct {
    ModelEntry modelEntry;
    FlexSkeletonHeader *skel;
    FlexSkeletonHeader *shieldingSkel;
    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];
} ModelEntryForm;

typedef struct {
    ModelEntry modelEntry;
    Link_EquipmentReplacement equipType;
} ModelEntryEquipment;

typedef struct {
    ModelEntry modelEntry;
    YAZMTCore_IterableU32Set *modelEntries;
} ModelEntryPack;

Gfx *ModelEntry_getDisplayList(const ModelEntry *entry, Link_DisplayList id);

bool ModelEntry_setDisplayList(ModelEntry *entry, Link_DisplayList id, Gfx *dl);

Mtx *ModelEntry_getMatrix(const ModelEntry *entry, Link_EquipmentMatrix id);

bool ModelEntry_setMatrix(ModelEntry *entry, Link_EquipmentMatrix id, Mtx *mtx);

bool ModelEntryForm_init(ModelEntryForm *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName);

TexturePtr ModelEntryForm_getEyesTexture(ModelEntryForm *entry, PlayerEyeIndex i);

TexturePtr ModelEntryForm_getMouthTexture(ModelEntryForm *entry, PlayerMouthIndex i);

bool ModelEntryEquipment_init(ModelEntryEquipment *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName);

bool ModelEntryPack_init(ModelEntryPack *entry, PlayerModelManagerHandle handle, char *internalName);

ModelEntry const *const *ModelEntryPack_getModelEntries(const ModelEntryPack *entry);

size_t ModelEntryPack_getModelEntriesCount(const ModelEntryPack *entry);

#endif
