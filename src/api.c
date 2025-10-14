#include "global.h"
#include "custommodelentrymanager.h"
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

static bool isEntryLoaded(ModelEntry *entry) {
    if (entry) {
        return CMEM_getCurrentEntry(getCategoryFromModelType(entry->type)) == entry;
    }

    return false;
}

static void refreshProxyIfEntryLoaded(ModelEntry *entry) {
    Link_CustomModelCategory cat = getCategoryFromModelType(entry->type);

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
        recomp_printf("%s: String passed in was NULL! Aborting!\n", callerName);
        return false;
    }

    bool isTooLong = isStrTooLong(strToVerify, maxLen);

    if (isTooLong) {
        recomp_printf("%s: String passed in was too long! Maximum length is %u!\n", callerName, maxLen);
        return false;
    }

    return true;
}

static bool sIsAPILocked = true;

static ModelEntry *getEntryOrPrintErr(PlayerModelManagerHandle h, const char *funcName) {
    ModelEntry *entry = CMEM_getEntry(h);

    if (!entry) {
        recomp_printf("PlayerModelManager: Invalid handle passed in to %s.\n", funcName);
    }

    return entry;
}

static ModelEntryForm *getFormEntryOrPrintErr(PlayerModelManagerHandle h, const char *funcName) {
    ModelEntry *entry = getEntryOrPrintErr(h, funcName);

    if (!entry) {
        return NULL;
    }

    if (!isFormCategory(getCategoryFromModelType(entry->type))) {
        recomp_printf("PlayerModelManager: Handle with internal name %s does not support the function %s\n", entry->internalName, funcName);
        return NULL;
    }

    return (ModelEntryForm *)entry;
}

