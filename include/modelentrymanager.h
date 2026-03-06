#ifndef CUSTOMMODELENTRYMANAGER_H
#define CUSTOMMODELENTRYMANAGER_H

typedef struct ModelEntry ModelEntry;

#include "playermodelmanager_api.h"
#include "modelmatrixids.h"

typedef unsigned long PlayerModelManagerHandle;

void ModelEntryManager_setCurrentEntry(PlayerModelManagerModelType modelType, const ModelEntry *entry);
const ModelEntry *ModelEntryManager_getCurrentEntry(PlayerModelManagerModelType modelType);
const ModelEntry **ModelEntryManager_getCategoryEntryData(PlayerModelManagerModelType modelType, size_t *count);
bool ModelEntryManager_tryApplyEntry(PlayerModelManagerModelType modelType, const ModelEntry *newEntry);
void ModelEntryManager_removeModel(PlayerModelManagerModelType modelType);
PlayerModelManagerHandle ModelEntryManager_createMemoryHandle(PlayerModelManagerModelType type, const char *internalName);
ModelEntry *ModelEntryManager_getEntry(PlayerModelManagerHandle h);
void ModelEntryManager_saveCurrentEntry(PlayerModelManagerModelType modelType);
void ModelEntryManager_reapplyEntry(PlayerModelManagerModelType modelType);
bool ModelEntryManager_forceApplyEntry(PlayerModelManagerModelType modelType, const ModelEntry *newEntry);
bool ModelEntryManager_isEntryHidden(const ModelEntry *ModelEntry);
void ModelEntryManager_setEntryHidden(const ModelEntry *modelEntry, bool isHidden);
void ModelEntryManager_applyModelsFromDisk(void);

PlayerTransformation getFormFromModelType(PlayerModelManagerModelType t);
PlayerTransformation getFormFromModelType(PlayerModelManagerModelType modelType);

bool isEquipmentModelType(PlayerModelManagerModelType modelType);
bool isFormModelType(PlayerModelManagerModelType modelType);
bool isPackModelType(PlayerModelManagerModelType modelType);
bool isValidModelType(PlayerModelManagerModelType modelType);

#endif
