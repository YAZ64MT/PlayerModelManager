#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

typedef unsigned long PlayerModelManagerHandle;

void CMEM_setCurrentEntry(PlayerTransformation form, FormModelEntry *entry);
FormModelEntry *CMEM_getCurrentEntry(PlayerTransformation form);
FormModelEntry **CMEM_getCombinedEntries(PlayerTransformation, size_t *count);
bool CMEM_tryApplyEntry(PlayerTransformation form, FormModelEntry *newEntry);
void CMEM_removeModel(PlayerTransformation form);
PlayerModelManagerHandle CMEM_createMemoryHandle(PlayerTransformation form, char *internalName);
FormModelMemoryEntry *CMEM_getMemoryEntry(PlayerModelManagerHandle h);
void CMEM_saveCurrentEntry(PlayerTransformation form);
PlayerTransformation getFormFromModelType(FormModelType t);
void CMEM_reapplyEntry(PlayerTransformation form);
bool CMEM_forceApplyEntry(PlayerTransformation form, FormModelEntry *newEntry);

#endif
