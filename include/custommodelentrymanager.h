#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

typedef unsigned long PlayerModelHandle;

void *CMEM_loadFromDisk(PlayerTransformation form, const char* path);
void CMEM_setCurrentEntry(PlayerTransformation form, CustomModelEntry *entry);
CustomModelEntry *CMEM_getCurrentEntry(PlayerTransformation form);
CustomModelEntry **CMEM_getCombinedEntries(PlayerTransformation, size_t *count);
bool CMEM_tryApplyEntry(PlayerTransformation form, CustomModelEntry *newEntry);
void CMEM_refreshDiskEntries();
void CMEM_removeModel(PlayerTransformation form);
PlayerModelHandle CMEM_createMemoryHandle(PlayerTransformation form);
CustomModelMemoryEntry *CMEM_getMemoryEntry(PlayerModelHandle h);
void CMEM_saveCurrentEntry(PlayerTransformation form);
PlayerTransformation getFormFromModelType(CustomModelType t);

#endif