static ModelEntry *getEntryOrPrintErrLocked(PlayerModelManagerHandle h, const char *funcName) {
    if (sIsAPILocked) {
        recomp_printf(
            "PlayerModelManager: %s called while API locked. "
            "Please only call these functions during a onRegisterModels callback.\n",
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
        recomp_printf("PlayerModelManager_registerModel: Model requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return 0;
    }

    if (sIsAPILocked) {
        recomp_printf("PlayerModelManager_registerModel: Models can only be registered during a onRegisterModels callback.\n");
        return 0;
    }

    if (!isStrValid("PlayerModelManager_registerModel", internalName, PMM_MAX_INTERNAL_NAME_LENGTH)) {
        return 0;
    }

    Link_CustomModelCategory cat = getCategoryFromModelType(modelType);

    if (!isValidCategory(cat)) {
        recomp_printf("PlayerModelManager_registerFormModel: Passed in unsupported PlayerModelManagerModelType to PlayerModelManager_registerModel.\n");
        return 0;
    }

    PlayerModelManagerHandle h = CMEM_createMemoryHandle(modelType, YAZMTCore_Utils_StrDup(internalName));

    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return 0;
    }

    if (modelType == PMM_MODEL_TYPE_ADULT) {
        entry->flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;
    }

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

    dupStrAndFreeOld(&entry->displayName, displayName);

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

    dupStrAndFreeOld(&entry->authorName, author);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setFlags");

    if (!entry) {
        return false;
    }

    entry->flags |= flags;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearFlags");

    if (!entry) {
        return false;
    }

    entry->flags &= ~flags;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearAllFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearAllFlags");

    if (!entry) {
        return false;
    }

    entry->flags = 0;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayList(PlayerModelManagerHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        recomp_printf("PlayerModelManager: Invalid display list ID passed in to PlayerModelManager_setDisplayList.\n");
        return false;
    }

    ModelEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return false;
    }

    if (entry->setDisplayList(entry, dlId, dl)) {
        refreshProxyDLIfEntryLoaded(entry, dlId);
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setMatrix(PlayerModelManagerHandle h, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        recomp_printf("PlayerModelManager: Invalid matrix ID passed in to PlayerModelManager_setMtx.\n");
        return false;
    }

    ModelEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_setMtx");

    if (!entry) {
        return false;
    }

    if (entry->setMatrix(entry, mtxId, matrix)) {
        refreshProxyMtxIfEntryLoaded(entry, mtxId);
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setCallback(PlayerModelManagerHandle h, PlayerModelManagerEventHandler *callback, void *userdata) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setCallback");

    if (!entry) {
        return false;
    }

    entry->callback = callback;

    entry->callbackData = userdata;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entry = getFormEntryOrPrintErr(h, "PlayerModelManager_setSkeleton");

    if (!entry) {
        return false;
    }

    entry->skel = skel;

    if (skel) {
        StandardLimb **limbs = (StandardLimb **)skel->sh.segment;

#define SET_LIMB_DL(pLimb, entryDL)                                                                                                          \
    {                                                                                                                                        \
        if (!ModelEntry_getDisplayList(&entry->modelEntry, entryDL))                                                                         \
            entry->modelEntry.setDisplayList(&entry->modelEntry, entryDL, (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gEmptyDL); \
        refreshProxyDLIfEntryLoaded(&entry->modelEntry, entryDL);                                                                            \
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

    refreshProxyIfEntryLoaded(&entry->modelEntry);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setShieldingSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entry = getFormEntryOrPrintErr(h, "PlayerModelManager_setShieldingSkeleton");

    if (!entry) {
        return false;
    }

    entry->shieldingSkel = skel;

    if (skel) {
        if (skel->sh.limbCount != PLAYER_BODY_SHIELD_LIMB_COUNT) {
            recomp_printf("Skeleton with incorrect limb count passed in to PlayerModelManager_setShieldingSkeleton");
            return false;
        }

        StandardLimb **limbs = (StandardLimb **)skel->sh.segment;

#define SET_SHIELDING_LIMB_DL(pLimb, entryDL)                                                                                                \
    {                                                                                                                                        \
        if (!ModelEntry_getDisplayList(&entry->modelEntry, entryDL))                                                                         \
            entry->modelEntry.setDisplayList(&entry->modelEntry, entryDL, (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gEmptyDL); \
        refreshProxyDLIfEntryLoaded(&entry->modelEntry, entryDL);                                                                            \
    }                                                                                                                                        \
    (void)0

        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_BODY, LINK_DL_BODY_SHIELD_BODY);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_HEAD, LINK_DL_BODY_SHIELD_HEAD);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
    }

#undef SET_SHIELDING_LIMB_DL

    refreshProxyIfEntryLoaded(&entry->modelEntry);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setEyesTextures(PlayerModelManagerHandle h, TexturePtr eyesTextures[]) {
    ModelEntryForm *entry = getFormEntryOrPrintErr(h, "PlayerModelManager_setEyesTextures");

    if (!entry) {
        return false;
    }

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        entry->eyesTex[i] = eyesTextures[i];
    }

    refreshProxyEyesTexturesIfEntryLoaded(&entry->modelEntry);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setMouthTextures(PlayerModelManagerHandle h, TexturePtr mouthTextures[]) {
    ModelEntryForm *entry = getFormEntryOrPrintErr(h, "PlayerModelManager_setMouthTextures");

    if (!entry) {
        return false;
    }

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        entry->mouthTex[i] = mouthTextures[i];
    }

    refreshProxyMouthTexturesIfEntryLoaded(&entry->modelEntry);

    return true;
}

RECOMP_EXPORT Gfx *PlayerModelManager_getFormDisplayList(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dlId) {
    if (apiVersion > PMM_API_VERSION) {
        recomp_printf("PlayerModelManager_getFormDisplayList: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return NULL;
    }

    if (form >= PLAYER_FORM_MAX) {
        recomp_printf("PlayerModelManager_getFormDisplayList: Mod requesting invalid player form %d!\n", form);
        return NULL;
    }

    if (dlId >= LINK_DL_MAX) {
        recomp_printf("PlayerModelManager_getFormDisplayList: Mod requesting display list ID %d!\n", dlId);
        return NULL;
    }

    return &gLinkFormProxies[form].displayLists[dlId];
}

RECOMP_EXPORT bool PlayerModelManager_overrideVanillaDisplayList(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dlId, Gfx *dl) {
    if (sIsAPILocked) {
        recomp_printf("PlayerModelManager: %s called while API locked. "
                      "Please only call these functions during a onRegisterModels callback.\n",
                      "PlayerModelManager_overrideVanillaDisplayList");

        return false;
    }

    if (apiVersion > PMM_API_VERSION) {
        recomp_printf("PlayerModelManager_overrideVanillaDisplayList: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return false;
    }

    if (form >= PLAYER_FORM_MAX) {
        recomp_printf("PlayerModelManager_overrideVanillaDisplayList: Mod requesting invalid player form %d!\n", form);
        return false;
    }

    if (dlId >= LINK_DL_MAX) {
        recomp_printf("PlayerModelManager_overrideVanillaDisplayList: Mod requesting invalid display list ID %d! (ID too high!)\n", form);
        return false;
    }

    if (dlId >= PMM_DL_SHIM_SWORD1 && dlId <= PMM_DL_SHIM_CENTER_FLOWER_PROPELLER_CLOSED) {
        recomp_printf("PlayerModelManager_overrideVanillaDisplayList: Mod requesting invalid display list ID %d! (ID was a shim!)\n", form);
    }

    gLinkFormProxies[form].vanilla.models[dlId] = dl;
    requestRefreshFormProxyDL(&gLinkFormProxies[form], dlId);
    return true;
}

RECOMP_EXPORT bool PlayerModelManager_overrideVanillaMatrix(unsigned long apiVersion, PlayerTransformation form, Link_EquipmentMatrix mtxId, Mtx *mtx) {
    if (sIsAPILocked) {
        recomp_printf("PlayerModelManager: %s called while API locked. "
                      "Please only call these functions during a onRegisterModels callback.\n",
                      "PlayerModelManager_overrideVanillaMatrix");

        return false;
    }

    if (apiVersion > PMM_API_VERSION) {
        recomp_printf("PlayerModelManager_overrideVanillaMatrix: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return false;
    }

    if (form >= PLAYER_FORM_MAX) {
        recomp_printf("PlayerModelManager_overrideVanillaMatrix: Mod requesting invalid player form %d!\n", form);
        return false;
    }

    if (mtxId >= LINK_DL_MAX) {
        recomp_printf("PlayerModelManager_overrideVanillaMatrix: Mod requesting invalid matrix ID %d! (ID too high!)\n", form);
        return false;
    }

    gLinkFormProxies[form].vanilla.equipMtx[mtxId] = mtx;
    requestRefreshFormProxyMtx(&gLinkFormProxies[form], mtxId);
    return true;
}

RECOMP_EXPORT bool PlayerModelManager_isApplied(PlayerModelManagerHandle h) {
    ModelEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_isApplied");

    if (!entry) {
        return false;
    }

    if (entry->type == PMM_MODEL_TYPE_NONE) {
        return false;
    }

    return CMEM_getCurrentEntry(getCategoryFromModelType(entry->type)) == entry;
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
    onRegisterModels();
    sIsAPILocked = true;
    _internal_onFinishedRegisterModels();
    onReady();
}

RECOMP_CALLBACK(".", _internal_onReadyCMEM)
void doRegisterModelsCMEMReady() {
    sIsCMEMReady = true;
    doRegisterModels();
}

MODEL_REPLACER_CALLBACK_ON_REGISTER_REPLACERS
void doRegisterModelsModelReplacerReady() {
    sIsModelReplacerReady = true;
    doRegisterModels();
}

GLOBAL_OBJECTS_CALLBACK_ON_READY
void doRegisterModelsGlobalObjectsReady() {
    sIsGlobalObjectsReady = true;
    doRegisterModels();
}
