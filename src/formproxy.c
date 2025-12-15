#include "global.h"
#include "modding.h"
#include "formproxy.h"
#include "logger.h"
#include "utils.h"
#include "rt64_extended_gbi.h"
#include "modelreplacer_compat.h"
#include "modelmatrixids.h"
#include "z64recomp_api.h"
#include "model_shared.h"
#include "modelentry.h"
#include "modelreplacer_compat.h"
#include "recompconfig.h"
#include "playermodelconfig.h"

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid FormProxy pointer.")

static U32HashsetHandle sValidFormProxyPtrs;

static bool isValidFormProxy(const FormProxy *fp) {
    return recomputil_u32_hashset_contains(sValidFormProxyPtrs, (uintptr_t)fp);
}

static Gfx sPopModelViewMtx[] = {
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};

static Gfx sStartDLWrapper[] = {
    gsSPNoOp(), // gEXPushEnvColor
    gsSPEndDisplayList(),
};

static Gfx sEndDLWrapper[] = {
    gsSPNoOp(), // gEXPopEnvColor
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

static Gfx sStartBowStringDL[] = {
    // Two commands worth of space for the gEXMatrixGroup.
    gsSPNoOp(),
    gsSPNoOp(),

    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_NOPUSH | G_MTX_MUL),
    gsSPEndDisplayList(),
};

static Gfx sEndBowstringDL[] = {
    gsSPNoOp(),
    gsSPBranchList(sEndDLWrapper),
};

RECOMP_CALLBACK(".", _internal_preInitHashObjects)
void initFormProxyExDLs() {
    gEXPushEnvColor(&sStartDLWrapper[0]);
    gEXPopEnvColor(&sEndDLWrapper[0]);
    gEXMatrixGroupSimple(&sStartBowStringDL[0], z64recomp_get_bowstring_transform_id(), G_EX_PUSH, G_MTX_MODELVIEW,
                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
    gEXPopMatrixGroup(&sEndBowstringDL[0], G_MTX_MODELVIEW);
}

static void initSkeleton(FormProxy *fp) {
    static const LodLimb skeletonBase[PLAYER_LIMB_COUNT] = {
        {{0, 0, 0}, PLAYER_LIMB_WAIST - 1, LIMB_DONE, {NULL, NULL}},
        {{0, 0, 0}, PLAYER_LIMB_LOWER_ROOT - 1, PLAYER_LIMB_UPPER_ROOT - 1, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_RIGHT_THIGH - 1, LIMB_DONE, {NULL, NULL}},
        {{0, 0, 0}, PLAYER_LIMB_RIGHT_SHIN - 1, PLAYER_LIMB_LEFT_THIGH - 1, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_RIGHT_FOOT - 1, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_LEFT_SHIN - 1, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_LEFT_FOOT - 1, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_HEAD - 1, LIMB_DONE, {NULL, NULL}},
        {{0, 0, 0}, PLAYER_LIMB_HAT - 1, PLAYER_LIMB_COLLAR - 1, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, PLAYER_LIMB_LEFT_SHOULDER - 1, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_LEFT_FOREARM - 1, PLAYER_LIMB_RIGHT_SHOULDER - 1, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_LEFT_HAND - 1, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_RIGHT_FOREARM - 1, PLAYER_LIMB_SHEATH - 1, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, PLAYER_LIMB_RIGHT_HAND - 1, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, PLAYER_LIMB_TORSO - 1, {gEmptyDL, gEmptyDL}},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDL, gEmptyDL}},
    };

#define SET_LIMB_DL(playerLimb, proxyLimbName) skel->limbs[playerLimb - 1].dLists[0] = skel->limbs[playerLimb - 1].dLists[1] = &fp->displayLists[proxyLimbName]

    SkeletonProxy *skel = &fp->skeleton;
    FlexSkeletonHeader *flex = &skel->flexSkeleton;
    flex->dListCount = 18;
    flex->sh.limbCount = PLAYER_LIMB_COUNT;

    for (u32 i = 0; i < PLAYER_LIMB_COUNT; i++) {
        skel->limbPtrs[i] = &skel->limbs[i];

        skel->limbs[i] = skeletonBase[i];
    }

    SET_LIMB_DL(PLAYER_LIMB_WAIST, LINK_DL_WAIST);
    SET_LIMB_DL(PLAYER_LIMB_RIGHT_THIGH, LINK_DL_RTHIGH);
    SET_LIMB_DL(PLAYER_LIMB_RIGHT_SHIN, LINK_DL_RSHIN);
    SET_LIMB_DL(PLAYER_LIMB_RIGHT_FOOT, LINK_DL_RFOOT);
    SET_LIMB_DL(PLAYER_LIMB_LEFT_THIGH, LINK_DL_LTHIGH);
    SET_LIMB_DL(PLAYER_LIMB_LEFT_SHIN, LINK_DL_LSHIN);
    SET_LIMB_DL(PLAYER_LIMB_LEFT_FOOT, LINK_DL_LFOOT);
    SET_LIMB_DL(PLAYER_LIMB_HEAD, LINK_DL_HEAD);
    SET_LIMB_DL(PLAYER_LIMB_HAT, LINK_DL_HAT);
    SET_LIMB_DL(PLAYER_LIMB_COLLAR, LINK_DL_COLLAR);
    SET_LIMB_DL(PLAYER_LIMB_LEFT_SHOULDER, LINK_DL_LSHOULDER);
    SET_LIMB_DL(PLAYER_LIMB_LEFT_FOREARM, LINK_DL_LFOREARM);
    SET_LIMB_DL(PLAYER_LIMB_LEFT_HAND, LINK_DL_LHAND);
    SET_LIMB_DL(PLAYER_LIMB_RIGHT_SHOULDER, LINK_DL_RSHOULDER);
    SET_LIMB_DL(PLAYER_LIMB_RIGHT_FOREARM, LINK_DL_RFOREARM);
    SET_LIMB_DL(PLAYER_LIMB_RIGHT_HAND, LINK_DL_RHAND);
    SET_LIMB_DL(PLAYER_LIMB_SHEATH, LINK_DL_SHEATH_NONE);
    SET_LIMB_DL(PLAYER_LIMB_TORSO, LINK_DL_TORSO);

    flex->sh.segment = (void **)skel->limbPtrs;

#undef SET_LIMB_DL
}

static void initShieldingSkeleton(FormProxy *fp) {
    static const StandardLimb shieldingSkeletonBase[] = {
        {{0, 0, 0}, LINK_BODY_SHIELD_LIMB_BODY - 1, LIMB_DONE, NULL},
        {{0, 2000, -800}, LINK_BODY_SHIELD_LIMB_HEAD - 1, LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS - 1, gEmptyDL},
        {{2600, 0, 0}, LIMB_DONE, LIMB_DONE, gEmptyDL},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, gEmptyDL},
    };

    ShieldingSkeletonProxy *skel = &fp->shieldingSkeleton;
    FlexSkeletonHeader *flex = &skel->flexSkeleton;

    flex->sh.limbCount = PLAYER_BODY_SHIELD_LIMB_COUNT;
    flex->dListCount = 3;

    for (int i = 0; i < PLAYER_BODY_SHIELD_LIMB_COUNT; ++i) {
        skel->limbs[i] = shieldingSkeletonBase[i];
        skel->limbPtrs[i] = &skel->limbs[i];
    }

    skel->limbs[LINK_BODY_SHIELD_LIMB_BODY - 1].dList = &fp->displayLists[LINK_DL_BODY_SHIELD_BODY];
    skel->limbs[LINK_BODY_SHIELD_LIMB_HEAD - 1].dList = &fp->displayLists[LINK_DL_BODY_SHIELD_HEAD];
    skel->limbs[LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS - 1].dList = &fp->displayLists[LINK_DL_BODY_SHIELD_ARMS_AND_LEGS];

    flex->sh.segment = (void **)skel->limbPtrs;
}

