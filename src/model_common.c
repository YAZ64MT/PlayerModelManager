#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager.h"
#include "playermodelmanager_utils.h"
#include "z64animation.h"
#include "model_common.h"
#include "z64object.h"

Gfx dfCommand[] = {
    gsSPEndDisplayList(),
};

Gfx callDfCommand[] = {
    gsSPBranchList(dfCommand),
};

Gfx popModelViewMtx[] = {
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};

Mtx zeroMtx = {0};

#define SHIM_ITEM_HAND(hand, item) shims[LINK_SHIMDL_##hand##_##item] = createShimDisplayList(2, &dls[LINK_DL_##hand], &dls[LINK_DL_##item])
#define SHIM_ITEM_LFIST(item) SHIM_ITEM_HAND(LFIST, item)
#define SHIM_ITEM_RFIST(item) SHIM_ITEM_HAND(RFIST, item)
#define SHIM_ITEM_LHAND(item) SHIM_ITEM_HAND(LHAND, item)
#define SHIM_ITEM_RHAND(item) SHIM_ITEM_HAND(RHAND, item)
#define SHIM_HILT_BACK(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_HILT_BACK] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK], &dls[LINK_DL_SWORD##swordNum##_HILT], popModelViewMtx)
#define SHIM_SWORD_SHEATHED(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHEATHED] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SWORD##swordNum##_HILT_BACK])
#define SHIM_SHIELD_BACK(shieldNum) shims[LINK_SHIMDL_SHIELD##shieldNum##_BACK] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD##shieldNum##_BACK], &dls[LINK_DL_SHIELD##shieldNum], popModelViewMtx)
#define SHIM_SWORD_SHIELD_UNSHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_UNSHEATHED] = createShimDisplayList(1, &dls[LINK_DL_SWORD##swordNum##_SHEATH])
#define SHIM_SWORD_SHIELD_SHEATH(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATH] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD_SHIELD_SHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATHED] = createShimDisplayList(3, &dls[LINK_DL_SWORD##swordNum##_SHEATHED], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD(swordNum) shims[LINK_SHIMDL_SWORD##swordNum] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_HILT], &dls[LINK_DL_SWORD##swordNum##_BLADE])
#define SHIM_SWORD_LFIST(swordNum) SHIM_ITEM_LFIST(SWORD##swordNum)
#define SHIM_SHIELD_RFIST(shieldNum) SHIM_ITEM_RFIST(SHIELD##shieldNum)

