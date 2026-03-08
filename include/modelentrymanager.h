#ifndef CUSTOMMODELENTRYMANAGER_H
#define CUSTOMMODELENTRYMANAGER_H

typedef struct ModelEntry ModelEntry;

#include "playermodelmanager_api.h"
#include "modelmatrixids.h"

typedef unsigned long PlayerModelManagerHandle;
typedef struct PlayerProxy PlayerProxy;

void ModelEntryManager_setCurrentEntry(PlayerModelManagerModelType modelType, const ModelEntry *entry);
const ModelEntry **ModelEntryManager_getCategoryEntryData(PlayerModelManagerModelType modelType, size_t *count);
PlayerModelManagerHandle ModelEntryManager_createMemoryHandle(PlayerModelManagerModelType type, const char *internalName);
ModelEntry *ModelEntryManager_getEntry(PlayerModelManagerHandle h);
void ModelEntryManager_saveCurrentEntry(PlayerProxy *pp, PlayerModelManagerModelType modelType);
bool ModelEntryManager_isEntryHidden(const ModelEntry *ModelEntry);
void ModelEntryManager_setEntryHidden(const ModelEntry *modelEntry, bool isHidden);
void ModelEntryManager_applyModelsFromDisk(PlayerProxy *pp);

PlayerTransformation getFormFromModelType(PlayerModelManagerModelType t);
PlayerTransformation getFormFromModelType(PlayerModelManagerModelType modelType);

#endif