static void initMatrixes(FormProxy *fp) {
    Gfx *mtxDLs = recomp_alloc(sizeof(Gfx) * 2 * LINK_EQUIP_MATRIX_MAX);

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        fp->mtxDisplayLists[i] = &mtxDLs[i * 2];

        gSPMatrix(&fp->mtxDisplayLists[i][0], &gIdentityMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPEndDisplayList(&fp->mtxDisplayLists[i][1]);
    }
}

static void initProxyShims(FormProxy *fp) {
    Gfx *dls = fp->displayLists;
    Gfx **mtxDls = fp->mtxDisplayLists;
    Gfx **shims = fp->shimDisplayListPtrs;

    Gfx *shimArr = recomp_alloc(sizeof(*shimArr) * LINK_SHIMDL_MAX);

    // init by pointing all to DF command
    for (int i = 0; i < LINK_SHIMDL_MAX; ++i) {
        shims[i] = &shimArr[i];
        gSPBranchList(shims[i], gEmptyDL);
    }

#define SHIM_ITEM_HAND(hand, item) shims[LINK_SHIMDL_##hand##_##item] = Utils_createShimDisplayList(2, &dls[LINK_DL_##item], &dls[LINK_DL_##hand])
#define SHIM_ITEM_OPT_HAND(hand, item) shims[LINK_SHIMDL_##hand##_##item] = Utils_createShimDisplayList(2, &dls[LINK_DL_##item], &dls[LINK_DL_OPT_##hand##_##item])
#define SHIM_ITEM_LFIST(item) SHIM_ITEM_HAND(LFIST, item)
#define SHIM_ITEM_OPT_LFIST(item) SHIM_ITEM_OPT_HAND(LFIST, item)
#define SHIM_ITEM_RFIST(item) SHIM_ITEM_HAND(RFIST, item)
#define SHIM_ITEM_OPT_RFIST(item) SHIM_ITEM_OPT_HAND(RFIST, item)
#define SHIM_ITEM_LHAND(item) SHIM_ITEM_HAND(LHAND, item)
#define SHIM_ITEM_OPT_LHAND(item) SHIM_ITEM_OPT_HAND(LHAND, item)
#define SHIM_ITEM_RHAND(item) SHIM_ITEM_HAND(RHAND, item)
#define SHIM_ITEM_OPT_RHAND(item) SHIM_ITEM_OPT_HAND(RHAND, item)
#define SHIM_ITEM_RHAND_OCARINA(item) shims[LINK_SHIMDL_RHAND_##item] = Utils_createShimDisplayList(2, &dls[LINK_DL_##item], &dls[LINK_DL_OPT_RHAND_##item])
#define SHIM_HILT_BACK(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_HILT_BACK] = Utils_createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK], &dls[LINK_DL_SWORD##swordNum##_HILT], sPopModelViewMtx)
#define SHIM_SWORD_SHEATHED(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHEATHED] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SWORD##swordNum##_HILT_BACK])
#define SHIM_SHIELD_BACK(shieldNum) shims[LINK_SHIMDL_SHIELD##shieldNum##_BACK] = Utils_createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD##shieldNum##_BACK], &dls[LINK_DL_SHIELD##shieldNum], sPopModelViewMtx)
#define SHIM_SWORD_SHIELD_UNSHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_UNSHEATHED] = Utils_createShimDisplayList(1, &dls[LINK_DL_SWORD##swordNum##_SHEATH])
#define SHIM_SWORD_SHIELD_SHEATH(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATH] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD_SHIELD_SHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATHED] = Utils_createShimDisplayList(3, &dls[LINK_DL_SWORD##swordNum##_SHEATHED], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD(swordNum) shims[LINK_SHIMDL_SWORD##swordNum] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_HILT], &dls[LINK_DL_SWORD##swordNum##_BLADE])
#define SHIM_SWORD_LFIST(swordNum) SHIM_ITEM_LFIST(SWORD##swordNum)
#define SHIM_SHIELD_RFIST(shieldNum) SHIM_ITEM_RFIST(SHIELD##shieldNum)

    SHIM_SWORD(1);
    SHIM_SWORD(2);
    SHIM_SWORD(3);
    SHIM_SWORD(4);
    shims[LINK_SHIMDL_SWORD4_BROKEN] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD4_HILT], &dls[LINK_DL_SWORD4_BLADE_BROKEN]);
    SHIM_SWORD(5);

    SHIM_HILT_BACK(1);
    SHIM_HILT_BACK(2);
    SHIM_HILT_BACK(3);
    SHIM_HILT_BACK(4);
    SHIM_HILT_BACK(5);

    SHIM_SWORD_SHEATHED(1);
    SHIM_SWORD_SHEATHED(2);
    SHIM_SWORD_SHEATHED(3);
    SHIM_SWORD_SHEATHED(4);
    SHIM_SWORD_SHEATHED(5);

    SHIM_SHIELD_BACK(1);
    SHIM_SHIELD_BACK(2);
    SHIM_SHIELD_BACK(3);

    SHIM_SWORD_SHIELD_SHEATH(1, 1);
    SHIM_SWORD_SHIELD_SHEATH(1, 2);
    SHIM_SWORD_SHIELD_SHEATH(1, 3);
    SHIM_SWORD_SHIELD_SHEATH(2, 1);
    SHIM_SWORD_SHIELD_SHEATH(2, 2);
    SHIM_SWORD_SHIELD_SHEATH(2, 3);
    SHIM_SWORD_SHIELD_SHEATH(3, 1);
    SHIM_SWORD_SHIELD_SHEATH(3, 2);
    SHIM_SWORD_SHIELD_SHEATH(3, 3);
    SHIM_SWORD_SHIELD_SHEATH(4, 1);
    SHIM_SWORD_SHIELD_SHEATH(4, 2);
    SHIM_SWORD_SHIELD_SHEATH(4, 3);
    SHIM_SWORD_SHIELD_SHEATH(5, 1);
    SHIM_SWORD_SHIELD_SHEATH(5, 2);
    SHIM_SWORD_SHIELD_SHEATH(5, 3);

    SHIM_SWORD_SHIELD_SHEATHED(1, 1);
    SHIM_SWORD_SHIELD_SHEATHED(1, 2);
    SHIM_SWORD_SHIELD_SHEATHED(1, 3);
    SHIM_SWORD_SHIELD_SHEATHED(2, 1);
    SHIM_SWORD_SHIELD_SHEATHED(2, 2);
    SHIM_SWORD_SHIELD_SHEATHED(2, 3);
    SHIM_SWORD_SHIELD_SHEATHED(3, 1);
    SHIM_SWORD_SHIELD_SHEATHED(3, 2);
    SHIM_SWORD_SHIELD_SHEATHED(3, 3);
    SHIM_SWORD_SHIELD_SHEATHED(4, 1);
    SHIM_SWORD_SHIELD_SHEATHED(4, 2);
    SHIM_SWORD_SHIELD_SHEATHED(4, 3);
    SHIM_SWORD_SHIELD_SHEATHED(5, 1);
    SHIM_SWORD_SHIELD_SHEATHED(5, 2);
    SHIM_SWORD_SHIELD_SHEATHED(5, 3);

    shims[LINK_SHIMDL_LFIST_SWORD1] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD1], &dls[LINK_DL_OPT_LFIST_SWORD1]);
    shims[LINK_SHIMDL_LFIST_SWORD2] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD2], &dls[LINK_DL_OPT_LFIST_SWORD2]);
    shims[LINK_SHIMDL_LFIST_SWORD3] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD3], &dls[LINK_DL_OPT_LFIST_SWORD3]);
    shims[LINK_SHIMDL_LFIST_SWORD3_PEDESTAL_GRABBED] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD3_PEDESTAL_GRABBED], &dls[LINK_DL_OPT_LFIST_SWORD3]);
    shims[LINK_SHIMDL_LFIST_SWORD4] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD4], &dls[LINK_DL_OPT_LFIST_SWORD4]);
    shims[LINK_SHIMDL_LFIST_SWORD4_BROKEN] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD4_BROKEN], &dls[LINK_DL_OPT_LFIST_SWORD4]);
    shims[LINK_SHIMDL_LFIST_SWORD5] = Utils_createShimDisplayList(2, &dls[LINK_DL_SWORD5], &dls[LINK_DL_OPT_LFIST_SWORD5]);

    shims[LINK_SHIMDL_RFIST_SHIELD1] = Utils_createShimDisplayList(2, &dls[LINK_DL_SHIELD1], &dls[LINK_DL_OPT_RFIST_SHIELD1]);
    shims[LINK_SHIMDL_RFIST_SHIELD2] = Utils_createShimDisplayList(2, &dls[LINK_DL_SHIELD2], &dls[LINK_DL_OPT_RFIST_SHIELD2]);
    shims[LINK_SHIMDL_RFIST_SHIELD3] = Utils_createShimDisplayList(2, &dls[LINK_DL_SHIELD3], &dls[LINK_DL_OPT_RFIST_SHIELD3]);

    SHIM_ITEM_OPT_LFIST(HAMMER);
    SHIM_ITEM_OPT_RFIST(BOW);
    SHIM_ITEM_OPT_RFIST(HOOKSHOT);
    SHIM_ITEM_OPT_RFIST(LONGSHOT);
    SHIM_ITEM_RHAND_OCARINA(OCARINA_FAIRY);
    SHIM_ITEM_RHAND_OCARINA(OCARINA_TIME);
    SHIM_ITEM_OPT_LFIST(BOOMERANG);

    shims[LINK_SHIMDL_FPS_RHAND_SLINGSHOT] = Utils_createShimDisplayList(2, &dls[LINK_DL_SLINGSHOT], &dls[LINK_DL_OPT_FPS_RHAND_SLINGSHOT]);
    shims[LINK_SHIMDL_FPS_RHAND_BOW] = Utils_createShimDisplayList(2, &dls[LINK_DL_FPS_BOW], &dls[LINK_DL_OPT_FPS_RHAND_BOW]);
    shims[LINK_SHIMDL_FPS_RHAND_HOOKSHOT] = Utils_createShimDisplayList(2, &dls[LINK_DL_FPS_HOOKSHOT], &dls[LINK_DL_OPT_FPS_RHAND_HOOKSHOT]);
    shims[LINK_SHIMDL_FPS_RHAND_LONGSHOT] = Utils_createShimDisplayList(2, &dls[LINK_DL_OPT_FPS_LONGSHOT], &dls[LINK_DL_OPT_FPS_RHAND_LONGSHOT]);

    shims[LINK_SHIMDL_SHIELD1_ITEM] = Utils_createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD1_ITEM], &dls[LINK_DL_SHIELD1], sPopModelViewMtx);
    shims[LINK_SHIMDL_SWORD3_PEDESTAL] = Utils_createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL], &dls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL], sPopModelViewMtx);
    shims[LINK_SHIMDL_SWORD3_PEDESTAL_GRABBED] = Utils_createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL_GRABBED], &dls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL_GRABBED], sPopModelViewMtx);

    shims[LINK_SHIMDL_CENTER_FLOWER_PROPELLER_CLOSED] = Utils_createShimDisplayList(2, &dls[LINK_DL_FLOWER_PROPELLER_CLOSED], &dls[LINK_DL_FLOWER_CENTER_CLOSED]);
    shims[LINK_SHIMDL_CENTER_FLOWER_PROPELLER_OPEN] = Utils_createShimDisplayList(2, &dls[LINK_DL_FLOWER_PROPELLER_OPEN], &dls[LINK_DL_FLOWER_CENTER_OPEN]);