void initFormProxyShims(Link_FormProxy *formProxy) {

    Gfx *dls = formProxy->displayLists;
    Gfx **mtxDls = formProxy->mtxDisplayLists;
    Gfx **shims = formProxy->shimDisplayListPtrs;

    // init by pointing all to DF command
    for (u32 i = 0; i < LINK_SHIMDL_MAX; ++i) {
        shims[i] = recomp_alloc(sizeof(Gfx));
        gSPBranchList(shims[i], &dls[LINK_DL_DF_COMMAND]);
    }

    SHIM_SWORD(1);
    SHIM_SWORD(2);
    SHIM_SWORD(3);
    SHIM_SWORD(4);
    shims[LINK_SHIMDL_SWORD4_BROKEN] = createShimDisplayList(2, &dls[LINK_DL_SWORD4_HILT], &dls[LINK_DL_SWORD4_BLADE_BROKEN]);

    SHIM_HILT_BACK(1);
    SHIM_HILT_BACK(2);
    SHIM_HILT_BACK(3);
    SHIM_HILT_BACK(4);

    SHIM_SWORD_SHEATHED(1);
    SHIM_SWORD_SHEATHED(2);
    SHIM_SWORD_SHEATHED(3);
    SHIM_SWORD_SHEATHED(4);

    SHIM_SHIELD_BACK(1);
    SHIM_SHIELD_BACK(2);
    SHIM_SHIELD_BACK(3);

    SHIM_SWORD_SHIELD_UNSHEATHED(1, 1);
    SHIM_SWORD_SHIELD_UNSHEATHED(1, 2);
    SHIM_SWORD_SHIELD_UNSHEATHED(1, 3);
    SHIM_SWORD_SHIELD_UNSHEATHED(2, 1);
    SHIM_SWORD_SHIELD_UNSHEATHED(2, 2);
    SHIM_SWORD_SHIELD_UNSHEATHED(2, 3);
    SHIM_SWORD_SHIELD_UNSHEATHED(3, 1);
    SHIM_SWORD_SHIELD_UNSHEATHED(3, 2);
    SHIM_SWORD_SHIELD_UNSHEATHED(3, 3);
    SHIM_SWORD_SHIELD_UNSHEATHED(4, 1);
    SHIM_SWORD_SHIELD_UNSHEATHED(4, 2);
    SHIM_SWORD_SHIELD_UNSHEATHED(4, 3);

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

    SHIM_SWORD_LFIST(1);
    SHIM_SWORD_LFIST(2);
    SHIM_SWORD_LFIST(3);
    SHIM_SWORD_LFIST(4);
    SHIM_SWORD_LFIST(4_BROKEN);

    SHIM_SHIELD_RFIST(1);
    SHIM_SHIELD_RFIST(2);
    SHIM_SHIELD_RFIST(3);

    SHIM_ITEM_LFIST(HAMMER);
    SHIM_ITEM_RFIST(BOW);
    SHIM_ITEM_RFIST(HOOKSHOT);
    SHIM_ITEM_RHAND(OCARINA_FAIRY);
    SHIM_ITEM_RHAND(OCARINA_TIME);
    SHIM_ITEM_LHAND(GUITAR);
    SHIM_ITEM_LFIST(BOOMERANG);

    shims[LINK_SHIMDL_FPS_RHAND_SLINGSHOT] = createShimDisplayList(2, &dls[LINK_DL_FPS_RHAND], &dls[LINK_DL_SLINGSHOT]);
    shims[LINK_SHIMDL_FPS_RHAND_BOW] = createShimDisplayList(2, &dls[LINK_DL_FPS_RHAND], &dls[LINK_DL_BOW]);
    shims[LINK_SHIMDL_FPS_RHAND_HOOKSHOT] = createShimDisplayList(2, &dls[LINK_DL_FPS_RHAND], &dls[LINK_DL_FPS_HOOKSHOT]);

    shims[LINK_SHIMDL_SHIELD1_ODD] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD1_ODD], &dls[LINK_DL_SHIELD1], popModelViewMtx);
}

#define PROXY_TO_SHIM(dlName) gSPBranchList(&formProxy->displayLists[LINK_DL_##dlName], formProxy->shimDisplayListPtrs[LINK_SHIMDL_##dlName])

