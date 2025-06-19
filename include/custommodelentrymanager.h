#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

typedef unsigned long ZPlayerModelHandle;

void *CMEM_loadFromDisk(PlayerTransformation form, const char* path);
void CMEM_setCurrentEntry(PlayerTransformation form, CustomModelEntry *entry);
CustomModelEntry *CMEM_getCurrentEntry(PlayerTransformation form);
CustomModelEntry **CMEM_getCombinedEntries(PlayerTransformation, size_t *count);
bool CMEM_tryApplyEntry(PlayerTransformation form, CustomModelEntry *newEntry);
void CMEM_refreshDiskEntries();
void CMEM_removeModel(PlayerTransformation form);
ZPlayerModelHandle CMEM_createMemoryHandle(PlayerTransformation form);
CustomModelMemoryEntry *CMEM_getMemoryEntry(ZPlayerModelHandle h);
void CMEM_saveCurrentEntry(PlayerTransformation form);
PlayerTransformation getFormFromModelType(CustomModelType t);

#endif
