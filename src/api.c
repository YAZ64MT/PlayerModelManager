#include "global.h"
#include "custommodelentrymanager.h"
#include "recomputils.h"
#include "playermodelmanager.h"
#include "model_common.h"
#include "defines_modelinfo.h"

const size_t
    MAX_INTERNAL_NAME_LENGTH = INTERNAL_NAME_MAX_LENGTH,
    MAX_AUTHOR_NAME_LENGTH = 128,
    MAX_DISPLAY_NAME_LENGTH = 64;

PlayerTransformation getPlayerFormFromModelType(CustomModelType t) {
    switch (t) {
        case CUSTOM_MODEL_TYPE_CHILD:
        case CUSTOM_MODEL_TYPE_ADULT:
            return PLAYER_FORM_HUMAN;
            break;
        
        case CUSTOM_MODEL_TYPE_DEKU:
            return PLAYER_FORM_DEKU;
            break;

        case CUSTOM_MODEL_TYPE_GORON:
            return PLAYER_FORM_GORON;
            break;

        case CUSTOM_MODEL_TYPE_ZORA:
            return PLAYER_FORM_ZORA;
            break;

        case CUSTOM_MODEL_TYPE_FIERCE_DEITY:
            return PLAYER_FORM_FIERCE_DEITY;
            break;

        default:
            recomp_printf("PlayerModelManager: Unknown player form passed into getPlayerFormFromType\n");
            break;
    }

    return 0;
}

bool isEntryLoaded(CustomModelMemoryEntry *entry) {
    return entry &&
           entry->modelEntry.type != CUSTOM_MODEL_TYPE_NONE &&
           CMEM_getCurrentEntry(getPlayerFormFromModelType(entry->modelEntry.type)) == (void *)entry;
}

