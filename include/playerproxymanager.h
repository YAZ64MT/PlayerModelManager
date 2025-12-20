#ifndef PLAYERPROXYMANAGER_H
#define PLAYERPROXYMANAGER_H

#include "modelmatrixids.h"

typedef struct PlayerProxy PlayerProxy;
typedef struct ModelEntry ModelEntry;

PlayerProxy *PlayerProxyManager_createPlayerProxy();
void PlayerProxyManager_refreshAll();
void PlayerProxyManager_refreshFullAllWithModelEntry(ModelEntry *modelEntry);

#endif