#undef SHIM_ITEM_HAND
#undef SHIM_ITEM_LFIST
#undef SHIM_ITEM_RFIST
#undef SHIM_ITEM_LHAND
#undef SHIM_ITEM_RHAND
#undef SHIM_OPT_ITEM_HAND
#undef SHIM_OPT_ITEM_LFIST
#undef SHIM_OPT_ITEM_RFIST
#undef SHIM_OPT_ITEM_LHAND
#undef SHIM_OPT_ITEM_RHAND
#undef SHIM_HILT_BACK
#undef SHIM_SWORD_SHEATHED
#undef SHIM_SHIELD_BACK
#undef SHIM_SWORD_SHIELD_UNSHEATHED
#undef SHIM_SWORD_SHIELD_SHEATH
#undef SHIM_SWORD_SHIELD_SHEATHED
#undef SHIM_SWORD
#undef SHIM_SWORD_LFIST
#undef SHIM_SHIELD_RFIST
}

static void initBowWrapper(FormProxy *fp) {
    gSPDisplayList(&fp->wrappedDisplayLists[LINK_DL_BOW_STRING].displayList[WRAPPED_DL_PREDRAW], sStartBowStringDL);
    gSPBranchList(&fp->wrappedDisplayLists[LINK_DL_BOW_STRING].displayList[WRAPPED_DL_POSTDRAW], sEndBowstringDL);
}

static void initProxyWrappers(FormProxy *fp) {
    for (size_t i = 0; i < fp->numDLs; ++i) {
        gSPDisplayList(&fp->wrappedDisplayLists[i].displayList[WRAPPED_DL_PREDRAW], sStartDLWrapper);
        gSPDisplayList(&fp->wrappedDisplayLists[i].displayList[WRAPPED_DL_DRAW], gEmptyDL);
        gSPBranchList(&fp->wrappedDisplayLists[i].displayList[WRAPPED_DL_POSTDRAW], sEndDLWrapper);
    }

    initBowWrapper(fp);
}

