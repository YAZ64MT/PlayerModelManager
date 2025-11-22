#ifndef PLAYERPROXYMANAGER_H
#define PLAYERPROXYMANAGER_H

#include "modelmatrixids.h"

typedef struct PlayerProxy PlayerProxy;
typedef struct ModelEntry ModelEntry;

PlayerProxy *PlayerProxyManager_createPlayerProxy();
void PlayerProxyManager_refreshAll();
void PlayerProxyManager_refreshFullAllWithModelEntry(ModelEntry *modelEntry);
void PlayerProxyManager_refreshDLAll(Link_DisplayList dlId);
void PlayerProxyManager_refreshMtxAll(Link_EquipmentMatrix mtxId);

#endif
