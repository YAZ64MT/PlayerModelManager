
#include "zproxy_manager.h"
#include "recomputils.h"
#include "libc/string.h"
#include "zobjutils.h"
#include "objectmanager.h"

RECOMP_DECLARE_EVENT(ZProxyManager_onInit())

bool gIsProxyLoaderDisabled = false;

typedef struct ZProxyManager {
    U32MemoryHashmapHandle vromToZProxyMap;
} ZProxyManager;

ZProxyManager gZProxyManager;

bool gIsManagerInitialized = false;

#define GET_OBJ_VROM_ADDR(objId) gObjectTable[objId].vromStart
#define IS_ZPROXY_EXIST(objId) recomputil_u32_memory_hashmap_contains(gZProxyManager.vromToZProxyMap, GET_OBJ_VROM_ADDR(objId))
#define GET_ZPROXY_FROM_VROM(vRomAddr) recomputil_u32_memory_hashmap_get(gZProxyManager.vromToZProxyMap, vRomAddr)
#define GET_ZPROXY(objId) GET_ZPROXY_FROM_VROM(GET_OBJ_VROM_ADDR(objId))
#define CREATE_ZPROXY_ENTRY(objId) recomputil_u32_memory_hashmap_create(gZProxyManager.vromToZProxyMap, GET_OBJ_VROM_ADDR(objId))
#define REMOVE_ZPROXY_ENTRY(objId) recomputil_u32_memory_hashmap_erase(gZProxyManager.vromToZProxyMap, GET_OBJ_VROM_ADDR(objId))

void ZProxyManager_initManager() {
    gZProxyManager.vromToZProxyMap = recomputil_create_u32_memory_hashmap(sizeof(ZProxy));
}

void ZProxyManager_destroyManager() {
    recomputil_destroy_u32_memory_hashmap(gZProxyManager.vromToZProxyMap);
}

bool ZProxyManager_registerZProxy(ObjectId id) {
    if (!IS_ZPROXY_EXIST(id) && CREATE_ZPROXY_ENTRY(id)) {
        ZProxy *zProxy = GET_ZPROXY(id);

        if (zProxy) {
            ZProxy_initZProxy(zProxy);
            return true;
        }
    }

    return false;
}

RECOMP_EXPORT
bool ZProxyManager_unregisterZProxy(ObjectId id) {
    ZProxy *zProxy = GET_ZPROXY(id);

    if (zProxy) {
        ZProxy_destroyZProxy(zProxy);
        return true;
    }

    return false;
}

RECOMP_EXPORT
bool ZProxyManager_setCustomDisplayList(ObjectId id, void *vanillaDl, Gfx customDl[]) {
    ZProxy *zProxy = GET_ZPROXY(id);

    if (!zProxy) {
        ZProxyManager_registerZProxy(id);
        zProxy = GET_ZPROXY(id);
    }

    return ZProxy_setCustomDisplayList(zProxy, vanillaDl, customDl);
}

RECOMP_EXPORT
bool ZProxyManager_removeCustomDisplayList(ObjectId id, void *vanillaDl) {
    ZProxy *zProxy = GET_ZPROXY(id);

    if (!zProxy) {
        ZProxyManager_registerZProxy(id);
        zProxy = GET_ZPROXY(id);
    }

    return zProxy_removeCustomDisplayList(zProxy, vanillaDl);
}

RECOMP_EXPORT
bool ZProxyManager_reserveVanillaDisplayList(ObjectId id, void *vanillaDl) {
    ZProxy *zProxy = GET_ZPROXY(id);

    if (!zProxy) {
        ZProxyManager_registerZProxy(id);
        zProxy = GET_ZPROXY(id);
    }

    return zProxy_reserveContainer(zProxy, vanillaDl);
}

void *gRam;
uintptr_t gVrom;
size_t gSize;

RECOMP_HOOK("DmaMgr_ProcessRequest")
void on_DmaMgr_RequestSync(DmaRequest *req) {
    gRam = req->dramAddr;
    gVrom = req->vromAddr;
    gSize = req->size;
}

RECOMP_HOOK_RETURN("DmaMgr_ProcessRequest")
void post_DmaMgr_RequestSync() {
    if (gIsProxyLoaderDisabled) {
        return;
    }

    ZProxy *zProxy = GET_ZPROXY_FROM_VROM(gVrom);
    if (zProxy) {
        if (!zProxy->vanillaObject) {
            zProxy->vanillaObject = ObjectManager_get(gVrom, gSize);
            ZProxy_refreshAllContainers(zProxy);
        }

        Node *curr = LinkedList_start(&zProxy->vanillaDisplayLists);
        while (curr) {
            u32 vanilla = (u32)curr->data;

            ZProxy_ProxyContainer *container = recomputil_u32_memory_hashmap_get(zProxy->vanillaToCustomMap, vanilla);

            if (container) {
                //recomp_printf("Injecting Proxy: 0x%x -> 0x%x\n", vanilla, &container->proxyDisplayList + SEGMENT_OFFSET(vanilla));

                gSPBranchList((uintptr_t)gRam + SEGMENT_OFFSET(vanilla), &container->proxyDisplayList);
            }

            curr = curr->next;
        }
    }

    gVrom = 0;
    gRam = NULL;
}

RECOMP_HOOK("TitleSetup_SetupTitleScreen")
void readPendingInitsOnce(PlayState *play) {
    if (!gIsManagerInitialized) {
        ZProxyManager_onInit();
        gIsManagerInitialized = true;
    }
}

RECOMP_CALLBACK("*", recomp_on_init)
void initZProxyManagerOnce(PlayState *play) {
    ZProxyManager_initManager();
}