static void initProxyDLs(FormProxy *fp) {
    for (size_t i = 0; i < fp->numDLs; ++i) {
        MRC_addExcludedDL(&fp->displayLists[i]);
        MRC_addExcludedDL(fp->wrappedDisplayLists[i].displayList);
        gSPBranchList(&fp->displayLists[i], fp->wrappedDisplayLists[i].displayList);
    }
}

void FormProxy_init(FormProxy *fp, PlayerProxy *pp, PlayerTransformation form, FormProxyId fpId, ModelInfo *fallback, ModelInfo *fallbackOverride) {
    if (!pp) {
        Logger_printError("Received NULL PlayerProxy argument!");
        Utils_tryCrashGame();
    }

    if (form < 0 || form >= PLAYER_FORM_MAX) {
        Logger_printError("Received invalid form argument with value %d!", form);
        Utils_tryCrashGame();
    }

    if (!fallback) {
        Logger_printError("Received NULL fallback argument!");
        Utils_tryCrashGame();
    }

    if (!fallbackOverride) {
        Logger_printError("Received NULL fallbackOverride argument!");
        Utils_tryCrashGame();
    }

    recomputil_u32_hashset_insert(sValidFormProxyPtrs, (uintptr_t)fp);

    fp->playerProxy = pp;
    fp->form = form;
    fp->fpId = fpId;
    ModelInfo_init(&fp->currentModelInfo);
    fp->fallbackOverrideModelInfo = fallbackOverride;
    fp->fallbackModelInfo = fallback;
    fp->displayListAlternates = recomputil_create_u32_value_hashmap();
    fp->tunicColor.isOverrideRequested = false;
    fp->numDLs = LINK_DL_MAX;
    fp->displayLists = recomp_alloc(fp->numDLs * sizeof(*fp->displayLists));
    fp->wrappedDisplayLists = recomp_alloc(fp->numDLs * sizeof(*fp->wrappedDisplayLists));
    initSkeleton(fp);
    initShieldingSkeleton(fp);
    initMatrixes(fp);
    initProxyShims(fp);
    initProxyWrappers(fp);
    initProxyDLs(fp);

    FormProxy_resetTunicColor(fp);
}

FormProxyId FormProxy_getFormProxyId(const FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return PlayerModelConfig_getNumFormIds();
    }

    return fp->fpId;
}

bool FormProxy_setAlternateFormProxyDL(FormProxy *fp, Link_DisplayList id, FormProxy *alt) {
    if (!isValidFormProxy(fp) || (alt && !isValidFormProxy(alt))) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (alt == fp) {
        Logger_printWarning("fp == alt! Trying to assign would create circular reference! Ignoring...");
        return false;
    }

    if (alt) {
        return recomputil_u32_value_hashmap_insert(fp->displayListAlternates, id, (uintptr_t)alt);
    }

    return FormProxy_unsetAlternateFormProxyDL(fp, id);
}

bool FormProxy_unsetAlternateFormProxyDL(FormProxy *fp, Link_DisplayList id) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return recomputil_u32_value_hashmap_erase(fp->displayListAlternates, id);
}

ModelInfo *FormProxy_getCurrentModelInfo(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return &fp->currentModelInfo;
}

ModelInfo *FormProxy_getFallbackOverrideModelInfo(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return fp->fallbackOverrideModelInfo;
}

ModelInfo *FormProxy_getFallbackModelInfo(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return fp->fallbackModelInfo;
}

bool FormProxy_setCurrentOverrideDL(FormProxy *fp, Link_DisplayList dlId, Gfx *dl) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!dl) {
        return FormProxy_unsetCurrentOverrideDL(fp, dlId);
    }

    return ModelInfo_setGfxOverride(&fp->currentModelInfo, dlId, dl);
}

bool FormProxy_unsetCurrentOverrideDL(FormProxy *fp, Link_DisplayList dlId) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return ModelInfo_unsetGfxOverride(&fp->currentModelInfo, dlId);
}

bool FormProxy_setCurrentOverrideMtx(FormProxy *fp, Link_EquipmentMatrix mtxId, Mtx *mtx) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!mtx) {
        return FormProxy_unsetCurrentOverrideMtx(fp, mtxId);
    }

    return ModelInfo_setMtxOverride(&fp->currentModelInfo, mtxId, mtx);
}

bool FormProxy_unsetCurrentOverrideMtx(FormProxy *fp, Link_EquipmentMatrix mtxId) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return ModelInfo_unsetMtxOverride(&fp->currentModelInfo, mtxId);
}

void FormProxy_refreshSkeletons(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

#define NO_DISPLAY_LIST LINK_DL_MAX

    static const Link_DisplayList limbToDLEntry[PLAYER_LIMB_COUNT] = {
        NO_DISPLAY_LIST,
        LINK_DL_WAIST,
        NO_DISPLAY_LIST,
        LINK_DL_RTHIGH,
        LINK_DL_RSHIN,
        LINK_DL_RFOOT,
        LINK_DL_LTHIGH,
        LINK_DL_LSHIN,
        LINK_DL_LFOOT,
        NO_DISPLAY_LIST,
        LINK_DL_HEAD,
        LINK_DL_HAT,
        LINK_DL_COLLAR,
        LINK_DL_LSHOULDER,
        LINK_DL_LFOREARM,
        LINK_DL_LHAND,
        LINK_DL_RSHOULDER,
        LINK_DL_RFOREARM,
        LINK_DL_RHAND,
        LINK_DL_SHEATH_NONE,
        LINK_DL_TORSO,
    };

    FlexSkeletonHeader *skel = ModelInfo_getSkeleton(&fp->currentModelInfo);

    if (!skel) {
        skel = ModelInfo_getSkeleton(fp->fallbackOverrideModelInfo);
    }

    if (!skel) {
        skel = ModelInfo_getSkeleton(fp->fallbackModelInfo);
    }

    if (skel) {
        for (int i = 0; i < PLAYER_LIMB_COUNT; ++i) {
            StandardLimb *limb = skel->sh.segment[i];
            fp->skeleton.limbs[i].child = limb->child;
            fp->skeleton.limbs[i].sibling = limb->sibling;
            fp->skeleton.limbs[i].jointPos = limb->jointPos;

            if (!limb->dList) {
                fp->skeleton.limbs[i].dLists[0] = NULL;
                fp->skeleton.limbs[i].dLists[1] = NULL;
            } else {
                Link_DisplayList id = limbToDLEntry[i];
                if (id != NO_DISPLAY_LIST) {
                    fp->skeleton.limbs[i].dLists[0] = &fp->displayLists[id];
                    fp->skeleton.limbs[i].dLists[1] = &fp->displayLists[id];
                }
            }
        }

        fp->skeleton.flexSkeleton.dListCount = skel->dListCount;
    }

    FlexSkeletonHeader *shieldSkel = ModelInfo_getShieldingSkeleton(&fp->currentModelInfo);

    if (!shieldSkel) {
        ModelInfo_getShieldingSkeleton(fp->fallbackOverrideModelInfo);
    }

    if (!shieldSkel) {
        shieldSkel = ModelInfo_getShieldingSkeleton(fp->fallbackModelInfo);
    }

    if (shieldSkel) {
        for (int i = 0; i < PLAYER_BODY_SHIELD_LIMB_COUNT; ++i) {
            StandardLimb *limb = shieldSkel->sh.segment[i];
            fp->shieldingSkeleton.limbs[i].child = limb->child;
            fp->shieldingSkeleton.limbs[i].sibling = limb->sibling;
            fp->shieldingSkeleton.limbs[i].jointPos = limb->jointPos;
        }

        fp->shieldingSkeleton.flexSkeleton.dListCount = shieldSkel->dListCount;
    }

#undef NO_DISPLAY_LIST
}

