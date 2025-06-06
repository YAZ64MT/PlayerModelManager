#include "global.h"
#include "custommodelentry.h"
#include "qdfileloader_api.h"
#include "ml64compat_mm.h"

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

    setupZobjZ64o(modelInfo, this->fileData);

    return true;
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
}

void CustomModelDiskEntry_init(CustomModelDiskEntry *this) {
    CustomModelEntry_init(&this->modelEntry);

    this->fileData = NULL;

    this->filePath = NULL;
}
