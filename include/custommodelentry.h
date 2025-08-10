#ifndef __CUSTOMMODELENTRY_H__
#define __CUSTOMMODELENTRY_H__

#include "global.h"
#include "playermodelmanager.h"
#include "playermodelmanager_api.h"
#include "equipmentoverrides.h"
#include "recompdata.h"

// This is the max file name length for most Linux distros and MacOS
#define INTERNAL_NAME_MAX_LENGTH 255

typedef struct {
    PlayerModelManagerModelType type;
    char *displayName;
    char *internalName;
    char *authorName;
    bool (*applyToModelInfo)(void *this, Link_ModelInfo *modelInfo);
    PlayerModelManagerEventHandler* callback;
    void *callbackData;
    PlayerModelManagerHandle handle;
    u64 flags;
    U32ValueHashmapHandle displayListPtrs;
    U32ValueHashmapHandle matrixPtrs;
} ModelEntry;

typedef struct {
    ModelEntry modelEntry;
    FlexSkeletonHeader *skel;
    FlexSkeletonHeader *shieldingSkel;
    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];
} ModelEntryForm;

typedef struct {
    ModelEntry modelEntry;
    Link_EquipmentReplacement equipType;
} ModelEntryEquipment;

void ModelEntry_init(ModelEntry *entry);

Gfx *ModelEntry_getDisplayList(const ModelEntry *entry, Link_DisplayList id);

void ModelEntry_setDisplayList(ModelEntry *entry, Link_DisplayList id, Gfx *dl);

Mtx *ModelEntry_getMatrix(const ModelEntry *entry, Link_EquipmentMatrix id);

void ModelEntry_setMatrix(ModelEntry *entry, Link_EquipmentMatrix id, Mtx *mtx);

void ModelEntryForm_init(ModelEntryForm *this);

#endif
