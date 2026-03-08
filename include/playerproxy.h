#ifndef PLAYERPROXY_H
#define PLAYERPROXY_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "z64player.h"
#include "recompdata.h"
#include "modelmatrixids.h"
#include "playermodelmanager_api.h"
#include "stdbool.h"
#include "modelentry.h"

typedef struct FormProxy FormProxy;

typedef struct PlayerProxy {
    U32MemoryHashmapHandle formProxies;
    U32ValueHashmapHandle modelEntries;
} PlayerProxy;

void PlayerProxy_init(PlayerProxy *pp);
FormProxy *PlayerProxy_getFormProxy(PlayerProxy *pp, FormProxyId formId);
void PlayerProxy_requestRefresh(PlayerProxy *pp);
void PlayerProxy_setOverrideDL(PlayerProxy *pp, Link_DisplayList dlId, Gfx *dl);
void PlayerProxy_setOverrideMtx(PlayerProxy *pp, Link_EquipmentMatrix mtxId, Mtx *mtx);
bool PlayerProxy_getProxyIdFromForm(PlayerTransformation form, FormProxyId *out);
void PlayerProxy_requestTunicColorOverride(PlayerProxy *pp, Color_RGBA8 color);
const ModelEntry *PlayerProxy_getCurrentEntry(PlayerProxy *pp, PlayerModelManagerModelType modelType);
bool PlayerProxy_tryApplyEntry(PlayerProxy *pp, PlayerModelManagerModelType modelType, const ModelEntry *newEntry);
bool PlayerProxy_forceApplyEntry(PlayerProxy *pp, PlayerModelManagerModelType modelType, const ModelEntry *newEntry);
void PlayerProxy_removeEntry(PlayerProxy *pp, PlayerModelManagerModelType modelType);

extern PlayerProxy *gPlayer1Proxy;
extern PlayerProxy *gPlayer2Proxy;

#endif
