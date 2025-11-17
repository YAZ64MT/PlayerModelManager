#ifndef __FORMPROXY_H__
#define __FORMPROXY_H__
#include "global.h"
#include "modelmatrixids.h"
#include "modelinfo.h"

#define PLAYER_BODY_SHIELD_LIMB_COUNT 4
#define PLAYER_LIMB_COUNT (PLAYER_LIMB_MAX - 1)

typedef struct ShieldingSkeletonProxy {
    FlexSkeletonHeader flexSkeleton;
    StandardLimb *limbPtrs[PLAYER_BODY_SHIELD_LIMB_COUNT];
    StandardLimb limbs[PLAYER_BODY_SHIELD_LIMB_COUNT];
} ShieldingSkeletonProxy;

typedef struct SkeletonProxy {
    FlexSkeletonHeader flexSkeleton;
    LodLimb *limbPtrs[PLAYER_LIMB_COUNT];
    LodLimb limbs[PLAYER_LIMB_COUNT];
} SkeletonProxy;

typedef struct WrappedDisplayList {
    Gfx displayList[3];
} WrappedDisplayList;

#define WRAPPED_DL_PREDRAW 0
#define WRAPPED_DL_DRAW 1
#define WRAPPED_DL_POSTDRAW 2

typedef struct TunicColor {
    Color_RGBA8 current;
    Color_RGBA8 requested;
    bool isOverrideRequested;
} TunicColor;

typedef struct FormProxy {
    PlayerTransformation form;
    FormProxyId fpId;
    ModelInfo currentModelInfo;
    ModelInfo *fallbackOverrideModelInfo;
    ModelInfo *fallbackModelInfo;
    U32ValueHashmapHandle displayListAlternates;
    SkeletonProxy skeleton;
    ShieldingSkeletonProxy shieldingSkeleton;
    TunicColor tunicColor;
    Gfx *shimDisplayListPtrs[LINK_SHIMDL_MAX];
    Gfx *mtxDisplayLists[LINK_EQUIP_MATRIX_MAX];
    Gfx displayLists[LINK_DL_MAX];
    WrappedDisplayList wrappedDisplayLists[LINK_DL_MAX];
} FormProxy;

void FormProxy_init(FormProxy *fp, PlayerTransformation form, FormProxyId fpId, ModelInfo *fallback, ModelInfo *fallbackOverride);
FormProxyId FormProxy_getFormProxyId(const FormProxy *fp);
ModelInfo *FormProxy_getCurrentModelInfo(FormProxy *fp);
ModelInfo *FormProxy_getFallbackOverrideModelInfo(FormProxy *fp);
ModelInfo *FormProxy_getFallbackModelInfo(FormProxy *fp);
bool FormProxy_setAlternateFormProxyDL(FormProxy *fp, Link_DisplayList id, FormProxy *alt);
bool FormProxy_unsetAlternateFormProxyDL(FormProxy *fp, Link_DisplayList id);
void FormProxy_refreshSkeletons(FormProxy *fp);
void FormProxy_refreshDL(FormProxy *fp, Link_DisplayList id);
void FormProxy_refreshAllDLs(FormProxy *fp);
void FormProxy_refreshMtx(FormProxy *fp, Link_EquipmentMatrix id);
void FormProxy_refreshAllMtxs(FormProxy *fp);
void FormProxy_requestTunicColor(FormProxy *fp, Color_RGBA8 color);
void FormProxy_refreshTunicColor(FormProxy *fp);
void FormProxy_setCurrentModelFormEntry(FormProxy *fp, ModelEntryForm *modelEntry);
void FormProxy_refreshPlayerFaceTextures(FormProxy *fp);
Mtx *FormProxy_getMatrix(FormProxy *fp, Link_EquipmentMatrix id);
Gfx *FormProxy_getDL(FormProxy *fp, Link_DisplayList id);

#endif
