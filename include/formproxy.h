#ifndef __FORMPROXY_H__
#define __FORMPROXY_H__
#include "global.h"
#include "modelmatrixids.h"
#include "modelinfo.h"

#define PLAYER_BODY_SHIELD_LIMB_COUNT 4
#define PLAYER_LIMB_COUNT (PLAYER_LIMB_MAX - 1)

typedef struct {
    FlexSkeletonHeader flexSkeleton;
    StandardLimb *limbPtrs[PLAYER_BODY_SHIELD_LIMB_COUNT];
    StandardLimb limbs[PLAYER_BODY_SHIELD_LIMB_COUNT];
} ShieldingSkeletonProxy;

typedef struct {
    FlexSkeletonHeader flexSkeleton;
    LodLimb *limbPtrs[PLAYER_LIMB_COUNT];
    LodLimb limbs[PLAYER_LIMB_COUNT];
} SkeletonProxy;

typedef struct {
    Gfx displayList[3];
} WrappedDisplayList;

#define WRAPPED_DL_PREDRAW 0
#define WRAPPED_DL_DRAW 1
#define WRAPPED_DL_POSTDRAW 2

typedef struct {
    Color_RGBA8 current;
    Color_RGBA8 requested;
    bool isOverrideRequested;
} TunicColor;

typedef struct {
    PlayerTransformation form;
    ModelInfo* currentModelInfo;
    ModelInfo* fallbackModelInfo;
    SkeletonProxy skeleton;
    ShieldingSkeletonProxy shieldingSkeleton;
    TunicColor tunicColor;
    Gfx *shimDisplayListPtrs[LINK_SHIMDL_MAX];
    Gfx *mtxDisplayLists[LINK_EQUIP_MATRIX_MAX];
    Gfx displayLists[LINK_DL_MAX];
    WrappedDisplayList wrappedDisplayLists[LINK_DL_MAX];
} FormProxy;

void FormProxy_init(FormProxy *fp, ModelInfo *current, ModelInfo *fallback, PlayerTransformation form);
ModelInfo *FormProxy_getCurrentModelInfo(FormProxy *fp);
ModelInfo *FormProxy_getFallbackModelInfo(FormProxy *fp);
void FormProxy_refreshSkeletons(FormProxy *fp);
void FormProxy_refreshDL(FormProxy *fp, Link_DisplayList id);
void FormProxy_refreshAllDLs(FormProxy *fp);
void FormProxy_refreshMtx(FormProxy *fp, Link_EquipmentMatrix id);
void FormProxy_refreshAllMtxs(FormProxy *fp);
void FormProxy_requestTunicColor(FormProxy *fp, Color_RGBA8 color);
void FormProxy_refreshTunicColor(FormProxy *fp);
void FormProxy_swapCurrentDLMap(FormProxy *fp);
void FormProxy_getMatrix(FormProxy *fp, Link_EquipmentMatrix id);
void FormProxy_refreshPlayerFaceTextures(FormProxy *fp);

#endif