void setDlToShims(Link_FormProxy *formProxy) {
    PROXY_TO_SHIM(SWORD1);
    PROXY_TO_SHIM(SWORD2);
    PROXY_TO_SHIM(SWORD3);
    PROXY_TO_SHIM(SWORD4);
    PROXY_TO_SHIM(SWORD4_BROKEN);

    PROXY_TO_SHIM(SWORD1_HILT_BACK);
    PROXY_TO_SHIM(SWORD2_HILT_BACK);
    PROXY_TO_SHIM(SWORD3_HILT_BACK);
    PROXY_TO_SHIM(SWORD4_HILT_BACK);

    PROXY_TO_SHIM(SHIELD1_BACK);
    PROXY_TO_SHIM(SHIELD2_BACK);
    PROXY_TO_SHIM(SHIELD3_BACK);

    PROXY_TO_SHIM(SWORD1_SHIELD1_UNSHEATHED);
    PROXY_TO_SHIM(SWORD1_SHIELD2_UNSHEATHED);
    PROXY_TO_SHIM(SWORD1_SHIELD3_UNSHEATHED);
    PROXY_TO_SHIM(SWORD2_SHIELD1_UNSHEATHED);
    PROXY_TO_SHIM(SWORD2_SHIELD2_UNSHEATHED);
    PROXY_TO_SHIM(SWORD2_SHIELD3_UNSHEATHED);
    PROXY_TO_SHIM(SWORD3_SHIELD1_UNSHEATHED);
    PROXY_TO_SHIM(SWORD3_SHIELD2_UNSHEATHED);
    PROXY_TO_SHIM(SWORD3_SHIELD3_UNSHEATHED);
    PROXY_TO_SHIM(SWORD4_SHIELD1_UNSHEATHED);
    PROXY_TO_SHIM(SWORD4_SHIELD2_UNSHEATHED);
    PROXY_TO_SHIM(SWORD4_SHIELD3_UNSHEATHED);

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

    PROXY_TO_SHIM(LFIST_SWORD1);
    PROXY_TO_SHIM(LFIST_SWORD2);
    PROXY_TO_SHIM(LFIST_SWORD3);
    PROXY_TO_SHIM(LFIST_SWORD4);
    PROXY_TO_SHIM(LFIST_SWORD4_BROKEN);

    PROXY_TO_SHIM(LFIST_HAMMER);
    PROXY_TO_SHIM(LFIST_BOOMERANG);
    PROXY_TO_SHIM(RFIST_BOW);
    PROXY_TO_SHIM(RFIST_SLINGSHOT);
    PROXY_TO_SHIM(RFIST_HOOKSHOT);
    PROXY_TO_SHIM(RHAND_OCARINA_FAIRY);
    PROXY_TO_SHIM(RHAND_OCARINA_TIME);
    PROXY_TO_SHIM(LHAND_GUITAR);

    PROXY_TO_SHIM(FPS_RHAND_BOW);
    PROXY_TO_SHIM(FPS_RHAND_SLINGSHOT);
    PROXY_TO_SHIM(FPS_RHAND_HOOKSHOT);
}

void refreshProxyDls(Link_FormProxy *formProxy) {
    setDlToShims(formProxy);

    Gfx *dls = formProxy->displayLists;
    Link_ModelInfo *current = &formProxy->current;
    Link_ModelInfo *vanilla = &formProxy->vanilla;

    for (u32 i = 0; i < LINK_DL_MAX; ++i) {
        Gfx *dl = current->models[i];

        if (!dl) {
            dl = vanilla->models[i];
        }

        if (dl) {
            gSPBranchList(&dls[i], dl);
        }
    }
}

extern TexturePtr sPlayerEyesTextures[];
extern TexturePtr sPlayerMouthTextures[];

void matchFaceTexturesToProxy(Link_FormProxy *formProxy) {
    Link_ModelInfo *current = &formProxy->current;
    Link_ModelInfo *vanilla = &formProxy->vanilla;

    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        TexturePtr eyesTex = current->eyesTextures[i];

        if (!eyesTex) {
            eyesTex = vanilla->eyesTextures[i];
        }

        sPlayerEyesTextures[i] = eyesTex;
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        TexturePtr mouthTex = current->mouthTextures[i];

        if (!mouthTex) {
            mouthTex = vanilla->mouthTextures[i];
        }

        sPlayerMouthTextures[i] = mouthTex;
    }
}

