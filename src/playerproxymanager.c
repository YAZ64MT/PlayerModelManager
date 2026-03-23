#include "global.h"
#include "playerproxymanager.h"
#include "playerproxy.h"
#include "modelentry.h"
#include "modelinfo.h"
#include "formproxy.h"
#include "playermodelconfig.h"
#include "yazmtcorelib_api.h"
#include "logger.h"
#include "utils.h"

static YAZMTCore_IterableU32Set *sPlayerProxies;

RECOMP_CALLBACK(".", _internal_initHashObjects) void initPPMHash(void) {
    sPlayerProxies = YAZMTCore_IterableU32Set_new();
}

PlayerProxy *PlayerProxyManager_createPlayerProxy(void) {
    PlayerProxy *pp = Utils_recompCalloc(sizeof(PlayerProxy));

    PlayerProxy_init(pp);

    YAZMTCore_IterableU32Set_insert(sPlayerProxies, (uintptr_t)pp);

    return pp;
}

static bool isModelEntryInModelInfo(ModelInfo *mi, ModelEntry *entry) {
    if (!mi || !entry) {
        return false;
    }

    return (void *)ModelInfo_getModelEntryForm(mi) == entry;
}

void PlayerProxyManager_refreshAll(void) {
    PlayerProxy *const *proxies = (PlayerProxy *const *)YAZMTCore_IterableU32Set_values(sPlayerProxies);
    size_t numProxies = YAZMTCore_IterableU32Set_size(sPlayerProxies);

    for (size_t i = 0; i < numProxies; ++i) {
        PlayerProxy *currPp = proxies[i];

        if (currPp) {
            PlayerProxy_requestRefresh(currPp);
        }
    }
}

void PlayerProxyManager_refreshFullAllWithModelEntry(ModelEntry *modelEntry) {
    PlayerProxy *const *proxies = (PlayerProxy *const *)YAZMTCore_IterableU32Set_values(sPlayerProxies);
    size_t numProxies = YAZMTCore_IterableU32Set_size(sPlayerProxies);

    for (size_t i = 0; i < numProxies; ++i) {
        PlayerProxy *currPp = proxies[i];

        for (size_t j = 0; j < gFormProxyIds->size; ++j) {
            FormProxy *currFp = PlayerProxy_getFormProxy(currPp, gFormProxyIds->ids[j]);

            if (currFp) {
                if (isModelEntryInModelInfo(FormProxy_getCurrentModelInfo(currFp), modelEntry) ||
                    isModelEntryInModelInfo(FormProxy_getFallbackModelInfo(currFp), modelEntry) ||
                    isModelEntryInModelInfo(FormProxy_getFallbackOverrideModelInfo(currFp), modelEntry)) {
                    PlayerProxy_requestRefresh(currPp);
                }                
            }
        }
    }
}

void PlayerProxyManager_updatePlayerProxies_on_UpdateMain(void) {
    PlayerProxy *const *proxies = (PlayerProxy *const *)YAZMTCore_IterableU32Set_values(sPlayerProxies);
    size_t numProxies = YAZMTCore_IterableU32Set_size(sPlayerProxies);

    for (size_t i = 0; i < numProxies; ++i) {
        PlayerProxy_updateInterpolationStatus(proxies[i]);
    }
}
