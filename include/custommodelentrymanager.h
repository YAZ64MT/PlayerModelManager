#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

typedef unsigned long ZPlayerModelHandle;

void CMEM_setCurrentEntry(CustomModelEntry *e);
CustomModelEntry *CMEM_getCurrentEntry();
CustomModelEntry **CMEM_getCombinedEntries(size_t *count);
bool CMEM_tryApplyEntry(CustomModelEntry *newEntry, Link_FormProxy *proxy);
void CMEM_refreshDiskEntries();
void CMEM_removeModel(Link_FormProxy *proxy);
ZPlayerModelHandle CMEM_createMemoryHandle();
CustomModelMemoryEntry *CMEM_getMemoryEntry(ZPlayerModelHandle h);

#endif
