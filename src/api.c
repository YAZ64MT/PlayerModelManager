#include "global.h"
#include "custommodelentrymanager.h"
#include "recomputils.h"
#include "playermodelmanager.h"
#include "model_common.h"
#include "defines_modelinfo.h"

const size_t 
    MAX_INTERNAL_NAME_LENGTH = 512,
    MAX_AUTHOR_NAME_LENGTH = 128,
    MAX_DISPLAY_NAME_LENGTH = 64;


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
    if (sIsAPILocked) {
        recomp_printf(
            "PlayerModelManager: %s called while API locked. "
            "Please only call these functions during a ZPlayerModels_onRegisterModels callback.\n",
            funcName);
    }

    CustomModelMemoryEntry *entry = CMEM_getMemoryEntry(h);

    if (!entry) {
        recomp_printf("PlayerModelManager: Invalid handle passed in to %s.\n", funcName);
    }

    return entry;
}

RECOMP_EXPORT ZPlayerModelHandle ZPlayerModel_registerPlayerModel(unsigned long apiVersion, char *internalName, CustomModelType modelType) {
    if (sIsAPILocked) {
        recomp_printf("ZPlayerModel_registerPlayerModel: Models can only be registered during a ZPlayerModels_onRegisterModels callback.\n");
        return 0;
    }

    if (isStrValid("ZPlayerModel_registerPlayerModel", internalName, MAX_INTERNAL_NAME_LENGTH)) {
        return 0;
    }

    PlayerTransformation form;

    switch (modelType) {
        case CUSTOM_MODEL_TYPE_ADULT:
        case CUSTOM_MODEL_TYPE_CHILD:
            form = PLAYER_FORM_HUMAN;
            break;

        default:
            recomp_printf("ZPlayerModel_registerPlayerModel: Passed in unsupported ZPlayerModel_ModelType to ZPlayerModel_registerModel.\n");
            return 0;
            break;
    }

    ZPlayerModelHandle h = CMEM_createMemoryHandle(form);

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_registerModel");

    if (!entry) {
        return 0;
    }

    entry->modelEntry.internalName = internalName;

    if (modelType == CUSTOM_MODEL_TYPE_ADULT) {
        entry->modelEntry.flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;
    }

    return h;
}

RECOMP_EXPORT bool ZPlayerModel_setDisplayName(ZPlayerModelHandle h, char *displayName) {
    if (!isStrValid("ZPlayerModel_setDisplayName", displayName, MAX_DISPLAY_NAME_LENGTH)) {
        return 0;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setDisplayName");

    if (!entry) {
        return false;
    }

    entry->modelEntry.displayName = displayName;
    
    return true;
}

RECOMP_EXPORT bool ZPlayerModel_setAuthor(ZPlayerModelHandle h, char *author) {
    if (!isStrValid(ZPlayerModel_setAuthor, author, MAX_AUTHOR_NAME_LENGTH)) {
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setAuthor");

    if (!entry) {
        return false;
    }


    entry->modelEntry.authorName = author;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_setFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags |= flags;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_clearFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_clearFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags &= ~flags;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_clearAllFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_clearAllFlags");

    if (!entry) {
        return false;
    }

    entry->modelEntry.flags = 0;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_setDL(ZPlayerModelHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        recomp_printf("PlayerModelManager: Invalid display list ID passed in to ZPlayerModel_setDL.\n");
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_registerModel");

    if (!entry) {
        return false;
    }

    entry->displayListPtrs[dlId] = dl;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_setMtx(ZPlayerModelHandle h, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        recomp_printf("PlayerModelManager: Invalid matrix ID passed in to ZPlayerModel_setMtx.\n");
        return false;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setMtx");

    if (!entry) {
        return false;
    }

    entry->matrixPtrs[mtxId] = matrix;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_setLoadCallback(ZPlayerModelHandle h, void (*onModelLoad)(void *), void *userdata) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setLoadCallback");

    if (!entry) {
        return false;
    }

    entry->modelEntry.onModelLoad = onModelLoad;
    entry->modelEntry.onModelLoadData = userdata;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_setUnloadCallback(ZPlayerModelHandle h, void (*onModelUnload)(void *), void *userdata) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setUnloadCallback");

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

RECOMP_EXPORT bool ZPlayerModel_setSkeleton(ZPlayerModelHandle h, FlexSkeletonHeader *skel) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setSkeleton");

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

    return true;
}

#undef GET_LIMB_DL

RECOMP_EXPORT bool ZPlayerModel_setEyesTextures(ZPlayerModelHandle h, TexturePtr eyesTextures[PLAYER_EYES_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setEyesTextures");

    if (!entry) {
        return false;
    }

    entry->eyesTex = eyesTextures;

    return true;
}

RECOMP_EXPORT bool ZPlayerModel_setMouthTextures(ZPlayerModelHandle h, TexturePtr mouthTextures[PLAYER_MOUTH_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setMouthTextures");

    if (!entry) {
        return false;
    }

    entry->mouthTex = mouthTextures;

    return true;
}

RECOMP_DECLARE_EVENT(ZPlayerModels_onRegisterModels());

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onFinishedRegisterModels());

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyCMEM)
void doRegisterModels() {
    sIsAPILocked = false;
    ZPlayerModels_onRegisterModels();
    sIsAPILocked = true;
    PlayerModelManager_internal_onFinishedRegisterModels();
}
