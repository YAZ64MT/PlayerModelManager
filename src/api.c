#include "global.h"
#include "modelentrymanager.h"
#include "recomputils.h"
#include "modelmatrixids.h"
#include "playermodelmanager_api.h"
#include "yazmtcorelib_api.h"
#include "modelreplacer_api.h"
#include "globalobjects_api.h"
#include "apilocal.h"
#include "logger.h"
#include "playerproxy.h"
#include "playerproxymanager.h"
#include "modelentry.h"
#include "formproxy.h"
#include "customdls.h"
#include "equipmentbuiltin.h"

static Gfx sBelowV3OcarinaHandDL[] = {
    gsSPBranchList(gLinkHumanRightHandOcarinaDL),
};

static Gfx sBelowV3OcarinaDL[] = {
    gsSPBranchList(gLinkHumanOcarinaDL),
};

static Gfx sBelowV3FirstPersonRightForearmDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sBelowV3FirstPersonRightHandDL[] = {
    gsSPBranchList(gLinkHumanFirstPersonArmDL),
};

static Gfx sBelowV3FirstPersonHookshotDL[] = {
    gsSPBranchList(gLinkHumanFirstPersonHookshotDL),
};

static Gfx sBelowV3FirstPersonBowDL[] = {
    gsSPBranchList(gLinkHumanFirstPersonBowDL),
};

static bool isEntryLoaded(ModelEntry *entry) {
    if (entry) {
        return ModelEntryManager_getCurrentEntry(ModelEntry_getCategory(entry)) == entry;
    }

    return false;
}

static void refreshProxyIfEntryLoaded(ModelEntry *entry) {
    Link_CustomModelCategory cat = ModelEntry_getCategory(entry);

    if (isEntryLoaded(entry)) {
        ModelEntryManager_reapplyEntry(cat);
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
    ModelEntry *entry = ModelEntryManager_getEntry(h);

    if (!entry) {
        Logger_printError("Invalid handle passed in to %s.", funcName);
    }

    return entry;
}

static ModelEntry *getEntryOrPrintErrLocked(PlayerModelManagerHandle h, const char *funcName) {
    if (sIsAPILocked) {
        Logger_printWarning(
            "%s called while API locked. "
            "Please only call these functions during a onRegisterModels callback.",
            funcName);

        return NULL;
    }

    return getEntryOrPrintErr(h, funcName);
}

static ModelEntryForm *getFormEntryOrPrintErr(PlayerModelManagerHandle h, const char *funcName) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, funcName);

    if (!entry) {
        return NULL;
    }

    if (!isFormCategory(ModelEntry_getCategory(entry))) {
        Logger_printWarning("Handle with internal name '%s' does not support the function '%s'", ModelEntry_getInternalName(entry), funcName);
        return NULL;
    }

    return (ModelEntryForm *)entry;
}

