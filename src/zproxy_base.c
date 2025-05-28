#include "recomputils.h"
#include "zproxy_base.h"
#include "model_common.h"
#include "zobjutils.h"

void ZProxy_initZProxy(ZProxy *this) {
    this->vanillaToCustomMap = recomputil_create_u32_memory_hashmap(sizeof(ZProxy_ProxyContainer));

    this->vanillaObject = NULL;
    
    LinkedList_initList(&this->vanillaDisplayLists);
}

void ZProxy_destroyZProxy(ZProxy *this) {
    recomputil_destroy_u32_memory_hashmap(this->vanillaToCustomMap);
    
    if (this->vanillaObject) {
        recomp_free(this->vanillaObject);
    }
}

bool zProxy_reserveContainer(ZProxy *this, void *vanillaDisplayList) {
    u32 vanilla = (u32)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaToCustomMap, vanilla);

    if (!container) {
        if (recomputil_u32_memory_hashmap_create(this->vanillaToCustomMap, vanilla)) {

            LinkedList_addBack(&this->vanillaDisplayLists, vanillaDisplayList);

            container = recomputil_u32_memory_hashmap_get(this->vanillaToCustomMap, vanilla);

            container->vanillaDisplayList = vanillaDisplayList;

            container->displayList = callDfCommand[0];

            if (this->vanillaObject) {
                ZobjUtils_repointDisplayList(this->vanillaObject, SEGMENT_OFFSET(vanilla), SEGMENT_NUMBER(vanilla), this->vanillaObject);
                gSPBranchList(&container->displayList, (Gfx *)(SEGMENT_OFFSET(vanilla) + (u32)this->vanillaObject));
            }

            gSPBranchList(&container->proxyDisplayList, &container->displayList);

            return true;
        }
    }

    return false;
}

bool ZProxy_setCustomDisplayList(ZProxy *this, void *vanillaDisplayList, Gfx customDisplayList[]) {
    zProxy_reserveContainer(this, vanillaDisplayList);

    u32 vanilla = (u32)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaToCustomMap, vanilla);

    if (container) {
        gSPBranchList(&container->displayList, customDisplayList);
        return true;
    }

    return false;
}

bool zProxy_removeCustomDisplayList(ZProxy *this, void *vanillaDisplayList) {
    zProxy_reserveContainer(this, vanillaDisplayList);

    u32 vanilla = (u32)vanillaDisplayList;

    ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(this->vanillaToCustomMap, vanilla);

    if (container && this->vanillaObject) {
        gSPBranchList(&container->displayList, (u32)this->vanillaObject + SEGMENT_OFFSET(vanillaDisplayList));
        return true;
    }

    return false;
}

bool isContainerDisplayListEmpty(Gfx *dl) {
    return dl->words.w1 == callDfCommand->words.w1;
}

bool ZProxy_refreshAllContainers(ZProxy *this) {
    if (!this->vanillaObject) {
        return false;
    }

    Node *curr = LinkedList_start(&this->vanillaDisplayLists);

    ZProxy_ProxyContainer *container;

    while (curr) {
        container = recomputil_u32_memory_hashmap_get((u32)container->vanillaDisplayList, (u32)this->vanillaToCustomMap);
        if (container && isContainerDisplayListEmpty(&container->displayList)) {
            u32 segOffset = SEGMENT_OFFSET(container->vanillaDisplayList);

            ZobjUtils_repointDisplayList(this->vanillaObject, segOffset, SEGMENT_NUMBER(container->vanillaDisplayList), this->vanillaObject);
            
            gSPBranchList(&container->displayList, segOffset + (u32)this->vanillaObject);
        }

        curr = curr->next;
    }

    return true;
}
