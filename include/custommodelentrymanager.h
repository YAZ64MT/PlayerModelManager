#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

typedef unsigned long PlayerModelManagerHandle;

void CMEM_setCurrentEntry(PlayerTransformation form, ModelEntry *entry);
ModelEntry *CMEM_getCurrentEntry(PlayerTransformation form);
ModelEntry **CMEM_getCombinedEntries(PlayerTransformation, size_t *count);
bool CMEM_tryApplyEntry(PlayerTransformation form, ModelEntry *newEntry);
void CMEM_removeModel(PlayerTransformation form);
PlayerModelManagerHandle CMEM_createMemoryHandle(PlayerTransformation form, char *internalName);
ModelEntryForm *CMEM_getFormEntry(PlayerModelManagerHandle h);
void CMEM_saveCurrentEntry(PlayerTransformation form);
PlayerTransformation getFormFromModelType(FormModelType t);
void CMEM_reapplyEntry(PlayerTransformation form);
bool CMEM_forceApplyEntry(PlayerTransformation form, ModelEntry *newEntry);

#endif
