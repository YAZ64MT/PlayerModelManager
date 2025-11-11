#include "global.h"
#include "modelentrymanager.h"
#include "recomputils.h"
#include "playermodelmanager.h"
#include "model_common.h"
#include "defines_modelinfo.h"
#include "colorfixes.h"
#include "playermodelmanager_api.h"
#include "yazmtcorelib_api.h"
#include "modelreplacer_api.h"
#include "globalobjects_api.h"
#include "modelreplacer_compat.h"
#include "apilocal.h"
#include "logger.h"

static bool isEntryLoaded(ModelEntry *entry) {
    if (entry) {
        return CMEM_getCurrentEntry(ModelEntry_getCategory(entry)) == entry;
    }

    return false;
}

static void refreshProxyIfEntryLoaded(ModelEntry *entry) {
    Link_CustomModelCategory cat = ModelEntry_getCategory(entry);

    if (isEntryLoaded(entry)) {
        CMEM_reapplyEntry(cat);
    }
}

static void refreshProxyDLIfEntryLoaded(ModelEntry *entry, Link_DisplayList dlId) {
    refreshProxyIfEntryLoaded(entry);
}

static void refreshProxyMtxIfEntryLoaded(ModelEntry *entry, Link_EquipmentMatrix mtxId) {
    refreshProxyIfEntryLoaded(entry);
}

static void refreshProxyEyesTexturesIfEntryLoaded(ModelEntry *entry) {
    refreshProxyIfEntryLoaded(entry);
}

static void refreshProxyMouthTexturesIfEntryLoaded(ModelEntry *entry) {
    refreshProxyIfEntryLoaded(entry);
}

static bool isStrTooLong(const char *s, size_t maxLen) {
    size_t length = 0;

    while (*s != '\0') {
        length++;
        s++;

        if (length > maxLen) {
            return true;
        }
    }

    return false;
}

static bool isStrValid(const char *callerName, const char *strToVerify, size_t maxLen) {
    if (!strToVerify) {
        Logger_printError("%s: String passed in was NULL! Aborting!", callerName);
        return false;
    }

    bool isTooLong = isStrTooLong(strToVerify, maxLen);

    if (isTooLong) {
        Logger_printError("%s: String passed in was too long! Maximum length is %u!", callerName, maxLen);
        return false;
    }

    return true;
}

static bool sIsAPILocked = true;

static ModelEntry *getEntryOrPrintErr(PlayerModelManagerHandle h, const char *funcName) {
    ModelEntry *entry = CMEM_getEntry(h);

    if (!entry) {
        Logger_printError("PlayerModelManager: Invalid handle passed in to %s.", funcName);
    }

    return entry;
}

static ModelEntryForm *getFormEntryOrPrintErr(PlayerModelManagerHandle h, const char *funcName) {
    ModelEntry *entry = getEntryOrPrintErr(h, funcName);

    if (!entry) {
        return NULL;
    }

    if (!isFormCategory(ModelEntry_getCategory(entry))) {
        Logger_printWarning("PlayerModelManager: Handle with internal name '%s' does not support the function '%s'", ModelEntry_getInternalName(entry), funcName);
        return NULL;
    }

    return (ModelEntryForm *)entry;
}

static ModelEntry *getEntryOrPrintErrLocked(PlayerModelManagerHandle h, const char *funcName) {
    if (sIsAPILocked) {
        Logger_printWarning(
            "PlayerModelManager: %s called while API locked. "
            "Please only call these functions during a onRegisterModels callback.",
            funcName);

        return NULL;
    }

    return getEntryOrPrintErr(h, funcName);
}

static void dupStrAndFreeOld(char **dest, const char *src) {
    if (*dest) {
        recomp_free(*dest);
    }

    *dest = YAZMTCore_Utils_StrDup(src);
}