void refreshProxyMatrixes(Link_FormProxy *formProxy) {
    Link_ModelInfo *current = &formProxy->current;
    Link_ModelInfo *vanilla = &formProxy->vanilla;

    for (u32 i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        Mtx *matrix = current->equipMtx[i];

        if (!matrix) {
            matrix = vanilla->equipMtx[i];
        }

        if (matrix) {
            //recomp_printf("Old DL:%X%X %X%X\n", formProxy->mtxDisplayLists[i][0].words.w0, formProxy->mtxDisplayLists[i][0].words.w1, formProxy->mtxDisplayLists[i][1].words.w0, formProxy->mtxDisplayLists[i][1].words.w1);
            //recomp_printf("Repointing matrix 0x%x -> 0x%x\n", formProxy->mtxDisplayLists[i][0].words.w1, matrix);
            gSPMatrix(&formProxy->mtxDisplayLists[i][0], matrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            //recomp_printf("New DL:%X%X %X%X\n", formProxy->mtxDisplayLists[i][0].words.w0, formProxy->mtxDisplayLists[i][0].words.w1, formProxy->mtxDisplayLists[i][1].words.w0, formProxy->mtxDisplayLists[i][1].words.w1);
        }
    }
}

void refreshProxySkeleton(Link_FormProxy *formProxy) {

    Vec3s *limbTrans = formProxy->current.limbTranslations;

    // while, technically, there *could* be a custom model where all bones are at (0,0,0)
    // but in practice this doesn't happen, so all transforms being at (0,0,0) will be 
    // treated as no custom skeleton
    bool hasCustomSkeleton = false;
    for (u32 i = 0; !hasCustomSkeleton && i < PLAYER_LIMB_COUNT; ++i) {
        hasCustomSkeleton = limbTrans->x != 0 || limbTrans->y != 0 || limbTrans->z != 0;
    }

    if (!hasCustomSkeleton) {
        limbTrans = formProxy->vanilla.limbTranslations;
    }

    if (limbTrans) {
        for (u32 i = 0; i < PLAYER_LIMB_COUNT; ++i) {
            formProxy->skeleton.limbs[i].jointPos = limbTrans[i];
        }
    }
}

void initFormProxyMatrixes(Link_FormProxy *formProxy) {
    for (u32 i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        formProxy->mtxDisplayLists[i] = recomp_alloc(sizeof(Gfx) * 2);

        gSPMatrix(&formProxy->mtxDisplayLists[i][0], &zeroMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPEndDisplayList(&formProxy->mtxDisplayLists[i][1]);
    }
}

void initFormProxyDisplayLists(Link_FormProxy *formProxy) {
    for (u32 i = 0; i < LINK_DL_MAX; ++i) {
        gSPBranchList(&formProxy->displayLists[i], &formProxy->displayLists[LINK_DL_DF_COMMAND]);
    }
    gSPBranchList(&formProxy->displayLists[LINK_DL_DF_COMMAND], dfCommand);

    for (u32 i = 0; i < LINK_DL_MAX; ++i) {
        gSPBranchList(&formProxy->proxyDisplayLists[i], &formProxy->displayLists[i]);
    }
}

LodLimb skeletonBase[PLAYER_LIMB_COUNT] = {
    {{0, 0, 0}, PLAYER_LIMB_WAIST - 1, LIMB_DONE, {NULL, NULL}},
    {{0, 0, 0}, PLAYER_LIMB_LOWER_ROOT - 1, PLAYER_LIMB_UPPER_ROOT - 1, {(Gfx *)LINK_DL_WAIST, (Gfx *)LINK_DL_WAIST}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_THIGH - 1, LIMB_DONE, {NULL, NULL}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_SHIN - 1, PLAYER_LIMB_LEFT_THIGH - 1, {(Gfx *)LINK_DL_RTHIGH, (Gfx *)LINK_DL_RTHIGH}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_FOOT - 1, LIMB_DONE, {(Gfx *)LINK_DL_RSHIN, (Gfx *)LINK_DL_RSHIN}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {(Gfx *)LINK_DL_RFOOT, (Gfx *)LINK_DL_RFOOT}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_SHIN - 1, LIMB_DONE, {(Gfx *)LINK_DL_LTHIGH, (Gfx *)LINK_DL_LTHIGH}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_FOOT - 1, LIMB_DONE, {(Gfx *)LINK_DL_LSHIN, (Gfx *)LINK_DL_LSHIN}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {(Gfx *)LINK_DL_LFOOT, (Gfx *)LINK_DL_LFOOT}},
    {{0, 0, 0}, PLAYER_LIMB_HEAD - 1, LIMB_DONE, {NULL, NULL}},
    {{0, 0, 0}, PLAYER_LIMB_HAT - 1, PLAYER_LIMB_COLLAR - 1, {(Gfx *)LINK_DL_HEAD, (Gfx *)LINK_DL_HEAD}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {(Gfx *)LINK_DL_HAT, (Gfx *)LINK_DL_HAT}},
    {{0, 0, 0}, LIMB_DONE, PLAYER_LIMB_LEFT_SHOULDER - 1, {(Gfx *)LINK_DL_COLLAR, (Gfx *)LINK_DL_COLLAR}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_FOREARM - 1, PLAYER_LIMB_RIGHT_SHOULDER - 1, {(Gfx *)LINK_DL_LSHOULDER, (Gfx *)LINK_DL_LSHOULDER}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_HAND - 1, LIMB_DONE, {(Gfx *)LINK_DL_LFOREARM, (Gfx *)LINK_DL_LFOREARM}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {(Gfx *)LINK_DL_LHAND, (Gfx *)LINK_DL_LHAND}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_FOREARM - 1, PLAYER_LIMB_SHEATH - 1, {(Gfx *)LINK_DL_RSHOULDER, (Gfx *)LINK_DL_RSHOULDER}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_HAND - 1, LIMB_DONE, {(Gfx *)LINK_DL_RFOREARM, (Gfx *)LINK_DL_RFOREARM}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {(Gfx *)LINK_DL_RHAND, (Gfx *)LINK_DL_RHAND}},
    {{0, 0, 0}, LIMB_DONE, PLAYER_LIMB_TORSO - 1, {(Gfx *)LINK_DL_SWORD1_SHEATH, (Gfx *)LINK_DL_SWORD1_SHEATH}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {(Gfx *)LINK_DL_TORSO, (Gfx *)LINK_DL_TORSO}},
};

void initFormProxySkeleton(Link_FormProxy *formProxy) {

    Link_SkeletonProxy *skel = &formProxy->skeleton;
    FlexSkeletonHeader *flex = &skel->flexSkeleton;
    flex->dListCount = 18;
    flex->sh.limbCount = PLAYER_LIMB_COUNT;

    for (u32 i = 0; i < PLAYER_LIMB_COUNT; i++) {
        skel->limbPtrs[i] = &skel->limbs[i];

        skel->limbs[i] = skeletonBase[i];

        Gfx** dLists = skel->limbs[i].dLists;

        if (dLists[0]) { // limbs with a NULL display list pointer should be ignored
            dLists[0] = &formProxy->proxyDisplayLists[(u32)dLists[0]];
            dLists[1] = &formProxy->proxyDisplayLists[(u32)dLists[1]];
        }
    }

    flex->sh.segment = (void **)skel->limbPtrs;
}

void loadVanillaSkeletonTransforms(Link_ModelInfo *modelInfo, ObjectId objectId, void **limbs) {
    size_t size = gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart;

    void *vanillaObj = recomp_alloc(size);

    DmaMgr_RequestSync(vanillaObj, gObjectTable[objectId].vromStart, size);

    recomp_printf("Limb ptr: 0x%x\nSegment Offset: 0x%x\n", limbs, SEGMENT_OFFSET(limbs));

    LodLimb **realLimbsPtr = (LodLimb **)(SEGMENT_OFFSET(limbs) + (u32)vanillaObj);

    for (u32 i = 0; i < PLAYER_LIMB_COUNT; ++i) {
        LodLimb *realLimb = (LodLimb *)(SEGMENT_OFFSET(realLimbsPtr[i]) + (u32)vanillaObj);
        modelInfo->limbTranslations[i] = realLimb->jointPos;
    }

    recomp_free(vanillaObj);
}

void initFormProxy(Link_FormProxy *formProxy) {
    initFormProxySkeleton(formProxy);
    initFormProxyDisplayLists(formProxy);
    initFormProxyMatrixes(formProxy);
    initFormProxyShims(formProxy);
}

void refreshFormProxy(Link_FormProxy *formProxy) {
    refreshProxySkeleton(formProxy);
    refreshProxyDls(formProxy);
    refreshProxyMatrixes(formProxy);
}
