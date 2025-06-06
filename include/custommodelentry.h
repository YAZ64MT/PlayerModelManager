#ifndef __CUSTOMMODELENTRY_H__
#define __CUSTOMMODELENTRY_H__

#include "global.h"
#include "playermodelmanager.h"

typedef enum {
    CUSTOM_MODEL_TYPE_CHILD,
    CUSTOM_MODEL_TYPE_ADULT,
    CUSTOM_MODEL_TYPE_DEKU,
    CUSTOM_MODEL_TYPE_GORON,
    CUSTOM_MODEL_TYPE_ZORA,
    CUSTOM_MODEL_TYPE_FIERCE_DEITY
} CustomModelType;

typedef struct CustomModelEntry {
    CustomModelType type;
    u64 flags;
    char *displayName;
    char *internalName;
    bool (*applyToModelInfo)(void *this, Link_ModelInfo *modelInfo);
    void (*onModelLoad)(void *userdata);
    void *onModelLoadData;
    void (*onModelUnload)(void *userdata);
    void *onModelUnloadData;
} CustomModelEntry;

typedef struct {
    CustomModelEntry modelEntry;
    char *filePath;
    void *fileData;
} CustomModelDiskEntry;

typedef struct {
    CustomModelEntry modelEntry;
    Gfx *displayListPtrs[LINK_DL_MAX];
    Mtx *matrixPtrs[LINK_EQUIP_MATRIX_MAX];
} CustomModelMemoryEntry;

void CustomModelEntry_init(CustomModelEntry *entry);

#endif
