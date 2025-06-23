#include "global.h"
#include "custommodelentrymanager.h"
#include "recomputils.h"
#include "playermodelmanager.h"
#include "model_common.h"
#include "defines_modelinfo.h"
#include "playermodelmanager_api.h"

const size_t
    MAX_INTERNAL_NAME_LENGTH = INTERNAL_NAME_MAX_LENGTH,
    MAX_AUTHOR_NAME_LENGTH = 128,
    MAX_DISPLAY_NAME_LENGTH = 64;

bool isEntryLoaded(CustomModelMemoryEntry *entry) {
    return entry &&
           entry->modelEntry.type != PMM_FORM_MODEL_TYPE_NONE &&
           CMEM_getCurrentEntry(getFormFromModelType(entry->modelEntry.type)) == (void *)entry;
}

void refreshProxyIfEntryLoaded(CustomModelMemoryEntry *entry) {
    if (isEntryLoaded(entry)) {
        requestRefreshFormProxy(&gLinkFormProxies[getFormFromModelType(entry->modelEntry.type)]);
    }
}

void refreshProxyDLIfEntryLoaded(CustomModelMemoryEntry *entry, Link_DisplayList dlId) {
    if (isEntryLoaded(entry)) {
        requestRefreshFormProxyDL(&gLinkFormProxies[getFormFromModelType(entry->modelEntry.type)], dlId);
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

CustomModelMemoryEntry *getEntryOrPrintErr(PlayerModelManagerFormHandle h, const char *funcName) {
    CustomModelMemoryEntry *entry = CMEM_getMemoryEntry(h);

    if (!entry) {
        recomp_printf("PlayerModelManager: Invalid handle passed in to %s.\n", funcName);
    }

    return entry;
}

CustomModelMemoryEntry *getEntryOrPrintErrLocked(PlayerModelManagerFormHandle h, const char *funcName) {
    if (sIsAPILocked) {
        recomp_printf(
            "PlayerModelManager: %s called while API locked. "
            "Please only call these functions during a onRegisterModels callback.\n",
            funcName);

        return NULL;
    }

    return getEntryOrPrintErr(h, funcName);
}

RECOMP_EXPORT PlayerModelManagerFormHandle PlayerModelManager_registerFormModel(unsigned long apiVersion, char *internalName, FormModelType modelType) {

    if (apiVersion > PMM_API_VERSION) {
        recomp_printf("PlayerModelManager_registerFormModel: Model requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return 0;
    }

    if (sIsAPILocked) {
        recomp_printf("PlayerModelManager_registerFormModel: Models can only be registered during a onRegisterModels callback.\n");
        return 0;
    }

    if (!isStrValid("PlayerModelManager_registerFormModel", internalName, MAX_INTERNAL_NAME_LENGTH)) {
        return 0;
    }

    if (modelType == PMM_FORM_MODEL_TYPE_NONE || modelType >= PMM_FORM_MODEL_TYPE_MAX) {
        recomp_printf("PlayerModelManager_registerFormModel: Passed in unsupported PlayerModelManager_FormModelType to PlayerModelManager_registerModel.\n");
        return 0;
    }

    PlayerTransformation form = getFormFromModelType(modelType);

    PlayerModelManagerFormHandle h = CMEM_createMemoryHandle(form);

    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return 0;
    }

    entry->modelEntry.internalName = internalName;

    if (modelType == PMM_FORM_MODEL_TYPE_ADULT) {
        entry->modelEntry.flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;
    }

    return h;
}

RECOMP_EXPORT bool PlayerModelManager_setDisplayName(PlayerModelManagerFormHandle h, char *displayName) {
    if (!isStrValid("PlayerModelManager_setDisplayName", displayName, MAX_DISPLAY_NAME_LENGTH)) {
        return 0;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setDisplayName");

    if (!entry) {
        return false;
    }

    entry->modelEntry.displayName = displayName;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setAuthor(PlayerModelManagerFormHandle h, char *author) {
    if (!isStrValid("PlayerModelManager_setAuthor", author, MAX_AUTHOR_NAME_LENGTH)) {
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setAuthor");

    if (!entry) {
        return false;
    }

    entry->modelEntry.authorName = author;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setFlags(PlayerModelManagerFormHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags |= flags;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearFlags(PlayerModelManagerFormHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags &= ~flags;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_clearAllFlags(PlayerModelManagerFormHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_clearAllFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags = 0;

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setDL(PlayerModelManagerFormHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        recomp_printf("PlayerModelManager: Invalid display list ID passed in to PlayerModelManager_setDL.\n");
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_registerModel");

    if (!entry) {
        return false;
    }

    entry->displayListPtrs[dlId] = dl;

    refreshProxyDLIfEntryLoaded(entry, dlId);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setMtx(PlayerModelManagerFormHandle h, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        recomp_printf("PlayerModelManager: Invalid matrix ID passed in to PlayerModelManager_setMtx.\n");
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_setMtx");

    if (!entry) {
        return false;
    }

    entry->matrixPtrs[mtxId] = matrix;

    refreshProxyIfEntryLoaded(entry);

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setCallback(PlayerModelManagerFormHandle h, PlayerModelManagerEventHandler *callback, void *userdata) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setLoadCallback");

    if (!entry) {
        return false;
    }

    entry->modelEntry.callback = callback;

    entry->modelEntry.callbackData = userdata;

    return true;
}

#define SET_LIMB_DL(pLimb, entryDL)       \
    if (!entry->displayListPtrs[entryDL]) \
    entry->displayListPtrs[entryDL] = (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gCallEmptyDisplayList

RECOMP_EXPORT bool PlayerModelManager_setSkeleton(PlayerModelManagerFormHandle h, FlexSkeletonHeader *skel) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_setSkeleton");

    if (!entry) {
        return false;
    }

    entry->skel = skel;

    if (skel) {
        StandardLimb **limbs = (StandardLimb **)skel->sh.segment;

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

    refreshProxyIfEntryLoaded(entry);

    return true;
}

#undef SET_LIMB_DL

RECOMP_EXPORT bool PlayerModelManager_setShieldingSkeleton(PlayerModelManagerFormHandle h, FlexSkeletonHeader *skel) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_setShieldingSkeleton");

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

#define SET_SHIELDING_LIMB_DL(pLimb, entryDL) \
    if (!entry->displayListPtrs[entryDL])     \
    entry->displayListPtrs[entryDL] = (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gCallEmptyDisplayList

        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_BODY, LINK_DL_BODY_SHIELD_BODY);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_HEAD, LINK_DL_BODY_SHIELD_HEAD);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
    }

#undef SET_SHIELDING_LIMB_DL

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setEyesTextures(PlayerModelManagerFormHandle h, TexturePtr eyesTextures[PLAYER_EYES_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "PlayerModelManager_setEyesTextures");

    if (!entry) {
        return false;
    }

    entry->eyesTex = eyesTextures;

    if (isEntryLoaded(entry) && GET_PLAYER_FORM == getFormFromModelType(entry->modelEntry.type)) {
        requestRefreshFaceTextures();
    }

    return true;
}

RECOMP_EXPORT bool PlayerModelManager_setMouthTextures(PlayerModelManagerFormHandle h, TexturePtr mouthTextures[PLAYER_MOUTH_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_setMouthTextures");

    if (!entry) {
        return false;
    }

    entry->mouthTex = mouthTextures;

    if (isEntryLoaded(entry) && GET_PLAYER_FORM == getFormFromModelType(entry->modelEntry.type)) {
        requestRefreshFaceTextures();
    }

    return true;
}

RECOMP_EXPORT Gfx *PlayerModelManager_getDL(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dl) {
    if (apiVersion > PMM_API_VERSION) {
        recomp_printf("PlayerModelManager_getDL: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return NULL;
    }

    // TODO: remove this check when all forms supported
    if (form != PLAYER_FORM_HUMAN) {
        return NULL;
    }

    return &gLinkFormProxies[form].displayLists[dl];
}

bool PlayerModelManager_isApplied(PlayerModelManagerFormHandle h) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "PlayerModelManager_isApplied");

    if (!entry) {
        return false;
    }

    if (entry->modelEntry.type == PMM_FORM_MODEL_TYPE_NONE) {
        return false;
    }

    return CMEM_getCurrentEntry(getFormFromModelType(entry->modelEntry.type)) == &entry->modelEntry;
}

RECOMP_DECLARE_EVENT(onRegisterModels());
RECOMP_DECLARE_EVENT(onReady());

RECOMP_DECLARE_EVENT(_internal_onFinishedRegisterModels());

RECOMP_CALLBACK(".", _internal_onReadyCMEM)
void doRegisterModels() {
    sIsAPILocked = false;
    onRegisterModels();
    sIsAPILocked = true;
    _internal_onFinishedRegisterModels();
    onReady();
}
