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
#include "repy_api.h"

static bool sIsGlobalObjectsReady;
static bool sIsAllHashObjectsInitialized;
static bool sIsREPYReady;

static void doRegisterModels(void);

void registerHuman(void);
void registerDeku(void);
void registerGoron(void);
void registerZora(void);
void registerFierceDeity(void);
void registerKafei(void);

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

    registerKafei();

    gPlayer1ProxyHandle = PlayerProxyManager_createPlayerProxy(PPALLOC_PERMANENT);
    gPlayer1Proxy = PlayerProxyManager_getPlayerProxy(gPlayer1ProxyHandle);

    gPlayer2ProxyHandle = PlayerProxyManager_createPlayerProxy(PPALLOC_PERMANENT);
    gPlayer2Proxy = PlayerProxyManager_getPlayerProxy(gPlayer2ProxyHandle);
}

static void initProxies(void) {
    static bool isFormProxiesInitialized;

    if (!isFormProxiesInitialized) {
        isFormProxiesInitialized = true;
        forceInitFormProxies();
    }
}

void initVanillaMMDLs(void);
void initCustomDLs(void);

GLOBAL_OBJECTS_CALLBACK_ON_READY void initCustomDLsOnGlobalObjects(void) {
    initCustomDLs();
    initVanillaMMDLs();
    sIsGlobalObjectsReady = true;
    doRegisterModels();
}

REPY_ON_POST_INIT void onREPYReady(void) {
    sIsREPYReady = true;
    doRegisterModels();
}

RECOMP_DECLARE_EVENT(_internal_preInitHashObjects(void));
RECOMP_DECLARE_EVENT(_internal_initHashObjects(void));
RECOMP_DECLARE_EVENT(_internal_postInitHashObjects(void));

RECOMP_CALLBACK("*", recomp_on_init) void handleInits(void) {
    _internal_preInitHashObjects();
    _internal_initHashObjects();
    _internal_postInitHashObjects();
    sIsAllHashObjectsInitialized = true;
    doRegisterModels();
}

RECOMP_DECLARE_EVENT(onRegisterModels(void));
RECOMP_DECLARE_EVENT(onReady(void));

void initUIFileList(void);

#define ID_PREFIX "mm."
#define PLAYER1_CFG_ID ID_PREFIX "player1"
#define PLAYER2_CFG_ID ID_PREFIX "kafei"

static void applyDiskModels(void) {
    ModelEntryManager_applySavedEntriesToProxy(gPlayer1Proxy, PLAYER1_CFG_ID);

    ModelEntryManager_applySavedEntriesToProxy(gPlayer2Proxy, PLAYER2_CFG_ID);
    if (!PlayerProxy_getCurrentEntry(gPlayer2Proxy, PMM_MODEL_TYPE_CHILD)) {
        PlayerProxy_tryApplyEntry(gPlayer2Proxy, PMM_MODEL_TYPE_CHILD, ModelEntryManager_getEntry(gKafeiModelHandle));
    }
}

static void doRegisterModels(void) {
    static bool isModelsRegistered;

    if (isModelsRegistered || !sIsAllHashObjectsInitialized || !sIsGlobalObjectsReady || !sIsREPYReady) {
        return;
    }

    isModelsRegistered = true;

    PlayerModelManager_unlockAPI();

    Logger_printInfo("Setting up vanilla models...");
    initProxies();
    ModelEntryManager_registerProxyToSave(gPlayer1Proxy, PLAYER1_CFG_ID);
    ModelEntryManager_registerProxyToSave(gPlayer2Proxy, PLAYER2_CFG_ID);

    Logger_printInfo("Registering custom models...");
    onRegisterModels();

    PlayerModelManager_lockAPI();

    initUIFileList();
    Logger_printInfo("Finished registering models.");

    PlayerProxyManager_refreshAll();

    applyDiskModels();

    Logger_printInfo("Ready!");

    onReady();
}
