#ifndef __MODELINFO_H__
#define __MODELINFO_H__

#include "global.h"
#include "modelmatrixids.h"
#include "recompdata.h"

typedef struct {
    U32ValueHashmapHandle gfx;
    U32ValueHashmapHandle mtx;
    U32ValueHashmapHandle gfxOverrides;
    U32ValueHashmapHandle mtxOverrides;
    u64 flags;
    FlexSkeletonHeader *skeleton;
    FlexSkeletonHeader *shieldingSkeleton;
    TexturePtr eyesTextures[PLAYER_EYES_MAX];
    TexturePtr mouthTextures[PLAYER_MOUTH_MAX];
} ModelInfo;

void ModelInfo_init(ModelInfo *modelInfo);
Gfx *ModelInfo_getGfx(ModelInfo *modelInfo, Link_DisplayList id);
Mtx *ModelInfo_getMtx(ModelInfo *modelInfo, Link_EquipmentMatrix id);
void ModelInfo_setEyesTextures(ModelInfo *modelInfo, TexturePtr eyeTextures[]);
TexturePtr ModelInfo_getEyesTexture(ModelInfo *modelInfo, PlayerEyeIndex i);
void ModelInfo_setMouthTextures(ModelInfo *modelInfo, TexturePtr mouthTextures[]);
TexturePtr ModelInfo_getMouthTexture(ModelInfo *modelInfo, PlayerMouthIndex i);
void ModelInfo_setSkeleton(ModelInfo *modelInfo, FlexSkeletonHeader *skel);
void ModelInfo_unsetSkeleton(ModelInfo *modelInfo);
void ModelInfo_setShieldingSkeleton(ModelInfo *modelInfo, FlexSkeletonHeader *skel);
void ModelInfo_unsetShieldingSkeleton(ModelInfo *modelInfo);
void ModelInfo_setGfxMap(ModelInfo *modelInfo, U32ValueHashmapHandle gfxMap);
void ModelInfo_unsetGfxMap(ModelInfo *modelInfo);
bool ModelInfo_setGfxOverride(ModelInfo *modelInfo, Link_DisplayList id, Gfx *dl);
bool ModelInfo_unsetGfxOverride(ModelInfo *modelInfo, Link_DisplayList id);
void ModelInfo_setMtxMap(ModelInfo *modelInfo, U32ValueHashmapHandle mtxMap);
void ModelInfo_unsetMtxMap(ModelInfo *modelInfo);
bool ModelInfo_setMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id, Mtx *mtx);
bool ModelInfo_unsetMtxOverride(ModelInfo *modelInfo, Link_EquipmentMatrix id);
void ModelInfo_setFlags(ModelInfo *modelInfo, u64 flags);
void ModelInfo_unsetFlags(ModelInfo *modelInfo, u64 flags);
void ModelInfo_clearAllGfx(ModelInfo *modelInfo);
void ModelInfo_clearAllGfxOverrides(ModelInfo *modelInfo);
void ModelInfo_clearAllMtx(ModelInfo *modelInfo);
void ModelInfo_clearAllMtxOverrides(ModelInfo *modelInfo);
void ModelInfo_clearAllEyesTextures(ModelInfo *modelInfo);
void ModelInfo_clearAllMouthTextures(ModelInfo *modelInfo);
void ModelInfo_clearAllFlags(ModelInfo *modelInfo);
bool ModelInfo_isAnyFlagEnabled(ModelInfo *modelInfo, u64 flags);

#endif
