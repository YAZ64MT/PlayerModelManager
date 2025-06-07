#ifndef __CUSTOMMODELENTRYMANAGER_H
#define __CUSTOMMODELENTRYMANAGER_H
#include "custommodelentry.h"

void CMEM_setCurrentEntry(CustomModelEntry *e);
CustomModelEntry *CMEM_getCurrentEntry();
void CMEM_pushMemoryEntry(CustomModelMemoryEntry *entry);
void CMEM_pushDiskEntry(CustomModelDiskEntry *entry);
void CMEM_clearDiskEntries();

#endif
