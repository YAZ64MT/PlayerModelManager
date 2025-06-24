#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

typedef unsigned long PlayerModelManagerFormHandle;

void *CMEM_loadFromDisk(PlayerTransformation form, const char* path);
void CMEM_setCurrentEntry(PlayerTransformation form, FormModelEntry *entry);
FormModelEntry *CMEM_getCurrentEntry(PlayerTransformation form);
FormModelEntry **CMEM_getCombinedEntries(PlayerTransformation, size_t *count);
bool CMEM_tryApplyEntry(PlayerTransformation form, FormModelEntry *newEntry);
void CMEM_refreshDiskEntries();
void CMEM_removeModel(PlayerTransformation form);
PlayerModelManagerFormHandle CMEM_createMemoryHandle(PlayerTransformation form);
FormModelMemoryEntry *CMEM_getMemoryEntry(PlayerModelManagerFormHandle h);
void CMEM_saveCurrentEntry(PlayerTransformation form);
PlayerTransformation getFormFromModelType(FormModelType t);
void CMEM_reapplyEntry(PlayerTransformation form);
bool CMEM_forceApplyEntry(PlayerTransformation form, FormModelEntry *newEntry);

#endif
