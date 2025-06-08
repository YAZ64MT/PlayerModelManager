#include "global.h"
#include "custommodelentrymanager.h"
#include "recomputils.h"
#include "playermodelmanager.h"

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

RECOMP_EXPORT void ZPlayerModel_setSkeleton(ZPlayerModelHandle h, FlexSkeletonHeader *skel) {
    CustomModelMemoryEntry *entry = getEntryOrPrintErr(h, "ZPlayerModel_setSkeleton");

    if (!entry) {
        return;
    }

    entry->skel = skel;
}

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
