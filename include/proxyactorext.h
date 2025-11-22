#ifndef PROXYACTOREXT_H
#define PROXYACTOREXT_H

#include "stdbool.h"
#include "modelmatrixids.h"

typedef struct Actor Actor;
typedef struct PlayerProxy PlayerProxy;
typedef struct FormProxy FormProxy;

FormProxy *ProxyActorExt_getFormProxy(Actor *actor);
PlayerProxy *ProxyActorExt_getPlayerProxy(Actor *actor);
bool ProxyActorExt_getFormProxyId(Actor *actor, FormProxyId *out);
bool ProxyActorExt_setFormProxyId(Actor *actor, FormProxyId id);
bool ProxyActorExt_setPlayerProxy(Actor *actor, PlayerProxy *pp);
bool ProxyActorExt_copyProxyInformation(Actor *dest, Actor *src);

#endif
