#ifndef __PROXY_BASE_H__
#define __PROXY_BASE_H__

#include "global.h"
#include "recompdata.h"
#include "z64object.h"
#include "libc/stdbool.h"
#include "linkedlist.h"

typedef struct {
    void* vanillaDisplayList;
    Gfx displayList;
    Gfx proxyDisplayList;
} ZProxy_ProxyContainer;

typedef struct {
    U32MemoryHashmapHandle vanillaToCustomMap;
    void *vanillaObject;
    LinkedList vanillaDisplayLists;
} ZProxy;

void ZProxy_initZProxy(ZProxy *zProxy);

void ZProxy_destroyZProxy(ZProxy *zProxy);

bool ZProxy_setCustomDisplayList(ZProxy *zProxy, void *vanillaDisplayList, Gfx customDisplayList[]);

bool zProxy_removeCustomDisplayList(ZProxy *zProxy, void *vanillaDisplayList);

bool zProxy_reserveContainer(ZProxy *zProxy, void *vanillaDisplayList);

bool ZProxy_refreshAllContainers(ZProxy *zProxy);

#endif
