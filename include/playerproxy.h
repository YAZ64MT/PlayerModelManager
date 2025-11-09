#ifndef __PLAYERPROXY_H__
#define __PLAYERPROXY_H__

#include "recompdata.h"

typedef struct PlayerProxy {
    U32MemoryHashmapHandle formProxies;
} PlayerProxy;

void PlayerProxy_init(PlayerProxy *pp);

#endif
