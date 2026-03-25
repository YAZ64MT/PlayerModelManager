#include "global.h"
#include "modding.h"
#include "playermodelmanager_api.h"
#include "playerproxymanager.h"
#include "proxyactorext.h"

RECOMP_EXPORT ActorAppearanceDataHandle PMM_ActorAppearanceData_createData(void) {
    return PlayerProxyManager_createPlayerProxy(PPALLOC_REF_COUNT);
}

RECOMP_EXPORT bool PMM_ActorAppearanceData_assignDataToActor(Actor *actor, ActorAppearanceDataHandle h) {
    return ProxyActorExt_setPlayerProxyHandle(actor, h);
}

RECOMP_EXPORT bool PMM_ActorAppearanceData_assignDataFromOtherActor(Actor *dest, Actor *src) {
    return ProxyActorExt_copyProxyInformation(dest, src);
}