void refreshProxyIfEntryLoaded(CustomModelMemoryEntry *entry) {
    if (isEntryLoaded(entry)) {
        requestRefreshFormProxy(getPlayerFormFromModelType(entry->modelEntry.type));
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

CustomModelMemoryEntry *getEntryOrPrintErr(ZPlayerModelHandle h, const char *funcName) {
    CustomModelMemoryEntry *entry = CMEM_getMemoryEntry(h);

    if (!entry) {
        recomp_printf("PlayerModelManager: Invalid handle passed in to %s.\n", funcName);
    }

    return entry;
}

CustomModelMemoryEntry *getEntryOrPrintErrLocked(ZPlayerModelHandle h, const char *funcName) {
    if (sIsAPILocked) {
        recomp_printf(
            "PlayerModelManager: %s called while API locked. "
            "Please only call these functions during a ZPlayerModels_onRegisterModels callback.\n",
            funcName);

        return NULL;
    }

    return getEntryOrPrintErr(h, funcName);
}

#define CURRENT_API_VERSION 1UL

RECOMP_EXPORT ZPlayerModelHandle ZPlayerModelManager_registerPlayerModel(unsigned long apiVersion, char *internalName, CustomModelType modelType) {

    if (apiVersion > CURRENT_API_VERSION) {
        recomp_printf("ZPlayerModelManager_registerPlayerModel: Model requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return 0;
    }
    
    if (sIsAPILocked) {
        recomp_printf("ZPlayerModelManager_registerPlayerModel: Models can only be registered during a ZPlayerModels_onRegisterModels callback.\n");
        return 0;
    }

    if (!isStrValid("ZPlayerModelManager_registerPlayerModel", internalName, MAX_INTERNAL_NAME_LENGTH)) {
        return 0;
    }

    PlayerTransformation form;

    switch (modelType) {
        case CUSTOM_MODEL_TYPE_ADULT:
        case CUSTOM_MODEL_TYPE_CHILD:
            form = PLAYER_FORM_HUMAN;
            break;

        default:
            recomp_printf("ZPlayerModelManager_registerPlayerModel: Passed in unsupported ZPlayerModelManager_ModelType to ZPlayerModelManager_registerModel.\n");
            return 0;
            break;
    }

    ZPlayerModelHandle h = CMEM_createMemoryHandle(form);

    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_registerModel");

    if (!entry) {
        return 0;
    }

    entry->modelEntry.internalName = internalName;

    if (modelType == CUSTOM_MODEL_TYPE_ADULT) {
        entry->modelEntry.flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;
    }

    return h;
}

RECOMP_EXPORT bool ZPlayerModelManager_setDisplayName(ZPlayerModelHandle h, char *displayName) {
    if (!isStrValid("ZPlayerModelManager_setDisplayName", displayName, MAX_DISPLAY_NAME_LENGTH)) {
        return 0;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_setDisplayName");

    if (!entry) {
        return false;
    }

    entry->modelEntry.displayName = displayName;

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_setAuthor(ZPlayerModelHandle h, char *author) {
    if (!isStrValid("ZPlayerModelManager_setAuthor", author, MAX_AUTHOR_NAME_LENGTH)) {
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_setAuthor");

    if (!entry) {
        return false;
    }

    entry->modelEntry.authorName = author;

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_setFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_setFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags |= flags;

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_clearFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_clearFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags &= ~flags;

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_clearAllFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_clearAllFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags = 0;

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_setDL(ZPlayerModelHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        recomp_printf("PlayerModelManager: Invalid display list ID passed in to ZPlayerModelManager_setDL.\n");
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModelManager_registerModel");

    if (!entry) {
        return false;
    }

    entry->displayListPtrs[dlId] = dl;

    refreshProxyIfEntryLoaded(entry);

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_setMtx(ZPlayerModelHandle h, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        recomp_printf("PlayerModelManager: Invalid matrix ID passed in to ZPlayerModelManager_setMtx.\n");
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModelManager_setMtx");

    if (!entry) {
        return false;
    }

    entry->matrixPtrs[mtxId] = matrix;

    refreshProxyIfEntryLoaded(entry);

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_setLoadCallback(ZPlayerModelHandle h, void (*onModelLoad)(void *), void *userdata) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_setLoadCallback");

    if (!entry) {
        return false;
    }

    entry->modelEntry.onModelLoad = onModelLoad;
    entry->modelEntry.onModelLoadData = userdata;

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_setUnloadCallback(ZPlayerModelHandle h, void (*onModelUnload)(void *), void *userdata) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_setUnloadCallback");

    if (!entry) {
        return false;
    }

    entry->modelEntry.onModelUnload = onModelUnload;
    entry->modelEntry.onModelUnloadData = userdata;

    return true;
}

#define SET_LIMB_DL(pLimb, entryDL)       \
    if (!entry->displayListPtrs[entryDL]) \
    entry->displayListPtrs[entryDL] = (limbs[pLimb - 1]->dLists[0]) ? (limbs[pLimb - 1]->dLists[0]) : gCallEmptyDisplayList

RECOMP_EXPORT bool ZPlayerModelManager_setSkeleton(ZPlayerModelHandle h, FlexSkeletonHeader *skel) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModelManager_setSkeleton");

    if (!entry) {
        return false;
    }

    entry->skel = skel;

    if (skel) {
        LodLimb **limbs = (LodLimb **)skel->sh.segment;

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

#undef GET_LIMB_DL

RECOMP_EXPORT bool ZPlayerModelManager_setEyesTextures(ZPlayerModelHandle h, TexturePtr eyesTextures[PLAYER_EYES_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErrLocked(h, "ZPlayerModelManager_setEyesTextures");

    if (!entry) {
        return false;
    }

    entry->eyesTex = eyesTextures;

    if (isEntryLoaded(entry) && GET_PLAYER_FORM == getPlayerFormFromModelType(entry->modelEntry.type)) {
        refreshFaceTextures();
    }

    return true;
}

RECOMP_EXPORT bool ZPlayerModelManager_setMouthTextures(ZPlayerModelHandle h, TexturePtr mouthTextures[PLAYER_MOUTH_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModelManager_setMouthTextures");

    if (!entry) {
        return false;
    }

    entry->mouthTex = mouthTextures;

    if (isEntryLoaded(entry) && GET_PLAYER_FORM == getPlayerFormFromModelType(entry->modelEntry.type)) {
        refreshFaceTextures();
    }

    return true;
}

RECOMP_EXPORT Gfx *ZPlayerModelManager_getDL(unsigned long apiVersion, PlayerTransformation form, Link_DisplayList dl) {
    if (apiVersion > CURRENT_API_VERSION) {
        recomp_printf("ZPlayerModelManager_getDL: Mod requesting unsupported API version %d! You may need to upgrade PlayerModelManager!\n");
        return NULL;
    }

    // TODO: remove this check when all forms supported
    if (form != PLAYER_FORM_HUMAN) {
        return NULL;
    }

    return &gLinkFormProxies[form].displayLists[dl];
}

RECOMP_DECLARE_EVENT(ZPlayerModelManager_onRegisterModels());

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onFinishedRegisterModels());

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyCMEM)
void doRegisterModels() {
    sIsAPILocked = false;
    ZPlayerModelManager_onRegisterModels();
    sIsAPILocked = true;
    PlayerModelManager_internal_onFinishedRegisterModels();
}