RECOMP_EXPORT PlayerModelManagerHandle PlayerModelManager_registerModel(unsigned long apiVersion, const char *internalName, PlayerModelManagerModelType modelType) {
    if (apiVersion > PMM_API_VERSION) {
        Logger_printWarning("PlayerModelManager_registerModel: Model requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return 0;
    }

    if (sIsAPILocked) {
        Logger_printWarning("PlayerModelManager_registerModel: Models can only be registered during a onRegisterModels callback.");
        return 0;
    }

    if (!isStrValid("PlayerModelManager_registerModel", internalName, PMM_MAX_INTERNAL_NAME_LENGTH)) {
        return 0;
    }

    Link_CustomModelCategory cat = getCategoryFromModelType(modelType);

    if (!isValidCategory(cat)) {
        Logger_printError("PlayerModelManager_registerFormModel: Passed in unsupported PlayerModelManagerModelType %d to PlayerModelManager_registerModel.", modelType);
        return 0;
    }

    PlayerModelManagerHandle h = CMEM_createMemoryHandle(modelType, YAZMTCore_Utils_StrDup(internalName));

    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return 0;
    }

    if (modelType == PMM_MODEL_TYPE_ADULT) {
        ModelEntry_setFlags(entry, MODELENTRY_FLAG_IS_ADULT);
    }

    Logger_printVerbose("PlayerModelManager: Registered new model '%s' with model type %d", ModelEntry_getInternalName(entry), modelType);

    return h;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayName(PlayerModelManagerHandle h, const char *displayName) {
    if (!isStrValid("PlayerModelManager_setDisplayName", displayName, PMM_MAX_DISPLAY_NAME_LENGTH)) {
        return 0;
    }

    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setDisplayName");

    if (!entry) {
        return false;
    }

    ModelEntry_setDisplayName(entry, displayName);

    Logger_printVerbose("PlayerModelManager: Setting display name of handle with internal name '%s' to '%s'", ModelEntry_getInternalName(entry), displayName);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setAuthor(PlayerModelManagerHandle h, const char *author) {
    if (!isStrValid("PlayerModelManager_setAuthor", author, PMM_MAX_AUTHOR_NAME_LENGTH)) {
        return false;
    }

    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setAuthor");

    if (!entry) {
        return false;
    }

    ModelEntry_setAuthorName(entry, author);

    Logger_printVerbose("PlayerModelManager: Setting author of handle with internal name '%s' to '%s'", ModelEntry_getInternalName(entry), author);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setFlags");

    if (!entry) {
        return false;
    }

    ModelEntry_setFlags(entry, flags);

    Logger_printVerbose("PlayerModelManager: Setting flags 0x%llX of handle with internal name '%s'", flags, ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearFlags");

    if (!entry) {
        return false;
    }

    ModelEntry_unsetFlags(entry, flags);

    Logger_printVerbose("PlayerModelManager: Clearing flags 0x%llX of handle with internal name '%s'", flags, ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearAllFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearAllFlags");

    if (!entry) {
        return false;
    }

    ModelEntry_unsetAllFlags(entry);

    Logger_printVerbose("PlayerModelManager: Clearing all flags of handle with internal name '%s'", ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayList(PlayerModelManagerHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        Logger_printError("PlayerModelManager: Invalid display list ID passed in to PlayerModelManager_setDisplayList.");
        return false;
    }

    ModelEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return false;
    }

    if (ModelEntry_setDisplayList(entry, dlId, dl)) {
        refreshProxyDLIfEntryLoaded(entry, dlId);
        Logger_printVerbose("PlayerModelManager: Setting DL id %d on handle with internal name '%s' to 0x%X", dlId, ModelEntry_getInternalName(entry), dl);
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setMatrix(PlayerModelManagerHandle h, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        Logger_printError("PlayerModelManager: Invalid matrix ID passed in to PlayerModelManager_setMtx.");
        return false;
    }

    ModelEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_setMtx");

    if (!entry) {
        return false;
    }

    if (ModelEntry_setMatrix(entry, mtxId, matrix)) {
        refreshProxyMtxIfEntryLoaded(entry, mtxId);
        Logger_printVerbose("PlayerModelManager: Copying matrix with id %d at 0x%X into handle with internal name '%s'", mtxId, matrix, ModelEntry_getInternalName(entry));
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_addHandleToPack(PlayerModelManagerHandle h, PlayerModelManagerHandle toAdd) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_addHandleToPack");

    if (!entry) {
        return false;
    }

    if (!isPackCategory(ModelEntry_getCategory(entry))) {
        Logger_printError("PlayerModelManager_addHandleToPack: Non-model pack passed into first arg of PlayerModelManager_addHandleToPack (Category was %d)", ModelEntry_getCategory(entry));
        return false;
    }

    ModelEntry *entryToAdd = getEntryOrPrintErrLocked(toAdd, "PlayerModelManager_addHandleToPack");

    if (!entryToAdd) {
        return false;
    }

    // cast relies on first member of ModelEntryPack being a ModelEntry
    ModelEntryPack *packEntry = (ModelEntryPack *)entry;

    if (!ModelEntryPack_addEntryToPack(packEntry, entryToAdd)) {
        Logger_printError("PlayerModelManager_addHandleToPack: Could not add handle! Handle with internal name '%s' is already a handle inside '%s' (Would cause circular reference)!", ModelEntry_getInternalName(entry), ModelEntry_getInternalName(entryToAdd));
        return false;
    }

    Logger_printVerbose("PlayerModelManager: Adding handle with internal name '%s' to model pack with internal name '%s'", ModelEntry_getInternalName(entry), ModelEntry_getInternalName(entryToAdd), ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setCallback(PlayerModelManagerHandle h, PlayerModelManagerEventHandler *callback, void *userdata) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setCallback");

    if (!entry) {
        return false;
    }

    ModelEntry_setCallback(entry, callback, userdata);

    Logger_printVerbose("PlayerModelManager: Setting handle with internal name '%s' to have callback function 0x%X with user data 0x%X", ModelEntry_getInternalName(entry), callback, userdata);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, "PlayerModelManager_setSkeleton");

    if (!entryForm) {
        return false;
    }

    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    ModelEntryForm_setSkeleton(entryForm, skel);

    if (skel) {
        StandardLimb **limbs = (StandardLimb **)skel->sh.segment;

#define SET_LIMB_DL(pLimb, entryDL)                                                                                                          \
    {                                                                                                                                        \
        if (!ModelEntry_getDisplayList(entry, entryDL))                                                                         \
            ModelEntry_setDisplayList(entry, entryDL, (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gEmptyDL); \
        refreshProxyDLIfEntryLoaded(entry, entryDL);                                                                            \
    }                                                                                                                                        \
    (void)0

        SET_LIMB_DL(PLAYER_LIMB_WAIST, LINK_DL_WAIST);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_THIGH, LINK_DL_RTHIGH);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_SHIN, LINK_DL_RSHIN);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_FOOT, LINK_DL_RFOOT);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_THIGH, LINK_DL_LTHIGH);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_SHIN, LINK_DL_LSHIN);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_FOOT, LINK_DL_LFOOT);
        SET_LIMB_DL(PLAYER_LIMB_HEAD, LINK_DL_HEAD);
        SET_LIMB_DL(PLAYER_LIMB_HAT, LINK_DL_HAT);
        SET_LIMB_DL(PLAYER_LIMB_COLLAR, LINK_DL_COLLAR);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_SHOULDER, LINK_DL_LSHOULDER);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_FOREARM, LINK_DL_LFOREARM);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_HAND, LINK_DL_LHAND);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_SHOULDER, LINK_DL_RSHOULDER);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_FOREARM, LINK_DL_RFOREARM);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_HAND, LINK_DL_RHAND);
        SET_LIMB_DL(PLAYER_LIMB_SHEATH, LINK_DL_SHEATH_NONE);
        SET_LIMB_DL(PLAYER_LIMB_TORSO, LINK_DL_TORSO);
    }

