#ifndef __PLAYERMODELMANAGER_H__
#define __PLAYERMODELMANAGER_H__

#include "global.h"
#include "modelmatrixids.h"
#include "recompdata.h"

#define PLAYER_LIMB_COUNT 21
#define PLAYER_BODY_SHIELD_LIMB_COUNT 4

typedef struct {
    u64 flags;
    Gfx *models[LINK_DL_MAX];
    Mtx *equipMtx[LINK_EQUIP_MATRIX_MAX];
    TexturePtr eyesTextures[PLAYER_EYES_MAX];
    TexturePtr mouthTextures[PLAYER_MOUTH_MAX];
    FlexSkeletonHeader *skeleton;
    FlexSkeletonHeader *shieldingSkeleton;
} Link_ModelInfo;

typedef struct {
    FlexSkeletonHeader flexSkeleton;
    StandardLimb *limbPtrs[PLAYER_BODY_SHIELD_LIMB_COUNT];
    StandardLimb limbs[PLAYER_BODY_SHIELD_LIMB_COUNT];
} Link_ShieldingSkeletonProxy;

typedef struct {
    FlexSkeletonHeader flexSkeleton;
    LodLimb *limbPtrs[PLAYER_LIMB_COUNT];
    LodLimb limbs[PLAYER_LIMB_COUNT];
} Link_SkeletonProxy;

typedef struct {
    Gfx displayList[3];
} WrappedDisplayList;

#define WRAPPED_DL_PREDRAW 0
#define WRAPPED_DL_DRAW 1
#define WRAPPED_DL_POSTDRAW 2

typedef struct {
    PlayerTransformation form;
    Link_ModelInfo current;
    Link_ModelInfo vanilla;
    U32ValueHashmapHandle gfxOverrides;
    U32ValueHashmapHandle mtxOverrides;
    Link_SkeletonProxy skeleton;
    Link_ShieldingSkeletonProxy shieldingSkeleton;
    Gfx *shimDisplayListPtrs[LINK_SHIMDL_MAX];
    Gfx *mtxDisplayLists[LINK_EQUIP_MATRIX_MAX];
    Gfx displayLists[LINK_DL_MAX];
    WrappedDisplayList wrappedDisplayLists[LINK_DL_MAX];
} Link_FormProxy;

#endif
