#include "global.h"
#include "custommodelentry.h"
#include "qdfileloader_api.h"
#include "ml64compat_mm.h"
#include "recomputils.h"

void CustomModelEntry_init(CustomModelEntry *this) {
    this->displayName = NULL;
    this->internalName = NULL;
    this->flags = 0;
    this->onModelLoad = NULL;
    this->onModelLoadData = NULL;
    this->onModelUnload = NULL;
    this->onModelUnloadData = NULL;
    this->applyToModelInfo = NULL;
}

bool applyCustomModelMemoryEntry(void *thisx, Link_ModelInfo *modelInfo) {
    CustomModelMemoryEntry *this = thisx;

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        modelInfo->models[i] = this->displayListPtrs[i];
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        modelInfo->equipMtx[i] = this->matrixPtrs[i];
    }

    modelInfo->flags = this->modelEntry.flags;

    LodLimb **customLimbs = (LodLimb **)this->skel->sh.segment;

    if (this->skel) {
        for (int i = 0; i < PLAYER_LIMB_MAX; ++i) {
            modelInfo->limbTranslations[i] = customLimbs[i]->jointPos;
        }
    }

    if (this->mouthTex) {
        for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
            modelInfo->mouthTextures[i] = this->mouthTex[i];
        }
    }

    if (this->eyesTex) {
        for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
            modelInfo->eyesTextures[i] = this->eyesTex[i];
        }
    }

    return true;
}

bool applyCustomModelDiskEntry(void *thisx, Link_ModelInfo *modelInfo) {
    CustomModelDiskEntry *this = thisx;

    if (!this->filePath) {
        return false;
    }

    this->fileData = NULL;

    QDFL_loadFile(this->filePath, &this->fileData);

    if (!this->fileData) {
        return false;
    }

    setupZobjZ64O(modelInfo, this->fileData);

    return true;
}

void unloadCustomModelDiskEntry(void *userdata) {
    CustomModelDiskEntry *this = userdata;

    if (this->fileData) {
        recomp_free(this->fileData);
        this->fileData = NULL;
    }

    if (this->isOrphaned) {
        CustomModelDiskEntry_freeMembers(this);
        recomp_free(this);
    }
}

void CustomModelMemoryEntry_init(CustomModelMemoryEntry *this) {
    CustomModelEntry_init(&this->modelEntry);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        this->displayListPtrs[i] = NULL;
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        this->matrixPtrs[i] = NULL;
    }

    this->modelEntry.applyToModelInfo = applyCustomModelMemoryEntry;

    this->skel = NULL;

    this->mouthTex = NULL;

    this->eyesTex = NULL;
}

void CustomModelDiskEntry_init(CustomModelDiskEntry *this) {
    CustomModelEntry_init(&this->modelEntry);

    this->fileData = NULL;

    this->filePath = NULL;

    this->modelEntry.applyToModelInfo = applyCustomModelDiskEntry;
    this->modelEntry.onModelUnload = unloadCustomModelDiskEntry;
    this->modelEntry.onModelUnloadData = this;
    this->isOrphaned = false;
}

void CustomModelDiskEntry_freeMembers(CustomModelDiskEntry *this) {
    if (this->fileData) {
        recomp_free(this->fileData);
        this->fileData = NULL;
    }

    if (this->filePath) {
        recomp_free(this->fileData);
        this->filePath = NULL;
    }

    CustomModelEntry *entry = &this->modelEntry;

    if (entry->displayName) {
        recomp_free(entry->displayName);
        entry->displayName = NULL;
    }

    if (entry->internalName) {
        recomp_free(entry->internalName);
        entry->internalName = NULL;
    }
}
