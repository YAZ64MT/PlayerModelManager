#include "global.h"
#include "modding.h"
#include "playermodelmanager_api.h"
#include "playerproxymanager.h"
#include "playerproxy.h"
#include "proxyactorext.h"
#include "modelentrymanager.h"
#include "utils.h"
#include "string.h"

RECOMP_EXPORT ActorAppearanceDataHandle PlayerModelManager_AppearanceData_createData(void) {
    PlayerProxyHandle h = PlayerProxyManager_createPlayerProxy(PPALLOC_REF_COUNT);

    PlayerProxy *pp = PlayerProxyManager_getPlayerProxy(h);

    if (pp) {
        PlayerProxy_requestRefresh(pp);
    }

    return h;
}

RECOMP_EXPORT bool PlayerModelManager_AppearanceData_assignDataToActor(Actor *actor, ActorAppearanceDataHandle h) {
    return ProxyActorExt_setPlayerProxyHandle(actor, h);
}

RECOMP_EXPORT bool PlayerModelManager_AppearanceData_assignDataFromOtherActor(Actor *dest, Actor *src) {
    return ProxyActorExt_copyProxyInformation(dest, src);
}

RECOMP_EXPORT bool PlayerModelManager_AppearanceData_assignModel(ActorAppearanceDataHandle h, PlayerModelManagerModelType type, const char *internalName) {
    PlayerProxy *pp = PlayerProxyManager_getPlayerProxy(h);

    if (pp) {
        return ModelEntryManager_applyByInternalName(pp, type, internalName);
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_AppearanceData_releaseHandle(ActorAppearanceDataHandle h) {
    if (h != gPlayer1ProxyHandle && h != gPlayer2ProxyHandle) {
        PlayerProxyManager_releaseReference(h);
    }
}

RECOMP_EXPORT bool PlayerModelManager_AppearanceData_setTunicColor(ActorAppearanceDataHandle h, PlayerModelManagerModelType type, Color_RGBA8 color) {
    if (Utils_isFormModelType(type)) {
        PlayerProxy *pp = PlayerProxyManager_getPlayerProxy(h);

        if (pp) {
            PlayerProxy_requestTunicColorOverrideForModelType(pp, type, color);
            return true;
        }
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_AppearanceData_getTunicColor(ActorAppearanceDataHandle h, PlayerModelManagerModelType type, Color_RGBA8 *out) {
    if (Utils_isFormModelType(type)) {
        PlayerProxy *pp = PlayerProxyManager_getPlayerProxy(h);

        if (pp) {
            return PlayerProxy_getTunicColor(pp, type, out);
        }
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_AppearanceData_getModelName(ActorAppearanceDataHandle h, PlayerModelManagerModelType type, char outBuf[], size_t outBufSize) {
    PlayerProxy *pp = PlayerProxyManager_getPlayerProxy(h);

    if (pp) {
        const ModelEntry *entry = PlayerProxy_getCurrentEntry(pp, type);

        if (entry) {
            const char *internalName = ModelEntry_getInternalName(entry);
            
            // we use > and < rather than >= and <= for the buffer length checks to account for the NULL terminator
            if (internalName && (outBufSize > PMM_MAX_INTERNAL_NAME_LENGTH || strlen(internalName) < outBufSize)) {
                strcpy(outBuf, internalName);
                return true;
            }
        }
    }

    return false;
}

RECOMP_EXPORT ActorAppearanceDataHandle PlayerModelManager_AppearanceData_getBuiltInPlayerHandle() {
    return gPlayer1ProxyHandle;
}
