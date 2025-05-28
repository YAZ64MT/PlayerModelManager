#include "global.h"
#include "recompdata.h"
#include "objectmanager.h"
#include "recomputils.h"
#include "zobjutils.h"

extern bool gIsProxyLoaderDisabled;

// we can use a U32 value hash map because ptrs are 32 bit
U32ValueHashmapHandle gVromToObjMap;

// avoid going through the potentially expensive repoint op more than a single time per dl
U32ValueHashmapHandle gRepointTracker;

void ObjectManager_init() {
    gVromToObjMap = recomputil_create_u32_value_hashmap();
    gRepointTracker = recomputil_create_u32_value_hashmap();
}

void *loadObjectFromVrom(uintptr_t vromAddr, size_t size) {

    gIsProxyLoaderDisabled = true;

    void *obj = recomp_alloc(size);

    DmaMgr_RequestSync(obj, vromAddr, size);

    gIsProxyLoaderDisabled = false;

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

Gfx *ObjectManager_getRebasedGfxPtr(uintptr_t vrom, size_t size, Gfx *segmentedAddr) {
    void *obj = ObjectManager_get(vrom, size);

    U32HashsetHandle set;
    recomputil_u32_value_hashmap_get(gRepointTracker, vrom, &set);

    // recomp_printf("Trying to insert 0x0%X into hash set %d\n", (u32)segmentedAddr, set);
    if (recomputil_u32_hashset_insert(set, (u32)segmentedAddr)) {
        // recomp_printf("Repointing gfx ptr 0x0%X -> 0x%X!\n", segmentedAddr, (u32)obj + SEGMENT_OFFSET(segmentedAddr));
        ZobjUtils_repointDisplayList(obj, SEGMENT_OFFSET(segmentedAddr), SEGMENT_NUMBER(segmentedAddr), obj);
    }

    return (void *)((u32)obj + SEGMENT_OFFSET(segmentedAddr));
}

Gfx *ObjectManager_getRebasedGfxPtrById(ObjectId id, Gfx *segmentedAddr) {
    uintptr_t vrom = gObjectTable[id].vromStart;

    size_t size = gObjectTable[id].vromEnd - vrom;

    return ObjectManager_getRebasedGfxPtr(vrom, size, segmentedAddr);
}

void ObjectManager_rebaseNPCHierarchy(uintptr_t vrom, size_t size, FlexSkeletonHeader *skel) {
    void *obj = ObjectManager_get(vrom, size);
    uintptr_t objAddr = (uintptr_t)obj;

    skel = (FlexSkeletonHeader *)((uintptr_t)skel + (uintptr_t)obj);

    u8 limbCount = skel->sh.limbCount;
    void** limbPtrs = skel->sh.segment;

    for (u8 i = 0; i < limbCount; ++i) {
        StandardLimb *limb = (StandardLimb *)(objAddr + SEGMENT_OFFSET(limbPtrs[i]));
        if (limb->dList) {
            limb->dList = (Gfx *)((uintptr_t)obj + SEGMENT_OFFSET(limb->dList));
        }

        limbPtrs[i] = limb;
    }
}

void ObjectManager_rebaseNPCHierarchyById(ObjectId id, FlexSkeletonHeader *skel) {
    uintptr_t vrom = gObjectTable[id].vromStart;

    size_t size = gObjectTable[id].vromEnd - vrom;

    ObjectManager_rebaseNPCHierarchy(vrom, size, skel);
}

RECOMP_CALLBACK("*", recomp_on_init)
void initObjectManagerOnce(PlayState *play) {
    ObjectManager_init();
}