void FormProxy_refreshDL(FormProxy *fp, Link_DisplayList id) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    Gfx *dl = FormProxy_getDL(fp, id);

    if (dl) {
        gSPDisplayList(&fp->wrappedDisplayLists[id].displayList[WRAPPED_DL_DRAW], dl);
    }
}

static void setDLsToShims(FormProxy *fp) {

#define PROXY_TO_SHIM(dlName) gSPDisplayList(&fp->wrappedDisplayLists[LINK_DL_##dlName].displayList[WRAPPED_DL_DRAW], fp->shimDisplayListPtrs[LINK_SHIMDL_##dlName])

    PROXY_TO_SHIM(SWORD1);
    PROXY_TO_SHIM(SWORD2);
    PROXY_TO_SHIM(SWORD3);
    PROXY_TO_SHIM(SWORD4);
    PROXY_TO_SHIM(SWORD4_BROKEN);
    PROXY_TO_SHIM(SWORD5);

    PROXY_TO_SHIM(SWORD1_HILT_BACK);
    PROXY_TO_SHIM(SWORD2_HILT_BACK);
    PROXY_TO_SHIM(SWORD3_HILT_BACK);
    PROXY_TO_SHIM(SWORD4_HILT_BACK);
    PROXY_TO_SHIM(SWORD5_HILT_BACK);

    PROXY_TO_SHIM(SWORD1_SHEATHED);
    PROXY_TO_SHIM(SWORD2_SHEATHED);
    PROXY_TO_SHIM(SWORD3_SHEATHED);
    PROXY_TO_SHIM(SWORD4_SHEATHED);
    PROXY_TO_SHIM(SWORD5_SHEATHED);

    PROXY_TO_SHIM(SHIELD1_BACK);
    PROXY_TO_SHIM(SHIELD2_BACK);
    PROXY_TO_SHIM(SHIELD3_BACK);

    PROXY_TO_SHIM(SWORD1_SHIELD1_SHEATH);
    PROXY_TO_SHIM(SWORD1_SHIELD2_SHEATH);
    PROXY_TO_SHIM(SWORD1_SHIELD3_SHEATH);
    PROXY_TO_SHIM(SWORD2_SHIELD1_SHEATH);
    PROXY_TO_SHIM(SWORD2_SHIELD2_SHEATH);
    PROXY_TO_SHIM(SWORD2_SHIELD3_SHEATH);
    PROXY_TO_SHIM(SWORD3_SHIELD1_SHEATH);
    PROXY_TO_SHIM(SWORD3_SHIELD2_SHEATH);
    PROXY_TO_SHIM(SWORD3_SHIELD3_SHEATH);
    PROXY_TO_SHIM(SWORD4_SHIELD1_SHEATH);
    PROXY_TO_SHIM(SWORD4_SHIELD2_SHEATH);
    PROXY_TO_SHIM(SWORD4_SHIELD3_SHEATH);
    PROXY_TO_SHIM(SWORD5_SHIELD1_SHEATH);
    PROXY_TO_SHIM(SWORD5_SHIELD2_SHEATH);
    PROXY_TO_SHIM(SWORD5_SHIELD3_SHEATH);

    PROXY_TO_SHIM(SWORD1_SHIELD1_SHEATHED);
    PROXY_TO_SHIM(SWORD1_SHIELD2_SHEATHED);
    PROXY_TO_SHIM(SWORD1_SHIELD3_SHEATHED);
    PROXY_TO_SHIM(SWORD2_SHIELD1_SHEATHED);
    PROXY_TO_SHIM(SWORD2_SHIELD2_SHEATHED);
    PROXY_TO_SHIM(SWORD2_SHIELD3_SHEATHED);
    PROXY_TO_SHIM(SWORD3_SHIELD1_SHEATHED);
    PROXY_TO_SHIM(SWORD3_SHIELD2_SHEATHED);
    PROXY_TO_SHIM(SWORD3_SHIELD3_SHEATHED);
    PROXY_TO_SHIM(SWORD4_SHIELD1_SHEATHED);
    PROXY_TO_SHIM(SWORD4_SHIELD2_SHEATHED);
    PROXY_TO_SHIM(SWORD4_SHIELD3_SHEATHED);
    PROXY_TO_SHIM(SWORD5_SHIELD1_SHEATHED);
    PROXY_TO_SHIM(SWORD5_SHIELD2_SHEATHED);
    PROXY_TO_SHIM(SWORD5_SHIELD3_SHEATHED);

    PROXY_TO_SHIM(LFIST_SWORD1);
    PROXY_TO_SHIM(LFIST_SWORD2);
    PROXY_TO_SHIM(LFIST_SWORD3);
    PROXY_TO_SHIM(LFIST_SWORD3_PEDESTAL_GRABBED);
    PROXY_TO_SHIM(LFIST_SWORD4);
    PROXY_TO_SHIM(LFIST_SWORD4_BROKEN);
    PROXY_TO_SHIM(LFIST_SWORD5);

    PROXY_TO_SHIM(RFIST_SHIELD1);
    PROXY_TO_SHIM(RFIST_SHIELD2);
    PROXY_TO_SHIM(RFIST_SHIELD3);

    PROXY_TO_SHIM(LFIST_HAMMER);
    PROXY_TO_SHIM(LFIST_BOOMERANG);
    PROXY_TO_SHIM(RFIST_BOW);
    PROXY_TO_SHIM(RFIST_SLINGSHOT);
    PROXY_TO_SHIM(RFIST_HOOKSHOT);
    PROXY_TO_SHIM(RFIST_LONGSHOT);
    PROXY_TO_SHIM(RHAND_OCARINA_FAIRY);
    PROXY_TO_SHIM(RHAND_OCARINA_TIME);

    PROXY_TO_SHIM(FPS_RHAND_BOW);
    PROXY_TO_SHIM(FPS_RHAND_SLINGSHOT);
    PROXY_TO_SHIM(FPS_RHAND_HOOKSHOT);
    PROXY_TO_SHIM(FPS_RHAND_LONGSHOT);

    PROXY_TO_SHIM(SHIELD1_ITEM);
    PROXY_TO_SHIM(SWORD3_PEDESTAL);
    PROXY_TO_SHIM(SWORD3_PEDESTAL_GRABBED);

    PROXY_TO_SHIM(CENTER_FLOWER_PROPELLER_CLOSED);
    PROXY_TO_SHIM(CENTER_FLOWER_PROPELLER_OPEN);

#undef PROXY_TO_SHIM
}

