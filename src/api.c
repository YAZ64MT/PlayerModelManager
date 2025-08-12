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

#define ENTRY_FORM(entry) (getFormFromModelType(entry->modelEntry.type))
#define ENTRY_LOADED_PROXY(entry) (isEntryLoaded(entry) ? &gLinkFormProxies[ENTRY_FORM(entry)] : NULL)

bool isEntryLoaded(ModelEntryForm *entry) {
    if (entry) {
        PlayerTransformation form = getFormFromModelType(entry->modelEntry.type);

        if (form < PLAYER_FORM_MAX) {
            return CMEM_getCurrentEntry(form) == (void *)entry;
        }
    }

    return false;
}

void refreshProxyIfEntryLoaded(ModelEntryForm *entry) {
    Link_FormProxy *fp = ENTRY_LOADED_PROXY(entry);

    if (fp) {
        // Don't need to verify ENTRY_FORM return because fp will be NULL if invalid
        CMEM_reapplyEntry(ENTRY_FORM(entry));
        requestRefreshFormProxy(fp);
    }
}

void refreshProxyDLIfEntryLoaded(ModelEntryForm *entry, Link_DisplayList dlId) {
    Link_FormProxy *fp = ENTRY_LOADED_PROXY(entry);

    if (fp) {
        fp->current.models[dlId] = ModelEntry_getDisplayList(&entry->modelEntry, dlId);
        requestRefreshFormProxyDL(&gLinkFormProxies[ENTRY_FORM(entry)], dlId);
    }
}

void refreshProxyMtxIfEntryLoaded(ModelEntryForm *entry, Link_EquipmentMatrix mtxId) {
    Link_FormProxy *fp = ENTRY_LOADED_PROXY(entry);

    if (fp) {
        fp->current.equipMtx[mtxId] = ModelEntry_getMatrix(&entry->modelEntry, mtxId);
        requestRefreshFormProxyMtx(&gLinkFormProxies[getFormFromModelType(entry->modelEntry.type)], mtxId);
    }
}

void refreshProxyEyesTexturesIfEntryLoaded(ModelEntryForm *entry) {
    Link_FormProxy *fp = ENTRY_LOADED_PROXY(entry);

    if (fp) {
        for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
            fp->current.eyesTextures[i] = entry->eyesTex[i];
        }
    }
}

void refreshProxyMouthTexturesIfEntryLoaded(ModelEntryForm *entry) {
    Link_FormProxy *fp = ENTRY_LOADED_PROXY(entry);

    if (fp) {
        for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
            fp->current.mouthTextures[i] = entry->mouthTex[i];
        }
    }
}

