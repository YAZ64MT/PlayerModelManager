#include "global.h"
#include "modding.h"
#include "modelreplacer_api.h"
#include "globalobjects_api.h"
#include "playerproxy.h"
#include "formproxy.h"
#include "playerproxymanager.h"
#include "apilocal.h"
#include "logger.h"
#include "modelinfo.h"
#include "modelentrymanager.h"
#include "fallbackmodels.h"

void registerHuman();
void registerDeku();
void registerGoron();
void registerZora();
void registerFierceDeity();

// initialize player models as blank display lists
void initFormProxies() {
    registerHuman();
    ModelInfo_setModelEntryForm(&gHumanModelInfo, (ModelEntryForm *)ModelEntryManager_getEntry(gHumanModelHandle));
    registerDeku();
    ModelInfo_setModelEntryForm(&gDekuModelInfo, (ModelEntryForm *)ModelEntryManager_getEntry(gDekuModelHandle));
    registerGoron();
    ModelInfo_setModelEntryForm(&gGoronModelInfo, (ModelEntryForm *)ModelEntryManager_getEntry(gGoronModelHandle));
    registerZora();
    ModelInfo_setModelEntryForm(&gZoraModelInfo, (ModelEntryForm *)ModelEntryManager_getEntry(gZoraModelHandle));
    registerFierceDeity();
    ModelInfo_setModelEntryForm(&gFierceDeityModelInfo, (ModelEntryForm *)ModelEntryManager_getEntry(gFierceDeityModelHandle));
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

void initVanillaMMDLs();

GLOBAL_OBJECTS_CALLBACK_ON_READY void initFormProxiesOnGlobalObjects() {
    initVanillaMMDLs();
}

RECOMP_DECLARE_EVENT(_internal_preInitHashObjects());
RECOMP_DECLARE_EVENT(_internal_initHashObjects());
RECOMP_DECLARE_EVENT(_internal_postInitHashObjects());

RECOMP_CALLBACK("*", recomp_on_init) void handleInits() {
    _internal_preInitHashObjects();
    _internal_initHashObjects();
    _internal_postInitHashObjects();
}
