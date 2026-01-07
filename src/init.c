#include "global.h"
#include "modding.h"
#include "globalobjects_api.h"
#include "playerproxy.h"
#include "formproxy.h"
#include "playerproxymanager.h"
#include "apilocal.h"
#include "logger.h"
#include "modelinfo.h"
#include "modelentrymanager.h"
#include "fallbackmodels.h"

static bool sIsModelEntryManagerReady;
static bool sIsGlobalObjectsReady;
static bool sIsModelsRegistered;
static bool sIsAllHashObjectsInitialized;

static void doRegisterModels(void);

void registerHuman(void);
void registerDeku(void);
void registerGoron(void);
void registerZora(void);
void registerFierceDeity(void);

// initialize player models as blank display lists
static void forceInitFormProxies(void) {
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

void initFormProxies(void) {
    static bool isFormProxiesInitialized;

    if (!isFormProxiesInitialized) {
        isFormProxiesInitialized = true;
        forceInitFormProxies();
        PlayerProxyManager_refreshAll();
    }
}

void initVanillaMMDLs(void);
void initCustomDLs(void);

GLOBAL_OBJECTS_CALLBACK_ON_READY void initFormProxiesOnGlobalObjects(void) {
    initCustomDLs();
    initVanillaMMDLs();
    sIsGlobalObjectsReady = true;

}

RECOMP_DECLARE_EVENT(_internal_preInitHashObjects(void));
RECOMP_DECLARE_EVENT(_internal_initHashObjects(void));
RECOMP_DECLARE_EVENT(_internal_postInitHashObjects(void));

RECOMP_CALLBACK("*", recomp_on_init) void handleInits(void) {
    _internal_preInitHashObjects();
    _internal_initHashObjects();
    _internal_postInitHashObjects();

    doRegisterModels();
    sIsAllHashObjectsInitialized = true;
}

RECOMP_DECLARE_EVENT(onRegisterModels(void));
RECOMP_DECLARE_EVENT(onReady(void));
RECOMP_DECLARE_EVENT(_internal_onFinishedRegisterModels(void));

static void doRegisterModels(void) {
    if (sIsModelsRegistered) {
        return;
    }

    if (!sIsModelEntryManagerReady || !sIsGlobalObjectsReady) {
        return;
    }

    sIsModelsRegistered = true;

    PlayerModelManager_unlockAPI();

    Logger_printInfo("Setting up vanilla models...");
    initFormProxies();

    Logger_printInfo("Registering custom models...");
    onRegisterModels();

    PlayerModelManager_lockAPI();

    _internal_onFinishedRegisterModels();
    Logger_printInfo("Finished registering models.");
    onReady();
    Logger_printInfo("Ready!");
}