void FormProxy_refreshAllDLs(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    WrappedDisplayList *wDLs = fp->wrappedDisplayLists;
    ModelInfo *current = &fp->currentModelInfo;
    ModelInfo *fallbackOverride = fp->fallbackOverrideModelInfo;
    ModelInfo *fallback = fp->fallbackModelInfo;

    for (size_t i = 0; i < fp->numDLs; ++i) {
        gSPDisplayList(&wDLs[i].displayList[WRAPPED_DL_DRAW], gEmptyDL);
    }

    setDLsToShims(fp);

    for (size_t i = 0; i < fp->numDLs; ++i) {
        FormProxy_refreshDL(fp, i);
    }

    // first person hookshot workaround
    Gfx *listenerHookDL = MRC_getListenerDL(fp->form, LINK_DL_HOOKSHOT);
    if (listenerHookDL) {
        gSPDisplayList(&wDLs[LINK_DL_FPS_HOOKSHOT].displayList[WRAPPED_DL_DRAW], listenerHookDL);
    }

    // first person bow workaround
    Gfx *listenerBowDL = MRC_getListenerDL(fp->form, LINK_DL_BOW);
    if (listenerBowDL) {
        gSPDisplayList(&wDLs[LINK_DL_BOW].displayList[WRAPPED_DL_DRAW], listenerBowDL);
    }

    // disable hilt for Great Fairy Sword if replaced
    Gfx *listenerGFSwordDL = MRC_getListenerDL(fp->form, LINK_DL_SWORD_GREAT_FAIRY_BLADE);
    if (listenerGFSwordDL) {
        gSPDisplayList(&wDLs[LINK_DL_SWORD_GREAT_FAIRY_HILT].displayList[WRAPPED_DL_DRAW], gEmptyDL);
    }

    // disable hilt for Fierce Deity's Sword if replaced
    Gfx *listenerFDSwordDL = MRC_getListenerDL(fp->form, LINK_DL_SWORD_FIERCE_DEITY_BLADE);
    if (listenerFDSwordDL) {
        gSPDisplayList(&wDLs[LINK_DL_SWORD_GREAT_FAIRY_HILT].displayList[WRAPPED_DL_DRAW], gEmptyDL);
    }
}

void FormProxy_refreshMtx(FormProxy *fp, Link_EquipmentMatrix id) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    Mtx *matrix = ModelInfo_getMtx(&fp->currentModelInfo, id);

    if (!matrix) {
        matrix = ModelInfo_getMtx(fp->fallbackOverrideModelInfo, id);
    }

    if (!matrix) {
        matrix = ModelInfo_getMtx(fp->fallbackModelInfo, id);
    }

    if (!matrix) {
        matrix = ModelEntry_getMatrix(ModelEntryForm_getModelEntry(gSharedModelEntry), id);
    }

    if (!matrix) {
        matrix = &gIdentityMtx;
    }

    gSPMatrix(&fp->mtxDisplayLists[id][0], matrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
}

void FormProxy_refreshAllMtxs(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        FormProxy_refreshMtx(fp, i);
    }
}

void FormProxy_requestTunicColorOverride(FormProxy *fp, Color_RGBA8 color) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    fp->tunicColor.isOverrideRequested = true;
    fp->tunicColor.requested = color;
}

bool FormProxy_isTunicColorOverrideRequested(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return fp->tunicColor.isOverrideRequested;
}

Color_RGBA8 FormProxy_getRequestedTunicColor(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return (Color_RGBA8){0, 0, 0, 0};
    }

    return fp->tunicColor.requested;
}

Color_RGBA8 FormProxy_getCurrentTunicColor(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return (Color_RGBA8){0, 0, 0, 0};
    }

    return fp->tunicColor.current;
}

void FormProxy_resetTunicColor(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    fp->tunicColor.current = (Color_RGBA8){30, 105, 27, 0};
}

// Convert char to its numeric value
// Returns 0xFF if invalid character
static u8 cToNum(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    return 0xFF;
}

static u8 sToU8(const char *s) {
    return (cToNum(s[0]) << 4) | cToNum(s[1]);
}

static bool isValidHexString(const char *s) {
    bool isValid = true;

    size_t len = 0;

    while (*s != '\0' && isValid) {
        if (len > 6 || cToNum(*s) == 0xFF) {
            isValid = false;
        }
        s++;
        len++;
    }

    if (len < 6) {
        isValid = false;
    }

    return isValid;
}

typedef enum TunicColorConfigOption {
    TUNIC_COLOR_OFF,
    TUNIC_COLOR_AUTO,
    TUNIC_COLOR_FORCE,
} TunicColorConfigOption;

void FormProxy_pullCurrentTunicColorFromConfig(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    char *color = recomp_get_config_string("tunic_color");

    if (color) {
        if (isValidHexString(color)) {
            fp->tunicColor.current.r = sToU8(color);
            fp->tunicColor.current.g = sToU8(color + 2);
            fp->tunicColor.current.b = sToU8(color + 4);
        } else {
            FormProxy_resetTunicColor(fp);
        }

        recomp_free_config_string(color);
    }
}

void FormProxy_pullCurrentTunicColorFromRequested(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    fp->tunicColor.current = fp->tunicColor.requested;
}

void FormProxy_setCurrentModelFormEntry(FormProxy *fp, ModelEntryForm *modelEntry) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    ModelInfo_setModelEntryForm(&fp->currentModelInfo, modelEntry);
}

void FormProxy_refreshPlayerFaceTextures(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    extern TexturePtr sPlayerEyesTextures[];
    extern TexturePtr sPlayerMouthTextures[];

    ModelInfo *current = &fp->currentModelInfo;
    ModelInfo *fallbackOverride = fp->fallbackOverrideModelInfo;
    ModelInfo *fallback = fp->fallbackModelInfo;

    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        TexturePtr eyesTex = ModelInfo_getEyesTexture(current, i);

        if (!eyesTex) {
            eyesTex = ModelInfo_getEyesTexture(fallbackOverride, i);
        }

        if (!eyesTex) {
            eyesTex = ModelInfo_getEyesTexture(fallback, i);
        }

        sPlayerEyesTextures[i] = eyesTex;
    }

    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        TexturePtr mouthTex = ModelInfo_getMouthTexture(current, i);

        if (!mouthTex) {
            mouthTex = ModelInfo_getMouthTexture(fallbackOverride, i);
        }

        if (!mouthTex) {
            mouthTex = ModelInfo_getMouthTexture(fallback, i);
        }

        sPlayerMouthTextures[i] = mouthTex;
    }
}

void FormProxy_refresh(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    FormProxy_refreshSkeletons(fp);
    FormProxy_refreshAllDLs(fp);
    FormProxy_refreshAllMtxs(fp);
}

Mtx *FormProxy_getMatrix(FormProxy *fp, Link_EquipmentMatrix id) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return (Mtx *)fp->mtxDisplayLists[id][0].words.w1;
}

static U32ValueHashmapHandle sDLToAltLists;

typedef struct DisplayListAltList {
    Link_DisplayList target;
    Link_DisplayList *alternates;
    size_t numAlts;
} DisplayListAltList;

#define DECLARE_ALT_LIST(dl, altArr) {.target = dl, .alternates = altArr, .numAlts = ARRAY_COUNT(altArr)}

