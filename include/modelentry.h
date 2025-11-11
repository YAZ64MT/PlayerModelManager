#ifndef __CUSTOMMODELENTRY_H__
#define __CUSTOMMODELENTRY_H__

#include "global.h"
#include "playermodelmanager.h"
#include "playermodelmanager_api.h"
#include "equipmentoverrides.h"
#include "recompdata.h"
#include "yazmtcorelib_api.h"

#define MODELENTRY_FLAG_IS_ADULT 0x1

// This is the max file name length for most Linux distros and MacOS
#define INTERNAL_NAME_MAX_LENGTH 255

typedef struct ModelEntry ModelEntry;
typedef struct ModelEntryForm ModelEntryForm;
typedef struct ModelEntryEquipment ModelEntryEquipment;
typedef struct ModelEntryPack ModelEntryPack;

Gfx *ModelEntry_getDisplayList(const ModelEntry *entry, Link_DisplayList id);

bool ModelEntry_setDisplayList(ModelEntry *entry, Link_DisplayList id, Gfx *dl);

Mtx *ModelEntry_getMatrix(const ModelEntry *entry, Link_EquipmentMatrix id);

bool ModelEntry_setMatrix(ModelEntry *entry, Link_EquipmentMatrix id, Mtx *mtx);

bool ModelEntry_applyToModelInfo(const ModelEntry *entry);

bool ModelEntry_removeFromModelInfo(const ModelEntry *entry);

const char *ModelEntry_getInternalName(const ModelEntry *entry);

const char *ModelEntry_getDisplayName(const ModelEntry *entry);

void ModelEntry_setDisplayName(ModelEntry *entry, const char *name);

const char *ModelEntry_getAuthorName(const ModelEntry *entry);

void ModelEntry_setAuthorName(ModelEntry *entry, const char *name);

Link_CustomModelCategory ModelEntry_getCategory(const ModelEntry *entry);

PlayerModelManagerModelType ModelEntry_getType(const ModelEntry *entry);

void ModelEntry_doCallback(const ModelEntry *entry, PlayerModelManagerModelEvent eventId);

void ModelEntry_setFlags(ModelEntry *entry, u64 flags);

void ModelEntry_unsetFlags(ModelEntry *entry, u64 flags);

void ModelEntry_unsetAllFlags(ModelEntry *entry);

bool ModelEntry_isAnyFlagEnabled(ModelEntry *entry, u64 flags);

void ModelEntry_setCallback(ModelEntry *entry, PlayerModelManagerEventHandler *callback, void *data);

ModelEntryForm *ModelEntryForm_new(PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName);

ModelEntry *ModelEntryForm_getModelEntry(ModelEntryForm *entry);

TexturePtr ModelEntryForm_getEyesTexture(ModelEntryForm *entry, PlayerEyeIndex i);

TexturePtr ModelEntryForm_getMouthTexture(ModelEntryForm *entry, PlayerMouthIndex i);

void ModelEntryForm_setEyesTexture(ModelEntryForm *entry, TexturePtr tex, PlayerEyeIndex i);

void ModelEntryForm_setMouthTexture(ModelEntryForm *entry, TexturePtr tex, PlayerMouthIndex i);

void ModelEntryForm_fillDefaultFaceTextures(ModelEntryForm *entry);

void ModelEntryForm_setDLsFromSkeletons(ModelEntryForm *entry);

void ModelEntryForm_setSkeleton(ModelEntryForm *entry, FlexSkeletonHeader *skel);

FlexSkeletonHeader *ModelEntryForm_getSkeleton(ModelEntryForm *entry);

void ModelEntryForm_setShieldingSkeleton(ModelEntryForm *entry, FlexSkeletonHeader *skel);

FlexSkeletonHeader *ModelEntryForm_getShieldingSkeleton(ModelEntryForm *entry);

ModelEntryEquipment *ModelEntryEquipment_new(PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName);

ModelEntry *ModelEntryEquipment_getModelEntry(ModelEntryEquipment *entry);

ModelEntryPack *ModelEntryPack_new(PlayerModelManagerHandle handle, char *internalName);

ModelEntry *ModelEntryPack_getModelEntry(ModelEntryPack *entry);

ModelEntry const *const *ModelEntryPack_getModelEntries(const ModelEntryPack *entry);

size_t ModelEntryPack_getModelEntriesCount(const ModelEntryPack *entry);

bool ModelEntryPack_addEntryToPack(ModelEntryPack *entry, ModelEntry *entryToAdd);

#endif
