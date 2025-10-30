#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

typedef unsigned long PlayerModelManagerHandle;

void CMEM_setCurrentEntry(Link_CustomModelCategory cat, const ModelEntry *entry);
const ModelEntry *CMEM_getCurrentEntry(Link_CustomModelCategory cat);
const ModelEntry **CMEM_getCategoryEntryData(Link_CustomModelCategory cat, size_t *count);
bool CMEM_tryApplyEntry(Link_CustomModelCategory cat, const ModelEntry *newEntry);
void CMEM_removeModel(Link_CustomModelCategory cat);
PlayerModelManagerHandle CMEM_createMemoryHandle(PlayerModelManagerModelType type, char *internalName);
ModelEntry *CMEM_getEntry(PlayerModelManagerHandle h);
void CMEM_saveCurrentEntry(Link_CustomModelCategory cat);
void CMEM_reapplyEntry(Link_CustomModelCategory cat);
bool CMEM_forceApplyEntry(Link_CustomModelCategory cat, const ModelEntry *newEntry);

PlayerTransformation getFormFromModelType(PlayerModelManagerModelType t);
Link_CustomModelCategory getCategoryFromModelType(PlayerModelManagerModelType t);
PlayerTransformation getFormFromCategory(Link_CustomModelCategory cat);
Link_EquipmentReplacement getEquipmentReplacementFromCategory(Link_CustomModelCategory cat);

bool isEquipmentCategory(Link_CustomModelCategory cat);
bool isFormCategory(Link_CustomModelCategory cat);
bool isPackCategory(Link_CustomModelCategory cat);
bool isValidCategory(Link_CustomModelCategory cat);

#endif
