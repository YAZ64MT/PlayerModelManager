#include "global.h"
#include "custommodelentry.h"
#include "qdfileloader_api.h"
#include "ml64compat_mm.h"
#include "recomputils.h"
#include "playermodelmanager_utils.h"
#include "custommodelentrymanager.h"

void FormModelEntry_init(FormModelEntry *entry) {
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
        modelInfo->models[i] = this->displayListPtrs[i];
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        modelInfo->equipMtx[i] = this->matrixPtrs[i];
    }

    modelInfo->flags = this->modelEntry.flags;

    modelInfo->skeleton = this->skel;

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

    modelInfo->shieldingSkeleton = this->shieldingSkel;

    return true;
}

bool applyFormModelDiskEntry(void *thisx, Link_ModelInfo *modelInfo) {
    FormModelDiskEntry *this = thisx;

    if (!this->filePath) {
        return false;
    }

    PlayerTransformation form = PLAYER_FORM_HUMAN;

    switch (this->modelEntry.type) {
        case PMM_MODEL_TYPE_ADULT:
        case PMM_MODEL_TYPE_CHILD:
            form = PLAYER_FORM_HUMAN;
            break;
        
        case PMM_MODEL_TYPE_DEKU:
            form = PLAYER_FORM_DEKU;
            break;

        case PMM_MODEL_TYPE_GORON:
            form = PLAYER_FORM_GORON;
            break;

        case PMM_MODEL_TYPE_ZORA:
            form = PLAYER_FORM_ZORA;
            break;

        case PMM_MODEL_TYPE_FIERCE_DEITY:
            form = PLAYER_FORM_FIERCE_DEITY;
            break;

        default:
            // invalid form
            return false;
            break;
    }

    this->fileData = CMEM_loadFromDisk(form, this->filePath);

    if (!this->fileData) {
        return false;
    }

    setupZobjZ64O(modelInfo, this->fileData);

    return true;
}

void formModelDiskEntryCallback(PlayerModelManagerHandle h, PlayerModelManagerModelEvent evt, void *userdata) {
    if (evt == PMM_EVENT_MODEL_REMOVED) {
        FormModelDiskEntry *this = userdata;

        this->fileData = NULL;
    }
}

void FormModelMemoryEntry_init(FormModelMemoryEntry *this) {
    FormModelEntry_init(&this->modelEntry);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        this->displayListPtrs[i] = NULL;
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        this->matrixPtrs[i] = NULL;
    }

    this->modelEntry.applyToModelInfo = applyFormModelMemoryEntry;

    this->skel = NULL;

    this->mouthTex = NULL;

    this->eyesTex = NULL;
}

void FormModelDiskEntry_init(FormModelDiskEntry *this, FormModelType type) {
    FormModelEntry_init(&this->modelEntry);

    this->fileData = NULL;

    this->filePath = NULL;

    this->modelEntry.applyToModelInfo = applyFormModelDiskEntry;
    this->modelEntry.callback = formModelDiskEntryCallback;
    this->modelEntry.callbackData = this;
    this->modelEntry.type = type;
}

void FormModelDiskEntry_freeMembers(FormModelDiskEntry *this) {
    this->fileData = NULL;

    if (this->filePath) {
        recomp_free(this->filePath);
        this->filePath = NULL;
    }

    FormModelEntry *entry = &this->modelEntry;

    if (entry->displayName) {
        recomp_free(entry->displayName);
        entry->displayName = NULL;
    }

    if (entry->internalName) {
        recomp_free(entry->internalName);
        entry->internalName = NULL;
    }

    if (entry->authorName) {
        recomp_free(entry->authorName);
        entry->authorName = NULL;
    }
}