#undef SET_LIMB_DL

    refreshProxyIfEntryLoaded(entry);

    Logger_printVerbose("PlayerModelManager: Setting skeleton of handle with internal name '%s' to 0x%X", ModelEntry_getInternalName(entry), skel);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setShieldingSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, "PlayerModelManager_setShieldingSkeleton");
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    if (!entry) {
        return false;
    }

    ModelEntryForm_setShieldingSkeleton(entryForm, skel);

    if (skel) {
        if (skel->sh.limbCount != PLAYER_BODY_SHIELD_LIMB_COUNT) {
            Logger_printError("PlayerModelManager_setShieldingSkeleton: Skeleton with incorrect limb count passed in to PlayerModelManager_setShieldingSkeleton");
            return false;
        }

        StandardLimb **limbs = (StandardLimb **)skel->sh.segment;

#define SET_SHIELDING_LIMB_DL(pLimb, entryDL)                                                                                                \
    {                                                                                                                                        \
        if (!ModelEntry_getDisplayList(entry, entryDL))                                                                         \
            ModelEntry_setDisplayList(entry, entryDL, (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gEmptyDL); \
        refreshProxyDLIfEntryLoaded(entry, entryDL);                                                                            \
    }                                                                                                                                        \
    (void)0

        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_BODY, LINK_DL_BODY_SHIELD_BODY);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_HEAD, LINK_DL_BODY_SHIELD_HEAD);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
    }

