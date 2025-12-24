#include "global.h"
#include "modding.h"
#include "modelreplacer_api.h"
#include "modelreplacer_compat.h"
#include "globalobjects_api.h"
#include "playerproxy.h"
#include "formproxy.h"
#include "playerproxymanager.h"

RECOMP_DECLARE_EVENT(_internal_setupVanillaModels());

// initialize player models as blank display lists
void initFormProxies() {
    _internal_setupVanillaModels();

    gPlayer1Proxy = PlayerProxyManager_createPlayerProxy();
}

void doInitFormProxies() {
    static bool isFormProxiesInitialized;

    if (!isFormProxiesInitialized) {
        isFormProxiesInitialized = true;
        initFormProxies();
        PlayerProxyManager_refreshAll();
    }
}

RECOMP_CALLBACK(".", _internal_onReadyModelReplacerCompat) void initFormProxiesOnModelReplacerCompat() {
    doInitFormProxies();
}

GLOBAL_OBJECTS_CALLBACK_ON_READY void initFormProxiesOnGlobalObjects() {
    if (!MRC_isMRCEnabled()) {
        doInitFormProxies();
    }
}

RECOMP_DECLARE_EVENT(_internal_preInitHashObjects());
RECOMP_DECLARE_EVENT(_internal_initHashObjects());
RECOMP_DECLARE_EVENT(_internal_postInitHashObjects());

RECOMP_CALLBACK("*", recomp_on_init) void handleInits() {
    _internal_preInitHashObjects();
    _internal_initHashObjects();
    _internal_postInitHashObjects();
}
