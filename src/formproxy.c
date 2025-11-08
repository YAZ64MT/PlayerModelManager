#include "global.h"
#include "modding.h"
#include "formproxy.h"
#include "logger.h"
#include "playermodelmanager_utils.h"
#include "rt64_extended_gbi.h"

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

RECOMP_CALLBACK("*", recomp_on_init)
void initExDLs() {
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

static void initProxyWrappers(FormProxy *fp) {
    for (int i = 0; i < LINK_DL_MAX; ++i) {
        gSPDisplayList(&fp->wrappedDisplayLists[i].displayList[WRAPPED_DL_PREDRAW], sStartDLWrapper);
        gSPDisplayList(&fp->wrappedDisplayLists[i].displayList[WRAPPED_DL_DRAW], gEmptyDL);
        gSPBranchList(&fp->wrappedDisplayLists[i].displayList[WRAPPED_DL_POSTDRAW], sEndDLWrapper);
    }

    initBowWrapper(fp);
}

static void initProxyDLs(FormProxy *fp) {
    for (int i = 0; i < LINK_DL_MAX; ++i) {
        gSPBranchList(&fp->displayLists[i], fp->wrappedDisplayLists[i].displayList);
    }
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
    initProxyShims(fp);
    initProxyDLs(fp);
}

ModelInfo *FormProxy_getCurrentModelInfo(FormProxy *fp) {
    return fp->currentModelInfo;
}

ModelInfo *FormProxy_getFallbackModelInfo(FormProxy *fp) {
    return fp->fallbackModelInfo;
}

void FormProxy_refreshSkeletons(FormProxy *fp) {
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

    FlexSkeletonHeader *skel = fp->currentModelInfo->skeleton;

    if (!skel) {
        skel = fp->fallbackModelInfo->skeleton;
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

    FlexSkeletonHeader *shieldSkel = fp->currentModelInfo->shieldingSkeleton;

    if (!shieldSkel) {
        shieldSkel = fp->fallbackModelInfo->shieldingSkeleton;
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

}

void FormProxy_refreshPlayerFaceTextures(FormProxy *fp) {
    extern TexturePtr sPlayerEyesTextures[];
    extern TexturePtr sPlayerMouthTextures[];

    ModelInfo *current = fp->currentModelInfo;
    ModelInfo *fallback = fp->fallbackModelInfo;

    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        TexturePtr eyesTex = ModelInfo_getEyesTexture(current, i);

        if (!eyesTex) {
            eyesTex = ModelInfo_getEyesTexture(fallback, i);
        }

        sPlayerEyesTextures[i] = eyesTex;
    }

    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        TexturePtr mouthTex = ModelInfo_getMouthTexture(current, i);

        if (!mouthTex) {
            mouthTex = ModelInfo_getMouthTexture(fallback, i);
        }

        sPlayerMouthTextures[i] = mouthTex;
    }
}
