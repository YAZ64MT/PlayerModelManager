#ifndef CUSTOMMODELENTRYMANAGER_H
#define CUSTOMMODELENTRYMANAGER_H

typedef struct ModelEntry ModelEntry;

#include "playermodelmanager_api.h"
#include "modelmatrixids.h"

typedef unsigned long PlayerModelManagerHandle;

void ModelEntryManager_setCurrentEntry(Link_CustomModelCategory cat, const ModelEntry *entry);
const ModelEntry *ModelEntryManager_getCurrentEntry(Link_CustomModelCategory cat);
const ModelEntry **ModelEntryManager_getCategoryEntryData(Link_CustomModelCategory cat, size_t *count);
bool ModelEntryManager_tryApplyEntry(Link_CustomModelCategory cat, const ModelEntry *newEntry);
void ModelEntryManager_removeModel(Link_CustomModelCategory cat);
PlayerModelManagerHandle ModelEntryManager_createMemoryHandle(PlayerModelManagerModelType type, const char *internalName);
ModelEntry *ModelEntryManager_getEntry(PlayerModelManagerHandle h);
void ModelEntryManager_saveCurrentEntry(Link_CustomModelCategory cat);
void ModelEntryManager_reapplyEntry(Link_CustomModelCategory cat);
bool ModelEntryManager_forceApplyEntry(Link_CustomModelCategory cat, const ModelEntry *newEntry);
bool ModelEntryManager_isEntryHidden(const ModelEntry *ModelEntry);
void ModelEntryManager_setEntryHidden(const ModelEntry *modelEntry, bool isHidden);
void ModelEntryManager_applyModelsFromDisk(void);

PlayerTransformation getFormFromModelType(PlayerModelManagerModelType t);
Link_CustomModelCategory getCategoryFromModelType(PlayerModelManagerModelType t);
PlayerTransformation getFormFromCategory(Link_CustomModelCategory cat);
Link_EquipmentReplacement getEquipmentReplacementFromCategory(Link_CustomModelCategory cat);

bool isEquipmentCategory(Link_CustomModelCategory cat);
bool isFormCategory(Link_CustomModelCategory cat);
bool isPackCategory(Link_CustomModelCategory cat);
bool isValidCategory(Link_CustomModelCategory cat);

#endif