bool isStrTooLong(const char *s, size_t maxLen) {
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

bool isStrValid(const char *callerName, const char *strToVerify, size_t maxLen) {
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

ModelEntryForm *getEntryOrPrintErr(PlayerModelManagerHandle h, const char *funcName) {
    ModelEntryForm *entry = CMEM_getFormEntry(h);

    if (!entry) {
        recomp_printf("PlayerModelManager: Invalid handle passed in to %s.\n", funcName);
    }

    return entry;
}

ModelEntryForm *getEntryOrPrintErrLocked(PlayerModelManagerHandle h, const char *funcName) {
    if (sIsAPILocked) {
        recomp_printf(
            "PlayerModelManager: %s called while API locked. "
            "Please only call these functions during a onRegisterModels callback.\n",
            funcName);

        return NULL;
    }

    return getEntryOrPrintErr(h, funcName);
}

void dupStrAndFreeOld(char **dest, const char *src) {
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

    PlayerTransformation form = getFormFromModelType(modelType);

    if (form >= PLAYER_FORM_MAX) {
        recomp_printf("PlayerModelManager_registerFormModel: Passed in unsupported PlayerModelManagerModelType to PlayerModelManager_registerModel.\n");
        return 0;
    }

    char *internalNameCopy = YAZMTCore_Utils_StrDup(internalName);
    PlayerModelManagerHandle h = CMEM_createMemoryHandle(form, internalNameCopy);

    ModelEntryForm *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return 0;
    }

    if (modelType == PMM_MODEL_TYPE_ADULT) {
        entry->modelEntry.flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;
    }

    entry->modelEntry.type = modelType;

    return h;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayName(PlayerModelManagerHandle h, const char *displayName) {
    if (!isStrValid("PlayerModelManager_setDisplayName", displayName, PMM_MAX_DISPLAY_NAME_LENGTH)) {
        return 0;
    }

    ModelEntryForm *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setDisplayName");

    if (!entry) {
        return false;
    }

    dupStrAndFreeOld(&entry->modelEntry.displayName, displayName);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setAuthor(PlayerModelManagerHandle h, const char *author) {
    if (!isStrValid("PlayerModelManager_setAuthor", author, PMM_MAX_AUTHOR_NAME_LENGTH)) {
        return false;
    }

    ModelEntryForm *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setAuthor");

    if (!entry) {
        return false;
    }

    dupStrAndFreeOld(&entry->modelEntry.authorName, author);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntryForm *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags |= flags;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntryForm *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags &= ~flags;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearAllFlags(PlayerModelManagerHandle h, u64 flags) {
    ModelEntryForm *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearAllFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags = 0;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayList(PlayerModelManagerHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        recomp_printf("PlayerModelManager: Invalid display list ID passed in to PlayerModelManager_setDisplayList.\n");
        return false;
    }

    ModelEntryForm *entry = getEntryOrPrintErr(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return false;
    }

    if (entry->modelEntry.setDisplayList(&entry->modelEntry, dlId, dl)) {
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

    ModelEntryForm *entry = getEntryOrPrintErr(h, "PlayerModelManager_setMtx");

    if (!entry) {
        return false;
    }

    if (entry->modelEntry.setMatrix(&entry->modelEntry, mtxId, matrix)) {
        refreshProxyMtxIfEntryLoaded(entry, mtxId);
        return true;
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_setCallback(PlayerModelManagerHandle h, PlayerModelManagerEventHandler *callback, void *userdata) {
    ModelEntryForm *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setLoadCallback");

    if (!entry) {
        return false;
    }

    entry->modelEntry.callback = callback;

    entry->modelEntry.callbackData = userdata;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entry = getEntryOrPrintErr(h, "PlayerModelManager_setSkeleton");

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
        refreshProxyDLIfEntryLoaded(entry, entryDL);                                                                                         \
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

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setShieldingSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel) {
    ModelEntryForm *entry = getEntryOrPrintErr(h, "PlayerModelManager_setShieldingSkeleton");

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
    }                                                                                                                                        \
    (void)0

        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_BODY, LINK_DL_BODY_SHIELD_BODY);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_HEAD, LINK_DL_BODY_SHIELD_HEAD);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
    }

#undef SET_SHIELDING_LIMB_DL

    refreshProxyIfEntryLoaded(entry);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setEyesTextures(PlayerModelManagerHandle h, TexturePtr eyesTextures[]) {
    ModelEntryForm *entry = getEntryOrPrintErr(h, "PlayerModelManager_setEyesTextures");

    if (!entry) {
        return false;
    }

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        entry->eyesTex[i] = eyesTextures[i];
    }

    refreshProxyEyesTexturesIfEntryLoaded(entry);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setMouthTextures(PlayerModelManagerHandle h, TexturePtr mouthTextures[]) {
    ModelEntryForm *entry = getEntryOrPrintErr(h, "PlayerModelManager_setMouthTextures");

    if (!entry) {
        return false;
    }

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        entry->mouthTex[i] = mouthTextures[i];
    }

    refreshProxyMouthTexturesIfEntryLoaded(entry);

    return true;
}

RECOMP_EXPORT Gfx *PlayerModelManager_getFormDisplayList(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dl) {
    if (apiVersion > PMM_API_VERSION) {
        recomp_printf("PlayerModelManager_getFormDisplayList: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return NULL;
    }

    if (form >= PLAYER_FORM_MAX || dl >= LINK_DL_MAX) {
        return NULL;
    }

    return &gLinkFormProxies[form].displayLists[dl];
}

RECOMP_EXPORT bool PlayerModelManager_isApplied(PlayerModelManagerHandle h) {
    ModelEntryForm *entry = getEntryOrPrintErr(h, "PlayerModelManager_isApplied");

    if (!entry) {
        return false;
    }

    if (entry->modelEntry.type == PMM_MODEL_TYPE_NONE) {
        return false;
    }

    return CMEM_getCurrentEntry(getFormFromModelType(entry->modelEntry.type)) == &entry->modelEntry;
}

RECOMP_EXPORT void PlayerModelManager_requestOverrideTunicColor(u8 r, u8 g, u8 b, u8 a) {
    setTunicColor(r, g, b, a);
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
