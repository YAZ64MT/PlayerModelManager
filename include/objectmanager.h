#ifndef __OBJECT_MANAGER__
#define __OBJECT_MANAGER__

#include "global.h"
#include "zobjutils.h"

// A workaround for object dependency issues
// This class keeps the objects loaded 

void ObjectManager_init();

// Returns a reference to the object with the given id
// if it was not already loaded, it is loaded here
void *ObjectManager_get(uintptr_t vromAddr, size_t size);

void *ObjectManager_getById(ObjectId id);

Gfx *ObjectManager_getRebasedGfxPtr(uintptr_t vrom, size_t size, Gfx *segmentedAddr);

Gfx *ObjectManager_getRebasedGfxPtrById(ObjectId id, Gfx *segmentedAddr);

#endif
