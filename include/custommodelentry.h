#ifndef __CUSTOMMODELENTRY_H__
#define __CUSTOMMODELENTRY_H__

#include "global.h"
#include "playermodelmanager.h"
#include "playermodelmanager_api.h"
#include "recompdata.h"

// This is the max file name length for most Linux distros and MacOS
#define INTERNAL_NAME_MAX_LENGTH 255

typedef PlayerModelManagerModelType FormModelType;

typedef struct {
    FormModelType type;
    char *displayName;
    char *internalName;
    char *authorName;
    bool (*applyToModelInfo)(void *this, Link_ModelInfo *modelInfo);
    PlayerModelManagerEventHandler* callback;
    void *callbackData;
    PlayerModelManagerHandle handle;
    u64 flags;
    Gfx *displayListPtrs[LINK_DL_MAX];
    Mtx *matrixPtrs[LINK_EQUIP_MATRIX_MAX];
    Mtx matrixes[LINK_EQUIP_MATRIX_MAX];
} ModelEntry;

typedef struct {
    ModelEntry modelEntry;
    FlexSkeletonHeader *skel;
    FlexSkeletonHeader *shieldingSkel;
    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];
} FormModelMemoryEntry;

void ModelEntry_init(ModelEntry *entry);

void FormModelMemoryEntry_init(FormModelMemoryEntry *this);

#endif
