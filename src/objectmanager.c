#include "global.h"
#include "recompdata.h"
#include "objectmanager.h"
#include "recomputils.h"
#include "zobjutils.h"

extern bool gDisableProxyLoader;

// we can use a U32 value hash map because ptrs are 32 bit
U32ValueHashmapHandle gVromToObjMap;

// avoid going through the potentially expensive repoint op more than a single time per dl
U32ValueHashmapHandle gRepointTracker;

void ObjectManager_init() {
    gVromToObjMap = recomputil_create_u32_value_hashmap();
    gRepointTracker = recomputil_create_u32_value_hashmap();
}

void *loadObjectFromVrom(uintptr_t vromAddr, size_t size) {

    gDisableProxyLoader = true;

    void *obj = recomp_alloc(size);

    DmaMgr_RequestSync(obj, vromAddr, size);

    gDisableProxyLoader = false;

    return obj;
}

void *ObjectManager_get(uintptr_t vromAddr, size_t size) {
    u32 addr;

    if (!recomputil_u32_value_hashmap_get(gVromToObjMap, vromAddr, &addr)) {
        addr = (u32)loadObjectFromVrom(vromAddr, size);
        recomputil_u32_value_hashmap_insert(gVromToObjMap, vromAddr, addr);
        recomputil_u32_value_hashmap_insert(gRepointTracker, vromAddr, recomputil_create_u32_hashset());
    }

    return (void *)addr;
}

void *ObjectManager_getById(ObjectId id) {
    uintptr_t vrom = gObjectTable[id].vromStart;

    size_t size = gObjectTable[id].vromEnd - vrom;

    return ObjectManager_get(vrom, size);
}

void *ObjectManager_getRebasedGfxPtr(uintptr_t vrom, size_t size, Gfx *segmentedAddr) {
    void *obj = ObjectManager_get(vrom, size);

    U32HashsetHandle set;
    recomputil_u32_value_hashmap_get(gRepointTracker, vrom, &set);

    if (!recomputil_u32_hashset_insert(set, (u32)segmentedAddr)) {
        ZobjUtils_repointDisplayList(obj, SEGMENT_OFFSET(segmentedAddr), SEGMENT_NUMBER(segmentedAddr), obj);
    }

    return (void *)((u32)obj + SEGMENT_OFFSET(segmentedAddr));
}

void *ObjectManager_getRebasedGfxPtrById(ObjectId id, Gfx *segmentedAddr) {
    uintptr_t vrom = gObjectTable[id].vromStart;

    size_t size = gObjectTable[id].vromEnd - vrom;

    return ObjectManager_getRebasedGfxPtr(vrom, size, segmentedAddr);
}

RECOMP_CALLBACK("*", recomp_on_init)
void initObjectManagerOnce(PlayState *play) {
    ObjectManager_init();
}
