#ifndef PLAYERPROXY_H
#define PLAYERPROXY_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "z64player.h"
#include "recompdata.h"
#include "modelmatrixids.h"
#include "stdbool.h"

typedef struct FormProxy FormProxy;

typedef struct PlayerProxy {
    U32MemoryHashmapHandle formProxies;
} PlayerProxy;

void PlayerProxy_init(PlayerProxy *pp);
FormProxy *PlayerProxy_getFormProxy(PlayerProxy *pp, FormProxyId formId);
void PlayerProxy_refresh(PlayerProxy *pp);
void PlayerProxy_setOverrideDL(PlayerProxy *pp, Link_DisplayList dlId, Gfx *dl);
void PlayerProxy_setOverrideMtx(PlayerProxy *pp, Link_EquipmentMatrix mtxId, Mtx *mtx);
bool PlayerProxy_getProxyIdFromForm(PlayerTransformation form, FormProxyId *out);
void PlayerProxy_requestTunicColorOverride(PlayerProxy *pp, Color_RGBA8 color);

extern PlayerProxy *gPlayer1Proxy;

#endif
