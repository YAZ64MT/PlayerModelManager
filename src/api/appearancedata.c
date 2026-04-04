#include "global.h"
#include "modding.h"
#include "playermodelmanager_api.h"
#include "playerproxymanager.h"
#include "playerproxy.h"
#include "proxyactorext.h"
#include "modelentrymanager.h"

RECOMP_EXPORT ActorAppearanceDataHandle PlayerModelManager_ActorAppearanceData_createData(void) {
    PlayerProxyHandle h = PlayerProxyManager_createPlayerProxy(PPALLOC_REF_COUNT);

    PlayerProxy *pp = PlayerProxyManager_getPlayerProxy(h);

    if (pp) {
        PlayerProxy_requestRefresh(pp);
    }

    return h;
}

RECOMP_EXPORT bool PlayerModelManager_ActorAppearanceData_assignDataToActor(Actor *actor, ActorAppearanceDataHandle h) {
    return ProxyActorExt_setPlayerProxyHandle(actor, h);
}

RECOMP_EXPORT bool PlayerModelManager_ActorAppearanceData_assignDataFromOtherActor(Actor *dest, Actor *src) {
    return ProxyActorExt_copyProxyInformation(dest, src);
}

RECOMP_EXPORT bool PlayerModelManager_ActorAppearanceData_assignModel(ActorAppearanceDataHandle h, PlayerModelManagerModelType type, const char *internalName) {
    PlayerProxy *pp = PlayerProxyManager_getPlayerProxy(h);

    if (pp) {
        return ModelEntryManager_applyByInternalName(pp, type, internalName);
    }

    return false;
}

RECOMP_EXPORT ActorAppearanceDataHandle PlayerModelManager_Actor_getAppearanceDataHandle(Actor *actor) {
    return ProxyActorExt_getAppearanceDataHandleCopy(actor);
}

RECOMP_EXPORT bool PlayerModelManager_ActorAppearanceData_releaseHandle(ActorAppearanceDataHandle h) {
    return PlayerProxyManager_releaseReference(h);
}
