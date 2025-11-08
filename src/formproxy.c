#include "global.h"
#include "formproxy.h"
#include "logger.h"
#include "playermodelmanager_utils.h"

static Gfx sPopModelViewMtx[] = {
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};

static void initSkeleton(FormProxy *formProxy) {
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

#define SET_LIMB_DL(playerLimb, proxyLimbName) skel->limbs[playerLimb - 1].dLists[0] = skel->limbs[playerLimb - 1].dLists[1] = &formProxy->displayLists[proxyLimbName]

    SkeletonProxy *skel = &formProxy->skeleton;
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

static void initShieldingSkeleton(FormProxy *formProxy) {
    static const StandardLimb shieldingSkeletonBase[] = {
        {{0, 0, 0}, LINK_BODY_SHIELD_LIMB_BODY - 1, LIMB_DONE, NULL},
        {{0, 2000, -800}, LINK_BODY_SHIELD_LIMB_HEAD - 1, LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS - 1, gEmptyDL},
        {{2600, 0, 0}, LIMB_DONE, LIMB_DONE, gEmptyDL},
        {{0, 0, 0}, LIMB_DONE, LIMB_DONE, gEmptyDL},
    };

    ShieldingSkeletonProxy *skel = &formProxy->shieldingSkeleton;
    FlexSkeletonHeader *flex = &skel->flexSkeleton;

    flex->sh.limbCount = PLAYER_BODY_SHIELD_LIMB_COUNT;
    flex->dListCount = 3;

    for (int i = 0; i < PLAYER_BODY_SHIELD_LIMB_COUNT; ++i) {
        skel->limbs[i] = shieldingSkeletonBase[i];
        skel->limbPtrs[i] = &skel->limbs[i];
    }

    skel->limbs[LINK_BODY_SHIELD_LIMB_BODY - 1].dList = &formProxy->displayLists[LINK_DL_BODY_SHIELD_BODY];
    skel->limbs[LINK_BODY_SHIELD_LIMB_HEAD - 1].dList = &formProxy->displayLists[LINK_DL_BODY_SHIELD_HEAD];
    skel->limbs[LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS - 1].dList = &formProxy->displayLists[LINK_DL_BODY_SHIELD_ARMS_AND_LEGS];

    flex->sh.segment = (void **)skel->limbPtrs;
}

static void initMatrixes(FormProxy *formProxy) {
    Gfx *mtxDLs = recomp_alloc(sizeof(Gfx) * 2 * LINK_EQUIP_MATRIX_MAX);

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        formProxy->mtxDisplayLists[i] = &mtxDLs[i * 2];

        gSPMatrix(&formProxy->mtxDisplayLists[i][0], &gIdentityMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPEndDisplayList(&formProxy->mtxDisplayLists[i][1]);
    }
}

static void initProxyShims(FormProxy *formProxy) {
    Gfx *dls = formProxy->displayLists;
    Gfx **mtxDls = formProxy->mtxDisplayLists;
    Gfx **shims = formProxy->shimDisplayListPtrs;

    // init by pointing all to DF command
    for (int i = 0; i < LINK_SHIMDL_MAX; ++i) {
        shims[i] = recomp_alloc(sizeof(Gfx));
        gSPBranchList(shims[i], gEmptyDL);
    }

#define SHIM_ITEM_HAND(hand, item) shims[LINK_SHIMDL_##hand##_##item] = createShimDisplayList(2, &dls[LINK_DL_##item], &dls[LINK_DL_##hand])
#define SHIM_ITEM_LFIST(item) SHIM_ITEM_HAND(LFIST, item)
#define SHIM_ITEM_RFIST(item) SHIM_ITEM_HAND(RFIST, item)
#define SHIM_ITEM_LHAND(item) SHIM_ITEM_HAND(LHAND, item)
#define SHIM_ITEM_RHAND(item) SHIM_ITEM_HAND(RHAND, item)
#define SHIM_HILT_BACK(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_HILT_BACK] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK], &dls[LINK_DL_SWORD##swordNum##_HILT], sPopModelViewMtx)
#define SHIM_SWORD_SHEATHED(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHEATHED] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SWORD##swordNum##_HILT_BACK])
#define SHIM_SHIELD_BACK(shieldNum) shims[LINK_SHIMDL_SHIELD##shieldNum##_BACK] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD##shieldNum##_BACK], &dls[LINK_DL_SHIELD##shieldNum], sPopModelViewMtx)
#define SHIM_SWORD_SHIELD_UNSHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_UNSHEATHED] = createShimDisplayList(1, &dls[LINK_DL_SWORD##swordNum##_SHEATH])
#define SHIM_SWORD_SHIELD_SHEATH(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATH] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD_SHIELD_SHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATHED] = createShimDisplayList(3, &dls[LINK_DL_SWORD##swordNum##_SHEATHED], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD(swordNum) shims[LINK_SHIMDL_SWORD##swordNum] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_HILT], &dls[LINK_DL_SWORD##swordNum##_BLADE])
#define SHIM_SWORD_LFIST(swordNum) SHIM_ITEM_LFIST(SWORD##swordNum)
#define SHIM_SHIELD_RFIST(shieldNum) SHIM_ITEM_RFIST(SHIELD##shieldNum)

    SHIM_SWORD(1);
    SHIM_SWORD(2);
    SHIM_SWORD(3);
    SHIM_SWORD(4);
    shims[LINK_SHIMDL_SWORD4_BROKEN] = createShimDisplayList(2, &dls[LINK_DL_SWORD4_HILT], &dls[LINK_DL_SWORD4_BLADE_BROKEN]);
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

    SHIM_SWORD_LFIST(1);
    SHIM_SWORD_LFIST(2);
    SHIM_SWORD_LFIST(3);
    SHIM_SWORD_LFIST(3_PEDESTAL_GRABBED);
    SHIM_SWORD_LFIST(4);
    SHIM_SWORD_LFIST(4_BROKEN);
    SHIM_SWORD_LFIST(5);

    SHIM_SHIELD_RFIST(1);
    SHIM_SHIELD_RFIST(2);
    SHIM_SHIELD_RFIST(3);

    SHIM_ITEM_LFIST(HAMMER);
    SHIM_ITEM_RFIST(BOW);
    SHIM_ITEM_RFIST(HOOKSHOT);
    SHIM_ITEM_RHAND(OCARINA_FAIRY);
    SHIM_ITEM_RHAND(OCARINA_TIME);
    SHIM_ITEM_LFIST(BOOMERANG);

    shims[LINK_SHIMDL_FPS_RHAND_SLINGSHOT] = createShimDisplayList(2, &dls[LINK_DL_SLINGSHOT], &dls[LINK_DL_FPS_RHAND]);
    shims[LINK_SHIMDL_FPS_RHAND_BOW] = createShimDisplayList(2, &dls[LINK_DL_FPS_BOW], &dls[LINK_DL_FPS_RHAND]);
    shims[LINK_SHIMDL_FPS_RHAND_HOOKSHOT] = createShimDisplayList(2, &dls[LINK_DL_FPS_HOOKSHOT], &dls[LINK_DL_FPS_RHAND]);

    shims[LINK_SHIMDL_SHIELD1_ITEM] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD1_ITEM], &dls[LINK_DL_SHIELD1], sPopModelViewMtx);
    shims[LINK_SHIMDL_SWORD3_PEDESTAL] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL], &dls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL], sPopModelViewMtx);
    shims[LINK_SHIMDL_SWORD3_PEDESTAL_GRABBED] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL_GRABBED], &dls[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL_GRABBED], sPopModelViewMtx);

    shims[LINK_SHIMDL_CENTER_FLOWER_PROPELLER_CLOSED] = createShimDisplayList(2, &dls[LINK_DL_FLOWER_PROPELLER_CLOSED], &dls[LINK_DL_FLOWER_CENTER_CLOSED]);
    shims[LINK_SHIMDL_CENTER_FLOWER_PROPELLER_OPEN] = createShimDisplayList(2, &dls[LINK_DL_FLOWER_PROPELLER_OPEN], &dls[LINK_DL_FLOWER_CENTER_OPEN]);

#undef SHIM_ITEM_HAND
#undef SHIM_ITEM_LFIST
#undef SHIM_ITEM_RFIST
#undef SHIM_ITEM_LHAND
#undef SHIM_ITEM_RHAND
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

void FormProxy_init(FormProxy *fp, ModelInfo *current, ModelInfo *fallback, PlayerTransformation form) {
    if (!current) {
        Logger_printError("PlayerModelManager: FormProxy_init received NULL current argument!");
        tryCrashGame();
    }

    if (!fallback) {
        Logger_printError("PlayerModelManager: FormProxy_init received NULL fallback argument!");
        tryCrashGame();
    }

    if (form < 0 || form >= PLAYER_FORM_MAX) {
        Logger_printError("PlayerModelManager: FormProxy_init received invalid form argument with value %d!", form);
        tryCrashGame();
    }

    fp->form = form;
    fp->currentModelInfo = current;
    fp->fallbackModelInfo = fallback;
    initSkeleton(fp);
    initShieldingSkeleton(fp);
    initMatrixes(fp);
}
