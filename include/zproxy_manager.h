#ifndef __PROXY_MANAGER_H__
#define __PROXY_MANAGER_H__

#include "global.h"
#include "zproxy_base.h"
#include "recompdata.h"
#include "z64object.h"

struct ZProxyManager;

extern bool gIsProxyLoaderDisabled;

void ZProxyManager_initManager();

void ZProxyManager_destroyManager();

bool ZProxyManager_registerZProxy(ObjectId id);

bool ZProxyManager_unregisterZProxy(ObjectId id);

bool ZProxyManager_setCustomDisplayList(ObjectId id, void *vanillaDl, Gfx customDl[]);

bool ZProxyManager_removeCustomDisplayList(ObjectId id, void *vanillaDl);

bool ZProxyManager_reserveVanillaDisplayList(ObjectId id, void *vanillaDl);

#endif
