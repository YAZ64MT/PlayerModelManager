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

bool applyFormEntry(void *thisx, Link_ModelInfo *modelInfo) {
    clearLinkModelInfo(modelInfo);

    ModelEntryForm *this = thisx;

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        modelInfo->models[i] = ModelEntry_getDisplayList(&this->modelEntry, i);
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

void ModelEntryForm_init(ModelEntryForm *this) {
    ModelEntry_init(&this->modelEntry);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        if (ModelEntry_getDisplayList(&this->modelEntry, i)) {
            ModelEntry_setDisplayList(&this->modelEntry, i, NULL);
        }
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        this->modelEntry.matrixPtrs[i] = NULL;
    }

    this->modelEntry.applyToModelInfo = applyFormEntry;

    this->skel = NULL;

    Lib_MemSet(this->mouthTex, 0, sizeof(this->mouthTex));

    Lib_MemSet(this->eyesTex, 0, sizeof(this->eyesTex));
}

Gfx *ModelEntry_getDisplayList(const ModelEntry *entry, Link_DisplayList id) {
    if (id >= LINK_DL_MAX || id < 0) {
        return NULL;
    }

    return entry->displayListPtrs[id];
}

void ModelEntry_setDisplayList(ModelEntry *entry, Link_DisplayList id, Gfx *dl) {
    if (id >= LINK_DL_MAX || id < 0) {
        return;
    }

    entry->displayListPtrs[id] = dl;
}
