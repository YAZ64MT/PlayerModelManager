#ifndef __PLAYERMODELMANAGER_H__
#define __PLAYERMODELMANAGER_H__

#include "global.h"
#include "modelmatrixids.h"
#include "recompdata.h"

#define PLAYER_LIMB_COUNT (PLAYER_LIMB_MAX - 1)
#define PLAYER_BODY_SHIELD_LIMB_COUNT 4

typedef struct Link_ModelInfo {
    u64 flags;
    Gfx *models[LINK_DL_MAX];
    Mtx *equipMtx[LINK_EQUIP_MATRIX_MAX];
    TexturePtr eyesTextures[PLAYER_EYES_MAX];
    TexturePtr mouthTextures[PLAYER_MOUTH_MAX];
    FlexSkeletonHeader *skeleton;
    FlexSkeletonHeader *shieldingSkeleton;
} Link_ModelInfo;

typedef struct Link_ModelInfoCustom {
    Link_ModelInfo modelInfo;
    U32ValueHashmapHandle gfxOverrides;
    U32ValueHashmapHandle mtxOverrides;
} Link_ModelInfoCustom;

typedef struct Link_ShieldingSkeletonProxy {
    FlexSkeletonHeader flexSkeleton;
    StandardLimb *limbPtrs[PLAYER_BODY_SHIELD_LIMB_COUNT];
    StandardLimb limbs[PLAYER_BODY_SHIELD_LIMB_COUNT];
} Link_ShieldingSkeletonProxy;

typedef struct Link_SkeletonProxy {
    FlexSkeletonHeader flexSkeleton;
    LodLimb *limbPtrs[PLAYER_LIMB_COUNT];
    LodLimb limbs[PLAYER_LIMB_COUNT];
} Link_SkeletonProxy;

typedef struct Link_WrappedDisplayList {
    Gfx displayList[3];
} Link_WrappedDisplayList;

#define WRAPPED_DL_PREDRAW 0
#define WRAPPED_DL_DRAW 1
#define WRAPPED_DL_POSTDRAW 2

typedef struct Link_TunicColor {
    Color_RGBA8 current;
    Color_RGBA8 requested;
    bool isOverrideRequested;
} Link_TunicColor;

typedef struct Link_FormProxy {
    PlayerTransformation form;
    Link_ModelInfoCustom current;
    Link_ModelInfo vanilla;
    Link_SkeletonProxy skeleton;
    Link_ShieldingSkeletonProxy shieldingSkeleton;
    Link_TunicColor tunicColor;
    Gfx *shimDisplayListPtrs[LINK_SHIMDL_MAX];
    Gfx *mtxDisplayLists[LINK_EQUIP_MATRIX_MAX];
    Gfx displayLists[LINK_DL_MAX];
    Link_WrappedDisplayList wrappedDisplayLists[LINK_DL_MAX];
} Link_FormProxy;

#endif