RECOMP_EXPORT PlayerModelManagerHandle PlayerModelManager_registerModel(unsigned long apiVersion, const char *internalName, PlayerModelManagerModelType modelType) {
    if (apiVersion > PMM_API_VERSION) {
        Logger_printWarning("Model requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return 0;
    }

    if (sIsAPILocked) {
        Logger_printWarning("Models can only be registered during a onRegisterModels callback.");
        return 0;
    }

    if (!isStrValid(__func__, internalName, PMM_MAX_INTERNAL_NAME_LENGTH)) {
        return 0;
    }

    Link_CustomModelCategory cat = getCategoryFromModelType(modelType);

    if (!isValidCategory(cat)) {
        Logger_printError("Passed in unsupported PlayerModelManagerModelType %d.", modelType);
        return 0;
    }

    PlayerModelManagerHandle h = ModelEntryManager_createMemoryHandle(modelType, YAZMTCore_Utils_StrDup(internalName));

    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return 0;
    }

    if (modelType == PMM_MODEL_TYPE_ADULT) {
        ModelEntry_setFlags(entry, MODELENTRY_FLAG_IS_ADULT);
    }

    ModelEntry_setDisplayName(entry, internalName);

    Logger_printVerbose("Registered new model '%s' with model type %d", ModelEntry_getInternalName(entry), modelType);

    if (apiVersion < 3UL && (modelType == PMM_MODEL_TYPE_CHILD || modelType == PMM_MODEL_TYPE_ADULT)) {
        ModelEntry_setDisplayList(entry, LINK_DL_OPT_FPS_RFOREARM, sBelowV3FirstPersonRightForearmDL);
        ModelEntry_setDisplayList(entry, LINK_DL_OPT_FPS_RHAND, sBelowV3FirstPersonRightHandDL);
        ModelEntry_setDisplayList(entry, LINK_DL_FPS_HOOKSHOT, sBelowV3FirstPersonHookshotDL);
        ModelEntry_setDisplayList(entry, LINK_DL_FPS_BOW, sBelowV3FirstPersonBowDL);
    }

    return h;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayName(PlayerModelManagerHandle h, const char *displayName) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    ModelEntry_setDisplayName(entry, displayName);

    Logger_printVerbose("Setting display name of handle with internal name '%s' to '%s'", ModelEntry_getInternalName(entry), displayName);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setAuthor(PlayerModelManagerHandle h, const char *author) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    ModelEntry_setAuthorName(entry, author);

    Logger_printVerbose("Setting author of handle with internal name '%s' to '%s'", ModelEntry_getInternalName(entry), author);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    ModelEntry_setFlags(entry, flags);

    Logger_printVerbose("Setting flags 0x%llX of handle with internal name '%s'", flags, ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    ModelEntry_unsetFlags(entry, flags);

    Logger_printVerbose("Clearing flags 0x%llX of handle with internal name '%s'", flags, ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearAllFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    ModelEntry_unsetAllFlags(entry);

    Logger_printVerbose("Clearing all flags of handle with internal name '%s'", ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayList(PlayerModelManagerHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        Logger_printError("Invalid display list ID passed in.");
        return false;
    }

    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    if (ModelEntry_setDisplayList(entry, dlId, dl)) {
        refreshProxyDLIfEntryLoaded(entry, dlId);
        Logger_printVerbose("Setting DL id %d on handle with internal name '%s' to 0x%X", dlId, ModelEntry_getInternalName(entry), dl);
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayListForModelType(PlayerModelManagerHandle h, PlayerModelManagerModelType type, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        Logger_printError("Invalid display list ID passed in.");
        return false;
    }

    if (type >= PMM_MODEL_TYPE_MAX || type < 0) {
        Logger_printError("Invalid model type passed in.");
        return false;
    }

    ModelEntry *entryx = getEntryOrPrintErrLocked(h, __func__);

    if (!entryx) {
        return false;
    }

    if (!isEquipmentCategory(ModelEntry_getCategory(entryx))) {
        Logger_printError("Handle with internal name %s is not an equipment handle!", ModelEntry_getInternalName(entryx));
        return false;
    }

    ModelEntryEquipment *entry = (ModelEntryEquipment *)entryx;

    if (ModelEntryEquipment_setDisplayListForModelType(entry, type, dlId, dl)) {
        refreshProxyDLIfEntryLoaded(entryx, dlId);
        Logger_printVerbose("Setting DL id %d for type %d on handle with internal name '%s' to 0x%X", dlId, type, ModelEntry_getInternalName(entryx), dl);
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayListForModelTypeArray(PlayerModelManagerHandle h, PlayerModelManagerModelType types[], size_t n, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        Logger_printError("Invalid display list ID passed in.");
        return false;
    }

    if (n <= 0) {
        Logger_printError("Invalid array size %d passed in.", n);
        return false;
    }

    for (size_t i = 0; i < n; ++i) {
        PlayerModelManagerModelType type = types[i];
        if (type >= PMM_MODEL_TYPE_MAX || type < 0) {
            Logger_printError("Invalid model type %d passed in.", type);
            return false;
        }
    }

    ModelEntry *entryx = getEntryOrPrintErrLocked(h, __func__);

    if (!entryx) {
        return false;
    }

    if (!isEquipmentCategory(ModelEntry_getCategory(entryx))) {
        Logger_printError("Handle with internal name %s is not an equipment handle!", ModelEntry_getInternalName(entryx));
        return false;
    }

    ModelEntryEquipment *entry = (ModelEntryEquipment *)entryx;

    for (size_t i = 0; i < n; ++i) {
        PlayerModelManagerModelType type = types[i];
        if (ModelEntryEquipment_setDisplayListForModelType(entry, type, dlId, dl)) {
            refreshProxyDLIfEntryLoaded(entryx, dlId);
            Logger_printVerbose("Setting DL id %d for type %d on handle with internal name '%s' to 0x%X", dlId, type, ModelEntry_getInternalName(entryx), dl);
        }
    }

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setMatrix(PlayerModelManagerHandle h, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        Logger_printError("Invalid matrix ID passed in.");
        return false;
    }

    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    if (ModelEntry_setMatrix(entry, mtxId, matrix)) {
        refreshProxyMtxIfEntryLoaded(entry, mtxId);
        Logger_printVerbose("Setting matrix with id %d at 0x%X into handle with internal name '%s'", mtxId, matrix, ModelEntry_getInternalName(entry));
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setMatrixForModelType(PlayerModelManagerHandle h, PlayerModelManagerModelType type, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        Logger_printError("Invalid matrix ID passed in.");
        return false;
    }

    if (type >= PMM_MODEL_TYPE_MAX || type < 0) {
        Logger_printCurrentFuncAndLine("Invalid model type passed in.");
    }

    ModelEntry *entryx = getEntryOrPrintErrLocked(h, __func__);

    if (!entryx) {
        return false;
    }

    if (!isEquipmentCategory(ModelEntry_getCategory(entryx))) {
        Logger_printError("Handle with internal name %s is not an equipment handle!", ModelEntry_getInternalName(entryx));
        return false;
    }

    ModelEntryEquipment *entry = (ModelEntryEquipment *)entryx;

    if (ModelEntryEquipment_setMatrixForModelType(entry, type, mtxId, matrix)) {
        refreshProxyMtxIfEntryLoaded(entryx, mtxId);
        Logger_printVerbose("Setting matrix with id %d for type %d at 0x%X into handle with internal name '%s'", mtxId, type, matrix, ModelEntry_getInternalName(entryx));
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setMatrixForModelTypeArray(PlayerModelManagerHandle h, PlayerModelManagerModelType types[], size_t n, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        Logger_printError("Invalid matrix ID passed in.");
        return false;
    }

    if (n <= 0) {
        Logger_printError("Invalid array size %d passed in.", n);
        return false;
    }

    for (size_t i = 0; i < n; ++i) {
        PlayerModelManagerModelType type = types[i];
        if (type >= PMM_MODEL_TYPE_MAX || type < 0) {
            Logger_printCurrentFuncAndLine("Invalid model type passed in.");
        }
    }

    ModelEntry *entryx = getEntryOrPrintErrLocked(h, __func__);

    if (!entryx) {
        return false;
    }

    if (!isEquipmentCategory(ModelEntry_getCategory(entryx))) {
        Logger_printError("Handle with internal name %s is not an equipment handle!", ModelEntry_getInternalName(entryx));
        return false;
    }

    ModelEntryEquipment *entry = (ModelEntryEquipment *)entryx;

    for (size_t i = 0; i < n; ++i) {
        PlayerModelManagerModelType type = types[i];
        if (ModelEntryEquipment_setMatrixForModelType(entry, type, mtxId, matrix)) {
            refreshProxyMtxIfEntryLoaded(entryx, mtxId);
            Logger_printVerbose("Setting matrix with id %d for type %d at 0x%X into handle with internal name '%s'", mtxId, type, matrix, ModelEntry_getInternalName(entryx));
        }
    }

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_addHandleToPack(PlayerModelManagerHandle h, PlayerModelManagerHandle toAdd) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    if (!isPackCategory(ModelEntry_getCategory(entry))) {
        Logger_printError("Non-model pack passed into first arg of %s (Category was %d)", __func__, ModelEntry_getCategory(entry));
        return false;
    }

    ModelEntry *entryToAdd = getEntryOrPrintErrLocked(toAdd, __func__);

    if (!entryToAdd) {
        return false;
    }

    // cast relies on first member of ModelEntryPack being a ModelEntry
    ModelEntryPack *packEntry = (ModelEntryPack *)entry;

    if (!ModelEntryPack_addEntryToPack(packEntry, entryToAdd)) {
        Logger_printError("Could not add handle! Handle with internal name '%s' is already a handle inside '%s' (Would cause circular reference)!", ModelEntry_getInternalName(entry), ModelEntry_getInternalName(entryToAdd));
        return false;
    }

    Logger_printVerbose("Adding handle with internal name '%s' to model pack with internal name '%s'", ModelEntry_getInternalName(entry), ModelEntry_getInternalName(entryToAdd), ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setCallback(PlayerModelManagerHandle h, PlayerModelManagerEventHandler *callback, void *userdata) {
    ModelEntry *entry = getEntryOrPrintErrLocked(h, __func__);

    if (!entry) {
        return false;
    }

    ModelEntry_setCallback(entry, callback, userdata);

    Logger_printVerbose("Setting handle with internal name '%s' to have callback function 0x%X with user data 0x%X", ModelEntry_getInternalName(entry), callback, userdata);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, __func__);

    if (!entryForm) {
        return false;
    }

    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    ModelEntryForm_setSkeleton(entryForm, skel);

    if (skel) {
        StandardLimb **limbs = (StandardLimb **)skel->sh.segment;

#define SET_LIMB_DL(pLimb, entryDL)                                                                                      \
    {                                                                                                                    \
        if (!ModelEntry_getDisplayList(entry, entryDL))                                                                  \
            ModelEntry_setDisplayList(entry, entryDL, (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gEmptyDL); \
        refreshProxyDLIfEntryLoaded(entry, entryDL);                                                                     \
    }                                                                                                                    \
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

    Logger_printVerbose("Setting skeleton of handle with internal name '%s' to 0x%X", ModelEntry_getInternalName(entry), skel);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setShieldingSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, __func__);
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    if (!entry) {
        return false;
    }

    ModelEntryForm_setShieldingSkeleton(entryForm, skel);

    if (skel) {
        if (skel->sh.limbCount != PLAYER_BODY_SHIELD_LIMB_COUNT) {
            Logger_printError("Skeleton with incorrect limb count passed in to %s", __func__);
            return false;
        }

        StandardLimb **limbs = (StandardLimb **)skel->sh.segment;

#define SET_SHIELDING_LIMB_DL(pLimb, entryDL)                                                                            \
    {                                                                                                                    \
        if (!ModelEntry_getDisplayList(entry, entryDL))                                                                  \
            ModelEntry_setDisplayList(entry, entryDL, (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gEmptyDL); \
        refreshProxyDLIfEntryLoaded(entry, entryDL);                                                                     \
    }                                                                                                                    \
    (void)0

        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_BODY, LINK_DL_BODY_SHIELD_BODY);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_HEAD, LINK_DL_BODY_SHIELD_HEAD);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
    }

#undef SET_SHIELDING_LIMB_DL

    refreshProxyIfEntryLoaded(entry);

    Logger_printVerbose("Setting shielding skeleton of handle with internal name '%s' to 0x%X", ModelEntry_getInternalName(entry), skel);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setEyesTextures(PlayerModelManagerHandle h, TexturePtr eyesTextures[]) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, __func__);

    if (!entryForm) {
        return false;
    }

    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        ModelEntryForm_setEyesTexture(entryForm, eyesTextures[i], i);
    }

    refreshProxyEyesTexturesIfEntryLoaded(entry);

    Logger_printVerbose("Setting eyes flipbook textures of handle with internal name '%s'", ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setMouthTextures(PlayerModelManagerHandle h, TexturePtr mouthTextures[]) {
    ModelEntryForm *entryForm = getFormEntryOrPrintErr(h, __func__);

    if (!entryForm) {
        return false;
    }

    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        ModelEntryForm_setMouthTexture(entryForm, mouthTextures[i], i);
    }

    refreshProxyMouthTexturesIfEntryLoaded(entry);

    Logger_printVerbose("Setting mouth flipbook textures of handle with internal name '%s'", ModelEntry_getInternalName(entry));

    return true;
}

RECOMP_EXPORT Gfx *PlayerModelManager_getFormDisplayList(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dlId) {
    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return NULL;
    }

    if (form >= PLAYER_FORM_MAX) {
        Logger_printError("Mod requesting invalid player form %d!", form);
        return NULL;
    }

    if (dlId >= LINK_DL_MAX) {
        Logger_printError("Mod requesting display list ID %d!", dlId);
        return NULL;
    }

    FormProxyId fpId;
    if (PlayerProxy_getProxyIdFromForm(form, &fpId)) {
        FormProxy *fp = PlayerProxy_getFormProxy(gPlayer1Proxy, fpId);

        if (fp) {
            return FormProxy_getDL(fp, dlId);
        }
    }

    return NULL;
}

RECOMP_EXPORT bool PlayerModelManager_overrideVanillaDisplayList(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dlId, Gfx *dl) {
    if (sIsAPILocked) {
        Logger_printError("%s called while API locked. "
                          "Please only call these functions during a onRegisterModels callback.",
                          __func__);

        return false;
    }

    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return false;
    }

    if (form >= PLAYER_FORM_MAX) {
        Logger_printError("Mod requesting invalid player form %d!", form);
        return false;
    }

    if (dlId >= LINK_DL_MAX) {
        Logger_printError("Mod requesting invalid display list ID %d! (ID too high!)", form);
        return false;
    }

    if (dlId >= LINK_DL_SWORD1 && dlId <= LINK_DL_CENTER_FLOWER_PROPELLER_CLOSED) {
        Logger_printError("Mod requesting invalid display list ID %d! (ID was a shim!)", form);
        return false;
    }

    FormProxyId fpId;
    if (PlayerProxy_getProxyIdFromForm(form, &fpId)) {
        FormProxy *fp = PlayerProxy_getFormProxy(gPlayer1Proxy, fpId);

        if (fp) {
            ModelInfo *fallbackModelInfo = FormProxy_getFallbackModelInfo(fp);

            if (fallbackModelInfo) {
                ModelInfo_setGfxOverride(fallbackModelInfo, dlId, dl);
                PlayerProxyManager_refreshAll();

                // preserve model behavior of old API versions
                if (form == PLAYER_FORM_HUMAN) {
                    switch (dlId) {
                        case LINK_DL_FPS_BOW:
                            gSPBranchList(sBelowV3FirstPersonBowDL, dl);
                            break;

                        case LINK_DL_FPS_HOOKSHOT:
                            gSPBranchList(sBelowV3FirstPersonHookshotDL, dl);
                            break;

                        case LINK_DL_OPT_FPS_RFOREARM:
                            gSPBranchList(sBelowV3FirstPersonRightForearmDL, dl);
                            break;

                        case LINK_DL_OPT_FPS_RHAND:
                            gSPBranchList(sBelowV3FirstPersonRightHandDL, dl);
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }

    Logger_printVerbose("Overriding vanilla DL with ID %d for form %d with new DL at 0x%X.", dlId, form, dl);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_overrideVanillaMatrix(unsigned long apiVersion, PlayerTransformation form, Link_EquipmentMatrix mtxId, Mtx *mtx) {
    if (sIsAPILocked) {
        Logger_printError("%s called while API locked. "
                          "Please only call these functions during a onRegisterModels callback.",
                          __func__);

        return false;
    }

    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!");
        return false;
    }

    if (form >= PLAYER_FORM_MAX) {
        Logger_printError("Mod requesting invalid player form %d!", form);
        return false;
    }

    if (mtxId >= LINK_EQUIP_MATRIX_MAX) {
        Logger_printError("Mod requesting invalid matrix ID %d! (ID too high!)", form);
        return false;
    }

    FormProxyId fpId;
    if (PlayerProxy_getProxyIdFromForm(form, &fpId)) {
        FormProxy *fp = PlayerProxy_getFormProxy(gPlayer1Proxy, fpId);

        if (fp) {
            ModelInfo *fallbackModelInfo = FormProxy_getFallbackModelInfo(fp);

            if (fallbackModelInfo) {
                ModelInfo_setMtxOverride(fallbackModelInfo, mtxId, mtx);
                PlayerProxyManager_refreshAll();
            }
        }
    }

    Logger_printVerbose("Copying matrix at 0x%X to with ID %d for form %d.");

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_isApplied(PlayerModelManagerHandle h) {
    ModelEntry *entry = getEntryOrPrintErr(h, __func__);

    if (!entry) {
        return false;
    }

    if (ModelEntry_getType(entry) == PMM_MODEL_TYPE_NONE) {
        return false;
    }

    return ModelEntryManager_getCurrentEntry(ModelEntry_getCategory(entry)) == entry;
}

RECOMP_EXPORT void PlayerModelManager_requestOverrideTunicColor(u8 r, u8 g, u8 b, u8 a) {
    PlayerProxy_requestTunicColorOverride(gPlayer1Proxy, (Color_RGBA8){r, g, b, a});
}

RECOMP_EXPORT void PlayerModelManager_requestOverrideFormTunicColor(PlayerTransformation form, u8 r, u8 g, u8 b, u8 a) {
    FormProxyId fpId;
    if (PlayerProxy_getProxyIdFromForm(form, &fpId)) {
        FormProxy *fp = PlayerProxy_getFormProxy(gPlayer1Proxy, fpId);

        if (fp) {
            FormProxy_requestTunicColorOverride(fp, (Color_RGBA8){r, g, b, a});
        }
    }
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

    return !!ModelEntryManager_getCurrentEntry(cat);
}

#define DECLARE_BUILT_IN_DL(id, dl) [id] = dl
static Gfx *sBuiltInDLs[PMM_BUILT_IN_DL_MAX] = {
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_KOKIRI_HILT, gCallHumanKokiriSwordHiltDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_KOKIRI_HILT_AS_ADULT, gHumanAdultKokiriSwordHiltDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_KOKIRI_BLADE, gCallHumanKokiriSwordBladeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_KOKIRI_BLADE_AS_ADULT, gHumanAdultKokiriSwordBladeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_KOKIRI_SHEATH, gCallHumanKokiriSwordSheathDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_KOKIRI_SHEATH_AS_ADULT, gHumanAdultKokiriSwordSheathDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_RAZOR_HILT, gCallHumanRazorSwordHiltDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_RAZOR_HILT_AS_ADULT, gHumanAdultRazorSwordHiltDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_RAZOR_BLADE, gCallHumanRazorSwordBladeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_RAZOR_BLADE_AS_ADULT, gHumanAdultRazorSwordBladeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_RAZOR_SHEATH, gCallHumanRazorSwordSheathDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_RAZOR_SHEATH_AS_ADULT, gHumanAdultRazorSwordSheathDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GILDED_HILT, gCallHumanGildedSwordHiltDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GILDED_HILT_AS_ADULT, gHumanAdultGildedSwordHiltDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GILDED_BLADE, gCallHumanGildedSwordBladeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GILDED_BLADE_AS_ADULT, gHumanAdultGildedSwordBladeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GILDED_SHEATH, gCallHumanGildedSwordSheathDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GILDED_SHEATH_AS_ADULT, gHumanAdultGildedSwordSheathDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GREAT_FAIRY, gCallHumanGreatFairySwordDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SWORD_GREAT_FAIRY_AS_ADULT, gHumanAdultGreatFairySwordDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_FIERCE_DEITY_SWORD, gCallFierceDeitySwordDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_FIERCE_DEITY_SWORD_AS_CHILD, gFierceDeityChildSwordDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_HERO, gCallHumanHeroShieldDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_HERO_AS_ADULT, gHumanAdultHeroShieldDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_MIRROR, gCallHumanMirrorShieldDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_MIRROR_AS_ADULT, gHumanAdultMirrorShieldDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_MIRROR_RAY, gCallHumanMirrorShieldRayDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_MIRROR_RAY_AS_ADULT, gHumanAdultMirrorShieldRayDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_MIRROR_RAY_BEAM, gCallHumanMirrorShieldRayBeamDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_SHIELD_MIRROR_RAY_BEAM_AS_ADULT, gHumanAdultMirrorShieldRayBeamDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_HOOKSHOT, gCallHumanHookshotDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_HOOKSHOT_AS_ADULT, gHumanAdultHookshotDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_FPS_HOOKSHOT, gCallHumanHookshotFirstPersonDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_FPS_HOOKSHOT_AS_ADULT, gHumanAdultHookshotFirstPersonDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_HOOKSHOT_HOOK, gCallHumanHookshotHookDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_HOOKSHOT_HOOK_AS_ADULT, gHumanAdultHookshotHookDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_HOOKSHOT_CHAIN, gCallHumanHookshotChainDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_HOOKSHOT_CHAIN_AS_ADULT, gHumanAdultHookshotChainDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_HOOKSHOT_RETICLE, gCallHumanHookshotReticleDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_BOW, gCallHumanBowDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_BOW_AS_ADULT, gHumanAdultBowDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_FPS_BOW, gCallHumanBowFirstPersonDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_FPS_BOW_AS_ADULT, gHumanAdultBowFirstPersonDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_BOW_STRING, gCallHumanBowStringDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_BOW_STRING_AS_ADULT, gHumanAdultBowDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_ARROW, gCallHumanArrowDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_MM_BOTTLE_GLASS, gCallMMBottleGlassDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_MM_BOTTLE_FILLING, gCallMMBottleContentsDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_OCARINA_TIME, gCallHumanOcarinaTimeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_OCARINA_TIME_AS_ADULT, gHumanAdultOcarinaTimeDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_DEKU_STICK, gCallHumanDekuStickDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_DEKU_PIPE_MOUTH, gCallDekuPipesMouthDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_DEKU_PIPE_RIGHT, gCallDekuPipesRightDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_DEKU_PIPE_UP, gCallDekuPipesUpDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_DEKU_PIPE_DOWN, gCallDekuPipesDownDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_DEKU_PIPE_LEFT, gCallDekuPipesLeftDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_DEKU_PIPE_A, gCallDekuPipesADL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_GORON_DRUM_STRAP, gCallGoronDrumStrapDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_GORON_DRUM_RIGHT, gCallGoronDrumRightDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_GORON_DRUM_UP, gCallGoronDrumUpDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_GORON_DRUM_DOWN, gCallGoronDrumDownDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_GORON_DRUM_LEFT, gCallGoronDrumLeftDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_GORON_DRUM_A, gCallGoronDrumADL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_ZORA_GUITAR, gCallZoraGuitarDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_TRUTH, gCallHumanMaskTruthDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_KAFEIS_MASK, gCallHumanMaskKafeiDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_ALL_NIGHT, gCallHumanMaskAllNightDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_BUNNY, gCallHumanMaskBunnyHoodDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_KEATON, gCallHumanMaskKeatonDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_GARO, gCallHumanMaskGaroDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_ROMANI, gCallHumanMaskRomaniDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_CIRCUS_LEADER, gCallHumanMaskCircusLeaderDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_COUPLE, gCallHumanMaskCoupleDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_POSTMAN, gCallHumanMaskPostmanDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_GREAT_FAIRY, gCallHumanMaskGreatFairyDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_GIBDO, gCallHumanMaskGibdoDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_GIBDO, gCallHumanMaskDonGeroDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_KAMARO, gCallHumanMaskKamaroDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_CAPTAIN, gCallHumanMaskCaptainDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_STONE, gCallHumanMaskStoneDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_BREMEN, gCallHumanMaskBremenDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_BLAST, gCallHumanMaskBlastDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_BLAST_COOLING_DOWN, gCallHumanMaskBlastCooldownDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_SCENTS, gCallHumanMaskScentsDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_GIANT, gCallHumanMaskGiantDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_DEKU, gCallHumanMaskDekuDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_GORON, gCallHumanMaskGoronDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_ZORA, gCallHumanMaskZoraDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_FIERCE_DEITY, gCallHumanMaskFierceDeityDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_DEKU_SCREAM, gCallHumanMaskDekuScreamDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_GORON, gCallHumanMaskGoronScreamDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_ZORA, gCallHumanMaskZoraScreamDL),
    DECLARE_BUILT_IN_DL(PMM_BUILT_IN_DL_HUMAN_MASK_FIERCE_DEITY, gCallHumanMaskFierceDeityScreamDL),
};
#undef DECLARE_BUILT_IN_DL

RECOMP_EXPORT Gfx *PlayerModelManager_getBuiltInDisplayList(unsigned long apiVersion, PlayerModelManagerBuiltInDLId id) {
    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("PlayerModelManager is out of date! Expected API version <= %d but received version %d!", PMM_API_VERSION, apiVersion);
        return NULL;
    }

    if (id < 0 || id >= ARRAY_COUNT(sBuiltInDLs)) {
        Logger_printError("Invalid id passed in.");
        return NULL;
    }

    return sBuiltInDLs[id];
}

#define DECLARE_BUILT_IN_MTX(id, mtx) [id] = mtx
static Mtx *sBuiltIMtxs[PMM_BUILT_IN_MTX_MAX] = {
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_KOKIRI_BACK, &gHumanKokiriSwordHiltBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_KOKIRI_BACK_AS_ADULT, &gHumanAdultKokiriSwordHiltBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_RAZOR_BACK, &gHumanRazorSwordHiltBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_RAZOR_BACK_AS_ADULT, &gHumanAdultRazorSwordHiltBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_GILDED_BACK, &gHumanGildedSwordHiltBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_GILDED_BACK_AS_ADULT, &gHumanAdultGildedSwordHiltBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_HERO_BACK, &gHumanHeroShieldBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_HERO_BACK_AS_ADULT, &gHumanAdultHeroShieldBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_MIRROR_BACK, &gHumanMirrorShieldBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_MIRROR_BACK_AS_ADULT, &gHumanAdultMirrorShieldBackMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_KOKIRI_HILT_AS_ADULT, &gHumanAdultKokiriSwordHiltResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_KOKIRI_BLADE_AS_ADULT, &gHumanAdultKokiriSwordBladeResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_KOKIRI_SHEATH_AS_ADULT, &gHumanAdultKokiriSwordSheathResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_RAZOR_HILT_AS_ADULT, &gHumanAdultRazorSwordHiltResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_RAZOR_BLADE_AS_ADULT, &gHumanAdultRazorSwordBladeResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_RAZOR_SHEATH_AS_ADULT, &gHumanAdultRazorSwordSheathResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_GILDED_HILT_AS_ADULT, &gHumanAdultGildedSwordHiltResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_GILDED_BLADE_AS_ADULT, &gHumanAdultGildedSwordBladeResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_GILDED_SHEATH_AS_ADULT, &gHumanAdultGildedSwordSheathResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SWORD_GREAT_FAIRY_AS_ADULT, &gHumanAdultGreatFairySwordResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_FIERCE_DEITY_SWORD_AS_CHILD, &gFierceDeityChildSwordResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_HERO_AS_ADULT, &gHumanAdultHeroShieldResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_MIRROR_AS_ADULT, &gHumanAdultMirrorShieldResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_MIRROR_RAY_AS_ADULT, &gHumanAdultMirrorShieldRayResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_SHIELD_MIRROR_RAY_BEAM_AS_ADULT, &gHumanAdultMirrorShieldRayBeamResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_HOOKSHOT_AS_ADULT, &gHumanAdultHookshotResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_FPS_HOOKSHOT_AS_ADULT, &gHumanAdultHookshotFirstPersonResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_HOOKSHOT_HOOK_AS_ADULT, &gHumanAdultHookshotHookResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_HOOKSHOT_CHAIN_AS_ADULT, &gHumanAdultHookshotChainResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_BOW_AS_ADULT, &gHumanAdultBowResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_FPS_BOW_AS_ADULT, &gHumanAdultBowFirstPersonResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_BOW_STRING_AS_ADULT, &gHumanAdultBowStringResizerMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_OCARINA_TIME_AS_ADULT, &gHumanAdultOcarinaTimeResizerMtx),

    // these two will need special handling when OoT recomp comes around
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_HOOKSHOT_CHAIN_AND_HOOK, &gIdentityMtx),
    DECLARE_BUILT_IN_MTX(PMM_BUILT_IN_MTX_HUMAN_HOOKSHOT_CHAIN_AND_HOOK_AS_ADULT, &gHumanAdultHookshotHookAndChainMtx),
};
#undef DECLARE_BUILT_IN_MTX

RECOMP_EXPORT Mtx *PlayerModelManager_getBuiltInMatrix(unsigned long apiVersion, PlayerModelManagerBuiltInMtxId id) {
    if (apiVersion > PMM_API_VERSION) {
        Logger_printError("PlayerModelManager is out of date! Expected API version <= %d but received version %d!", PMM_API_VERSION, apiVersion);
        return NULL;
    }

    if (id < 0 || id >= ARRAY_COUNT(sBuiltIMtxs)) {
        Logger_printError("Invalid id passed in.");
        return NULL;
    }

    return sBuiltIMtxs[id];
}

RECOMP_DECLARE_EVENT(onRegisterModels());
RECOMP_DECLARE_EVENT(onReady());

static bool sIsCMEMReady = false;
static bool sIsGlobalObjectsReady = false;
static bool sIsModelsRegistered = false;

RECOMP_DECLARE_EVENT(_internal_onFinishedRegisterModels());

void doRegisterModels() {
    if (sIsModelsRegistered) {
        return;
    }

    if (!sIsCMEMReady || !sIsGlobalObjectsReady) {
        return;
    }

    sIsModelsRegistered = true;

    sIsAPILocked = false;
    Logger_printInfo("Registering models...");
    onRegisterModels();
    sIsAPILocked = true;
    _internal_onFinishedRegisterModels();
    Logger_printInfo("Finished registering models.");
    onReady();
    Logger_printInfo("Ready!");
}

RECOMP_CALLBACK(".", _internal_postInitHashObjects) void doRegisterModelsCMEMReady() {
    sIsCMEMReady = true;
    Logger_printVerbose("ModelEntryManager is ready.");
    doRegisterModels();
}

MODEL_REPLACER_CALLBACK_ON_REGISTER_REPLACERS
void doRegisterModelsModelReplacerReady() {
    Logger_printVerbose("ModelReplacer compatibility is ready.");
    doRegisterModels();
}

GLOBAL_OBJECTS_CALLBACK_ON_READY
void doRegisterModelsGlobalObjectsReady() {
    sIsGlobalObjectsReady = true;
    Logger_printVerbose("GlobalObjects is ready.");
    doRegisterModels();
}
