#include "global.h"
#include "custommodelentrymanager.h"
#include "recomputils.h"
#include "playermodelmanager.h"
#include "model_common.h"

static bool sIsAPILocked = true;

CustomModelMemoryEntry *getEntryOrPrintErr(ZPlayerModelHandle h, const char *funcName) {
    if (sIsAPILocked) {
        recomp_printf(
            "PlayerModelManager: %s called while API locked. " 
            "Please only call these functions during a ZPlayerModels_onRegisterModels callback.\n", funcName);
    }

        CustomModelMemoryEntry *entry = CMEM_getMemoryEntry(h);

    if (!entry) {
        recomp_printf("PlayerModelManager: Invalid handle passed in to %s.\n", funcName);
    }

    return entry;
}

RECOMP_EXPORT ZPlayerModelHandle ZPlayerModel_registerModel(unsigned long apiVersion, const char *internalName) {
    if (sIsAPILocked) {
        recomp_printf("PlayerModelManager: Models can only be registered during a ZPlayerModels_onRegisterModels callback.\n");
        return 0;
    }

    return CMEM_createMemoryHandle();
}

RECOMP_EXPORT void ZPlayerModel_setDisplayName(ZPlayerModelHandle h, const char* displayName) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setDisplayName");

    if (!entry) {
        return;
    }

    entry->modelEntry.displayName = displayName;
}

RECOMP_EXPORT void ZPlayerModel_setFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setFlags");

    if (!entry) {
        return;
    }

    entry->modelEntry.flags |= flags;
}

RECOMP_EXPORT void ZPlayerModel_clearFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_clearFlags");

    if (!entry) {
        return;
    }

    entry->modelEntry.flags &= ~flags;
}

RECOMP_EXPORT void ZPlayerModel_clearAllFlags(ZPlayerModelHandle h, u64 flags) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_clearAllFlags");

    if (!entry) {
        return;
    }

    entry->modelEntry.flags = 0;
}

RECOMP_EXPORT void ZPlayerModel_setDL(ZPlayerModelHandle h, Link_DisplayList dlId, Gfx *dl) {
    if (dlId >= LINK_DL_MAX || dlId < 0) {
        recomp_printf("PlayerModelManager: Invalid display list ID passed in to ZPlayerModel_setDL.\n");
        return;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_registerModel");

    if (!entry) {
        return;
    }

    entry->displayListPtrs[dlId] = dl;
}

RECOMP_EXPORT void ZPlayerModel_setMtx(ZPlayerModelHandle h, Link_EquipmentMatrix mtxId, Mtx *matrix) {
    if (mtxId >= LINK_EQUIP_MATRIX_MAX || mtxId < 0) {
        recomp_printf("PlayerModelManager: Invalid matrix ID passed in to ZPlayerModel_setMtx.\n");
        return;
    }

    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setMtx");

    if (!entry) {
        return;
    }

    entry->matrixPtrs[mtxId] = matrix;
}

RECOMP_EXPORT void ZPlayerModel_setLoadCallback(ZPlayerModelHandle h, void (*onModelLoad)(void *), void *userdata) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setLoadCallback");

    if (!entry) {
        return;
    }

    entry->modelEntry.onModelLoad = onModelLoad;
    entry->modelEntry.onModelLoadData = userdata;
}

RECOMP_EXPORT void ZPlayerModel_setUnloadCallback(ZPlayerModelHandle h, void (*onModelUnload)(void *), void *userdata) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setUnloadCallback");

    if (!entry) {
        return;
    }

    entry->modelEntry.onModelUnload = onModelUnload;
    entry->modelEntry.onModelUnloadData = userdata;
}

#define SET_LIMB_DL(pLimb, entryDL)       \
    if (!entry->displayListPtrs[entryDL]) \
        entry->displayListPtrs[entryDL] = (limbs[pLimb - 1]->dLists[0]) ? (limbs[pLimb - 1]->dLists[0]) : callDfCommand

RECOMP_EXPORT void ZPlayerModel_setSkeleton(ZPlayerModelHandle h, FlexSkeletonHeader *skel) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setSkeleton");

    if (!entry) {
        return;
    }

    entry->skel = skel;

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

#undef GET_LIMB_DL

RECOMP_EXPORT void ZPlayerModel_setEyesTextures(ZPlayerModelHandle h, TexturePtr eyesTextures[PLAYER_EYES_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setEyesTextures");

    if (!entry) {
        return;
    }

    entry->eyesTex = eyesTextures;
}

RECOMP_EXPORT void ZPlayerModel_setMouthTextures(ZPlayerModelHandle h, TexturePtr mouthTextures[PLAYER_MOUTH_MAX]) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setMouthTextures");

    if (!entry) {
        return;
    }

    entry->mouthTex = mouthTextures;
}

RECOMP_DECLARE_EVENT(ZPlayerModels_onRegisterModels());

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyML64Compat)
void doRegisterModels() {
    sIsAPILocked = false;
    ZPlayerModels_onRegisterModels();
    sIsAPILocked = true;
}
