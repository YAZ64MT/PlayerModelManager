#ifndef __CUSTOMMODELENTRY_H__
#define __CUSTOMMODELENTRY_H__

#include "global.h"
#include "playermodelmanager.h"

// This is the max file name length for most Linux distros and MacOS
#define INTERNAL_NAME_MAX_LENGTH 255

typedef enum {
    CUSTOM_MODEL_TYPE_NONE,
    CUSTOM_MODEL_TYPE_CHILD,
    CUSTOM_MODEL_TYPE_ADULT,
    CUSTOM_MODEL_TYPE_DEKU,
    CUSTOM_MODEL_TYPE_GORON,
    CUSTOM_MODEL_TYPE_ZORA,
    CUSTOM_MODEL_TYPE_FIERCE_DEITY
} CustomModelType;

typedef struct CustomModelEntry {
    CustomModelType type;
    char *displayName;
    char *internalName;
    char *authorName;
    bool (*applyToModelInfo)(void *this, Link_ModelInfo *modelInfo);
    void (*onModelLoad)(void *userdata);
    void *onModelLoadData;
    void (*onModelUnload)(void *userdata);
    void *onModelUnloadData;
    u64 flags;
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
    FlexSkeletonHeader *skel;
    FlexSkeletonHeader *shieldingSkel;
    TexturePtr *eyesTex;
    TexturePtr *mouthTex;
} CustomModelMemoryEntry;

void CustomModelEntry_init(CustomModelEntry *entry);

void CustomModelMemoryEntry_init(CustomModelMemoryEntry *this);

void CustomModelDiskEntry_init(CustomModelDiskEntry *this, CustomModelType type);

void CustomModelDiskEntry_freeMembers(CustomModelDiskEntry *this);

#endif