#undef SET_SHIELDING_LIMB_DL

    refreshProxyIfEntryLoaded(entry);

    Logger_printVerbose("PlayerModelManager: Setting shielding skeleton of handle with internal name '%s' to 0x%X", ModelEntry_getInternalName(entry), skel);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setEyesTextures(PlayerModelManagerHandle h, TexturePtr eyesTextures[]) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, "PlayerModelManager_setEyesTextures");

    if (!entryForm) {
        return false;
    }

    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        ModelEntryForm_setEyesTexture(entryForm, eyesTextures[i], i);
    }

    refreshProxyEyesTexturesIfEntryLoaded(entry);

    Logger_printVerbose("PlayerModelManager: Setting eyes flipbook textures of handle with internal name '%s'", ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setMouthTextures(PlayerModelManagerHandle h, TexturePtr mouthTextures[]) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, "PlayerModelManager_setMouthTextures");

    if (!entryForm) {
        return false;
    }

    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        ModelEntryForm_setMouthTexture(entryForm, mouthTextures[i], i);
    }

    refreshProxyMouthTexturesIfEntryLoaded(entry);

    Logger_printVerbose("PlayerModelManager: Setting mouth flipbook textures of handle with internal name '%s'", ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT Gfx *PlayerModelManager_getFormDisplayList(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dlId) {
    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("PlayerModelManager_getFormDisplayList: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return NULL;
    }

    if (form >= PLAYER_FORM_MAX) {
        Logger_printError("PlayerModelManager_getFormDisplayList: Mod requesting invalid player form %d!", form);
        return NULL;
    }

    if (dlId >= LINK_DL_MAX) {
        Logger_printError("PlayerModelManager_getFormDisplayList: Mod requesting display list ID %d!", dlId);
        return NULL;
    }

    return &gLinkFormProxies[form].displayLists[dlId];
}

RECOMP_EXPORT bool PlayerModelManager_overrideVanillaDisplayList(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dlId, Gfx *dl) {
    if (sIsAPILocked) {
        Logger_printError("PlayerModelManager: %s called while API locked. "
                          "Please only call these functions during a onRegisterModels callback.",
                          "PlayerModelManager_overrideVanillaDisplayList");

        return false;
    }

    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("PlayerModelManager_overrideVanillaDisplayList: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return false;
    }

    if (form >= PLAYER_FORM_MAX) {
        Logger_printError("PlayerModelManager_overrideVanillaDisplayList: Mod requesting invalid player form %d!", form);
        return false;
    }

    if (dlId >= LINK_DL_MAX) {
        Logger_printError("PlayerModelManager_overrideVanillaDisplayList: Mod requesting invalid display list ID %d! (ID too high!)", form);
        return false;
    }

    if (dlId >= LINK_DL_SWORD1 && dlId <= LINK_DL_CENTER_FLOWER_PROPELLER_CLOSED) {
        Logger_printError("PlayerModelManager_overrideVanillaDisplayList: Mod requesting invalid display list ID %d! (ID was a shim!)", form);
        return false;
    }

    gLinkFormProxies[form].vanilla.models[dlId] = dl;
    requestRefreshFormProxyDL(&gLinkFormProxies[form], dlId);

    Logger_printVerbose("PlayerModelManager_overrideVanillaDisplayList: Overriding vanilla DL with ID %d for form %d with new DL at 0x%X.", dlId, form, dl);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_overrideVanillaMatrix(unsigned long apiVersion, PlayerTransformation form, Link_EquipmentMatrix mtxId, Mtx *mtx) {
    if (sIsAPILocked) {
        Logger_printError("PlayerModelManager: %s called while API locked. "
                          "Please only call these functions during a onRegisterModels callback.",
                          "PlayerModelManager_overrideVanillaMatrix");

        return false;
    }

    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("PlayerModelManager_overrideVanillaMatrix: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return false;
    }

    if (form >= PLAYER_FORM_MAX) {
        Logger_printError("PlayerModelManager_overrideVanillaMatrix: Mod requesting invalid player form %d!", form);
        return false;
    }

    if (mtxId >= LINK_EQUIP_MATRIX_MAX) {
        Logger_printError("PlayerModelManager_overrideVanillaMatrix: Mod requesting invalid matrix ID %d! (ID too high!)", form);
        return false;
    }

    gLinkFormProxies[form].vanilla.equipMtx[mtxId] = mtx;
    requestRefreshFormProxyMtx(&gLinkFormProxies[form], mtxId);

    Logger_printVerbose("PlayerModelManager_overrideVanillaMatrix: Copying matrix at 0x%X to with ID %d for form %d.");

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_isApplied(PlayerModelManagerHandle h) {
    ModelEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_isApplied");

    if (!entry) {
        return false;
    }

    if (ModelEntry_getType(entry) == PMM_MODEL_TYPE_NONE) {
        return false;
    }

    return CMEM_getCurrentEntry(ModelEntry_getCategory(entry)) == entry;
}

RECOMP_EXPORT void PlayerModelManager_requestOverrideTunicColor(u8 r, u8 g, u8 b, u8 a) {
    setTunicColor(r, g, b, a);
}

RECOMP_EXPORT void PlayerModelManager_requestOverrideFormTunicColor(PlayerTransformation form, u8 r, u8 g, u8 b, u8 a) {
    setFormTunicColor(form, r, g, b, a);
}

RECOMP_EXPORT bool PlayerModelManager_isCustomModelApplied(PlayerTransformation form) {
    Link_CustomModelCategory cat;

    switch (form) {
        case PLAYER_FORM_FIERCE_DEITY:
            cat = LINK_CMC_FIERCE_DEITY;
            break;

        case PLAYER_FORM_GORON:
            cat = LINK_CMC_GORON;
            break;

        case PLAYER_FORM_ZORA:
            cat = LINK_CMC_ZORA;
            break;

        case PLAYER_FORM_DEKU:
            cat = LINK_CMC_DEKU;
            break;

        case PLAYER_FORM_HUMAN:
            cat = LINK_CMC_HUMAN;
            break;

        default:
            return false;
            break;
    }

    return !!CMEM_getCurrentEntry(cat);
}

RECOMP_DECLARE_EVENT(onRegisterModels());
RECOMP_DECLARE_EVENT(onReady());

static bool sIsCMEMReady = false;
static bool sIsModelReplacerReady = false;
static bool sIsGlobalObjectsReady = false;
static bool sIsModelsRegistered = false;

RECOMP_DECLARE_EVENT(_internal_onFinishedRegisterModels());

void doRegisterModels() {
    if (sIsModelsRegistered) {
        return;
    }

    if (!sIsCMEMReady || (!sIsModelReplacerReady && MRC_isMRCEnabled()) || !sIsGlobalObjectsReady) {
        return;
    }

    sIsModelsRegistered = true;

    sIsAPILocked = false;
    Logger_printInfo("PlayerModelManager: Registering models...");
    onRegisterModels();
    sIsAPILocked = true;
    _internal_onFinishedRegisterModels();
    Logger_printInfo("PlayerModelManager: Finished registering models...");
    onReady();
    Logger_printInfo("PlayerModelManager: Ready");
}

RECOMP_CALLBACK(".", _internal_onReadyCMEM)
void doRegisterModelsCMEMReady() {
    sIsCMEMReady = true;
    Logger_printVerbose("PlayerModelManager: CustomModelEntryManager is ready.");
    doRegisterModels();
}

MODEL_REPLACER_CALLBACK_ON_REGISTER_REPLACERS
void doRegisterModelsModelReplacerReady() {
    sIsModelReplacerReady = true;
    Logger_printVerbose("PlayerModelManager: ModelReplacer compatibility is ready.");
    doRegisterModels();
}

GLOBAL_OBJECTS_CALLBACK_ON_READY
void doRegisterModelsGlobalObjectsReady() {
    sIsGlobalObjectsReady = true;
    Logger_printVerbose("PlayerModelManager: GlobalObjects is ready.");
    doRegisterModels();
}