static Link_DisplayList sFpsHookshotAlts[] = {LINK_DL_HOOKSHOT};
static Link_DisplayList sLongshotAlts[] = {LINK_DL_HOOKSHOT};
static Link_DisplayList sLongshotChainAlts[] = {LINK_DL_HOOKSHOT_CHAIN};
static Link_DisplayList sLongshotHookAlts[] = {LINK_DL_HOOKSHOT_HOOK};
static Link_DisplayList sLongshotReticleAlts[] = {LINK_DL_HOOKSHOT_RETICLE};
static Link_DisplayList sFpsLongshotAlts[] = {LINK_DL_OPT_LONGSHOT, LINK_DL_FPS_HOOKSHOT, LINK_DL_HOOKSHOT};
static Link_DisplayList sFpsBowAlts[] = {LINK_DL_BOW};
static Link_DisplayList sFpsSlingshotAlts[] = {LINK_DL_SLINGSHOT};
static Link_DisplayList sFpsAlts[] = {LINK_DL_SLINGSHOT};
static Link_DisplayList sLeftFistSwordAlts[] = {LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistSword1Alts[] = {LINK_DL_OPT_LFIST_SWORD, LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistSword2Alts[] = {LINK_DL_OPT_LFIST_SWORD, LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistSword3Alts[] = {LINK_DL_OPT_LFIST_SWORD, LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistSword4Alts[] = {LINK_DL_OPT_LFIST_SWORD_TWO_HANDED, LINK_DL_OPT_LFIST_SWORD, LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistSword5Alts[] = {LINK_DL_OPT_LFIST_SWORD_TWO_HANDED, LINK_DL_OPT_LFIST_SWORD, LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistSword2HandAlts[] = {LINK_DL_OPT_LFIST_SWORD, LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistDekuStickAlts[] = {LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistHammerAlts[] = {LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sLeftFistBoomerangAlts[] = {LINK_DL_OPT_LFIST_ITEM, LINK_DL_LFIST};
static Link_DisplayList sRightHandOcarinaAlts[] = {LINK_DL_RHAND};
static Link_DisplayList sRightHandOcarinaFairyAlts[] = {LINK_DL_OPT_RHAND_OCARINA, LINK_DL_RHAND};
static Link_DisplayList sRightHandOcarinaTimeAlts[] = {LINK_DL_OPT_RHAND_OCARINA, LINK_DL_RHAND};
static Link_DisplayList sRightFistShieldAlts[] = {LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistShield1Alts[] = {LINK_DL_OPT_RFIST_SHIELD, LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistShield2Alts[] = {LINK_DL_OPT_RFIST_SHIELD, LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistShield3Alts[] = {LINK_DL_OPT_RFIST_SHIELD, LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistSwordTwoHandedAlts[] = {LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistSword4Alts[] = {LINK_DL_OPT_LFIST_SWORD_TWO_HANDED, LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistSword5Alts[] = {LINK_DL_OPT_LFIST_SWORD_TWO_HANDED, LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistDekuStickAlts[] = {LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistBowAlts[] = {LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistSlingshotAlts[] = {LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistHookshotAlts[] = {LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sRightFistLongshotAlts[] = {LINK_DL_OPT_RFIST_ITEM, LINK_DL_RFIST};
static Link_DisplayList sLeftShoulderHookshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LSHOULDER, LINK_DL_LSHOULDER};
static Link_DisplayList sLeftForearmHookshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LFOREARM, LINK_DL_LFOREARM};
static Link_DisplayList sLeftHandHookshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LHAND, LINK_DL_LHAND};
static Link_DisplayList sRightShoulderHookshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RSHOULDER, LINK_DL_RSHOULDER};
static Link_DisplayList sRightForearmHookshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RFOREARM, LINK_DL_RFOREARM};
static Link_DisplayList sRightHandHookshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RHAND, LINK_DL_RHAND};
static Link_DisplayList sLeftShoulderLongshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LSHOULDER_HOOKSHOT, LINK_DL_OPT_FPS_LSHOULDER, LINK_DL_LSHOULDER};
static Link_DisplayList sLeftForearmLongshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LFOREARM_HOOKSHOT, LINK_DL_OPT_FPS_LFOREARM, LINK_DL_LFOREARM};
static Link_DisplayList sLeftHandLongshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LHAND_HOOKSHOT, LINK_DL_OPT_FPS_LHAND, LINK_DL_LHAND};
static Link_DisplayList sRightShoulderLongshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RSHOULDER_HOOKSHOT, LINK_DL_OPT_FPS_RSHOULDER, LINK_DL_RSHOULDER};
static Link_DisplayList sRightForearmLongshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RFOREARM_HOOKSHOT, LINK_DL_OPT_FPS_RFOREARM, LINK_DL_RFOREARM};
static Link_DisplayList sRightHandLongshotFirstPersonAlts[] = {LINK_DL_FPS_RHAND_HOOKSHOT, LINK_DL_OPT_FPS_RHAND, LINK_DL_RHAND};
static Link_DisplayList sLeftShoulderBowFirstPersonAlts[] = {LINK_DL_OPT_FPS_LSHOULDER, LINK_DL_LSHOULDER};
static Link_DisplayList sLeftForearmBowFirstPersonAlts[] = {LINK_DL_OPT_FPS_LFOREARM, LINK_DL_LFOREARM};
static Link_DisplayList sLeftHandBowFirstPersonAlts[] = {LINK_DL_OPT_FPS_LHAND, LINK_DL_LHAND};
static Link_DisplayList sRightShoulderBowFirstPersonAlts[] = {LINK_DL_OPT_FPS_RSHOULDER, LINK_DL_RSHOULDER};
static Link_DisplayList sRightForearmBowFirstPersonAlts[] = {LINK_DL_OPT_FPS_RFOREARM, LINK_DL_RFOREARM};
static Link_DisplayList sRightHandBowFirstPersonAlts[] = {LINK_DL_OPT_FPS_RHAND, LINK_DL_RHAND};
static Link_DisplayList sLeftShoulderSlingshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LSHOULDER, LINK_DL_LSHOULDER};
static Link_DisplayList sLeftForearmSlingshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LFOREARM, LINK_DL_LFOREARM};
static Link_DisplayList sLeftHandSlingshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_LHAND, LINK_DL_LHAND};
static Link_DisplayList sRightShoulderSlingshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RSHOULDER, LINK_DL_RSHOULDER};
static Link_DisplayList sRightForearmSlingshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RFOREARM, LINK_DL_RFOREARM};
static Link_DisplayList sRightHandSlingshotFirstPersonAlts[] = {LINK_DL_OPT_FPS_RHAND, LINK_DL_RHAND};

static DisplayListAltList sDisplayListAlts[] = {
    DECLARE_ALT_LIST(LINK_DL_FPS_HOOKSHOT, sFpsHookshotAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LONGSHOT, sLongshotAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LONGSHOT_CHAIN, sLongshotChainAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LONGSHOT_HOOK, sLongshotHookAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LONGSHOT_RETICLE, sLongshotReticleAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LONGSHOT, sFpsLongshotAlts),
    DECLARE_ALT_LIST(LINK_DL_FPS_BOW, sFpsBowAlts),
    DECLARE_ALT_LIST(LINK_DL_FPS_SLINGSHOT, sFpsSlingshotAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_SWORD, sLeftFistSwordAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_SWORD1, sLeftFistSword1Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_SWORD2, sLeftFistSword2Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_SWORD3, sLeftFistSword3Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_SWORD_TWO_HANDED, sLeftFistSword2HandAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_SWORD4, sRightFistSword4Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_SWORD5, sRightFistSword5Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_DEKU_STICK, sLeftFistDekuStickAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_HAMMER, sLeftFistHammerAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_LFIST_BOOMERANG, sLeftFistBoomerangAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RHAND_OCARINA, sRightHandOcarinaAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RHAND_OCARINA_FAIRY, sRightHandOcarinaFairyAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RHAND_OCARINA_TIME, sRightHandOcarinaTimeAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_SHIELD, sRightFistShieldAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_SHIELD1, sRightFistShield1Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_SHIELD2, sRightFistShield2Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_SHIELD3, sRightFistShield3Alts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_SWORD_TWO_HANDED, sRightFistSwordTwoHandedAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_DEKU_STICK, sRightFistDekuStickAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_BOW, sRightFistBowAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_SLINGSHOT, sRightFistSlingshotAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_HOOKSHOT, sRightFistHookshotAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LSHOULDER_HOOKSHOT, sLeftShoulderHookshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LFOREARM_HOOKSHOT, sLeftForearmHookshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LHAND_HOOKSHOT, sLeftHandHookshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RSHOULDER_HOOKSHOT, sRightShoulderHookshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RFOREARM_HOOKSHOT, sRightForearmHookshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RHAND_HOOKSHOT, sRightHandHookshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_RFIST_LONGSHOT, sRightFistLongshotAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LSHOULDER_LONGSHOT, sLeftShoulderLongshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LFOREARM_LONGSHOT, sLeftForearmLongshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LHAND_LONGSHOT, sLeftHandLongshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RSHOULDER_LONGSHOT, sRightShoulderLongshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RFOREARM_LONGSHOT, sRightForearmLongshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RHAND_LONGSHOT, sRightHandLongshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LSHOULDER_BOW, sLeftShoulderBowFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LFOREARM_BOW, sLeftForearmBowFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LHAND_BOW, sLeftHandBowFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RSHOULDER_BOW, sRightShoulderBowFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RFOREARM_BOW, sRightForearmBowFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RHAND_BOW, sRightHandBowFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LSHOULDER_SLINGSHOT, sLeftShoulderSlingshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LFOREARM_SLINGSHOT, sLeftForearmSlingshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_LHAND_SLINGSHOT, sLeftHandSlingshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RSHOULDER_SLINGSHOT, sRightShoulderSlingshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RFOREARM_SLINGSHOT, sRightForearmSlingshotFirstPersonAlts),
    DECLARE_ALT_LIST(LINK_DL_OPT_FPS_RHAND_SLINGSHOT, sRightHandSlingshotFirstPersonAlts),
};

static DisplayListAltList *getAltList(Link_DisplayList dlId) {
    uintptr_t out = (uintptr_t)NULL;

    recomputil_u32_value_hashmap_get(sDLToAltLists, dlId, &out);

    return (DisplayListAltList *)out;
}

static Gfx *getDLOrAltFromModelEntry(ModelEntry *me, Link_DisplayList dlId) {
    Gfx *result = ModelEntry_getDisplayList(me, dlId);

    if (!result) {
        DisplayListAltList *alts = getAltList(dlId);

        if (alts) {
            for (size_t i = 0; i < alts->numAlts && !result; ++i) {
                Link_DisplayList curr = alts->alternates[i];

                result = ModelEntry_getDisplayList(me, curr);
            }
        }
    }

    return result;
}

static Gfx *getDLOrAltFromModelInfo(ModelInfo *mi, Link_DisplayList dlId) {
    Gfx *result = ModelInfo_getGfx(mi, dlId);

    if (!result) {
        DisplayListAltList *alts = getAltList(dlId);

        if (alts) {
            for (size_t i = 0; i < alts->numAlts && !result; ++i) {
                Link_DisplayList curr = alts->alternates[i];

                result = ModelInfo_getGfx(mi, curr);
            }
        }
    }

    return result;
}

Gfx *FormProxy_getDL(FormProxy *fp, Link_DisplayList id) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    Gfx *dl = MRC_getListenerDL(fp->form, id);

    if (!dl) {
        FormProxy *fpAlt;
        if (recomputil_u32_value_hashmap_get(fp->displayListAlternates, id, (uintptr_t *)&fpAlt)) {
            if (fpAlt && fpAlt != fp) {
                dl = FormProxy_getDL(fpAlt, id);
            }
        }
    }

    ModelInfo *current = &fp->currentModelInfo;
    ModelInfo *fallbackOverride = fp->fallbackOverrideModelInfo;
    ModelInfo *fallback = fp->fallbackModelInfo;

    if (!dl) {
        dl = getDLOrAltFromModelInfo(current, id);
    }

    if (!dl) {
        dl = getDLOrAltFromModelInfo(fallbackOverride, id);
    }

    if (!dl) {
        dl = getDLOrAltFromModelInfo(fallback, id);
    }

    if (!dl) {
        dl = getDLOrAltFromModelEntry(ModelEntryForm_getModelEntry(gSharedModelEntry), id);
    }

    return dl;
}

Gfx *FormProxy_getCurrentDL(FormProxy *fp, Link_DisplayList id) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return &fp->displayLists[id];
}

FlexSkeletonHeader *FormProxy_getSkeleton(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return &fp->skeleton.flexSkeleton;
}

FlexSkeletonHeader *FormProxy_getShieldingSkeleton(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return &fp->shieldingSkeleton.flexSkeleton;
}

bool FormProxy_isAdultModelType(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    ModelEntryForm *curr = ModelInfo_getModelEntryForm(&fp->currentModelInfo);

    if (curr) {
        ModelEntry *currModelEntry = ModelEntryForm_getModelEntry(curr);

        if (currModelEntry) {
            return ModelEntry_isAnyFlagEnabled(currModelEntry, MODELENTRY_FLAG_IS_ADULT);
        }
    }

    return false;
}

PlayerProxy *FormProxy_getPlayerProxy(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return fp->playerProxy;
}

PlayerTransformation FormProxy_getTargetForm(FormProxy *fp) {
    if (!isValidFormProxy(fp)) {
        PRINT_INVALID_PTR_ERR();
        return PLAYER_FORM_MAX;
    }

    return fp->form;
}

static void setupAltLists() {
    sDLToAltLists = recomputil_create_u32_value_hashmap();

    for (int i = 0; i < ARRAY_COUNT(sDisplayListAlts); ++i) {
        DisplayListAltList *curr = &sDisplayListAlts[i];

        if (!recomputil_u32_value_hashmap_insert(sDLToAltLists, curr->target, (uintptr_t)curr)) {
            Logger_printWarning("Duplicate target with value of %d inserted into sDLToAltLists!", curr->target);
        }
    }
}

RECOMP_CALLBACK(".", _internal_initHashObjects)
void initFormProxyObjects() {
    sValidFormProxyPtrs = recomputil_create_u32_hashset();
    setupAltLists();
}
