#include "global.h"
#include "custommodelentry.h"
#include "recomputils.h"
#include "playermodelmanager_utils.h"
#include "custommodelentrymanager.h"

void ModelEntry_init(ModelEntry *entry) {
    entry->displayName = NULL;
    entry->internalName = NULL;
    entry->authorName = NULL;
    entry->flags = 0;
    entry->callback = NULL;
    entry->callbackData = NULL;
    entry->handle = 0;
    entry->applyToModelInfo = NULL;
}

bool applyFormModelMemoryEntry(void *thisx, Link_ModelInfo *modelInfo) {
    clearLinkModelInfo(modelInfo);

    FormModelMemoryEntry *this = thisx;

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        modelInfo->models[i] = this->modelEntry.displayListPtrs[i];
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        modelInfo->equipMtx[i] = this->modelEntry.matrixPtrs[i];
    }

    modelInfo->flags = this->modelEntry.flags;

    modelInfo->skeleton = this->skel;

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        if (this->eyesTex[i]) {
            modelInfo->mouthTextures[i] = this->mouthTex[i];
        }
    }

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        if (this->eyesTex[i]) {
            modelInfo->eyesTextures[i] = this->eyesTex[i];
        }
    }

    modelInfo->shieldingSkeleton = this->shieldingSkel;

    return true;
}

void FormModelMemoryEntry_init(FormModelMemoryEntry *this) {
    ModelEntry_init(&this->modelEntry);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        this->modelEntry.displayListPtrs[i] = NULL;
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        this->modelEntry.matrixPtrs[i] = NULL;
    }

    this->modelEntry.applyToModelInfo = applyFormModelMemoryEntry;

    this->skel = NULL;

    Lib_MemSet(this->mouthTex, 0, sizeof(this->mouthTex));

    Lib_MemSet(this->eyesTex, 0, sizeof(this->eyesTex));
}
