#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager.h"
#include "playermodelmanager_utils.h"
#include "z64animation.h"
#include "model_common.h"
#include "z64object.h"
#include "model_shared.h"
#include "globalobjects_api.h"
#include "modelreplacer_compat.h"

bool gIsAgePropertyRefreshRequested = false;

Gfx gEmptyDisplayList[] = {
    gsSPEndDisplayList(),
};

Gfx gCallEmptyDisplayList[] = {
    gsSPBranchList(gEmptyDisplayList),
};

Gfx gPopModelViewMtx[] = {
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};

static Gfx sSetBilerpDL[] = {
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Mtx gZeroMtx = {0};

void initFormProxyShims(Link_FormProxy *formProxy) {

    Gfx *dls = formProxy->displayLists;
    Gfx **mtxDls = formProxy->mtxDisplayLists;
    Gfx **shims = formProxy->shimDisplayListPtrs;

    // init by pointing all to DF command
    for (u32 i = 0; i < LINK_SHIMDL_MAX; ++i) {
        shims[i] = recomp_alloc(sizeof(Gfx));
        gSPBranchList(shims[i], gEmptyDisplayList);
    }

#define SHIM_ITEM_HAND(hand, item) shims[LINK_SHIMDL_##hand##_##item] = createShimDisplayList(2, &dls[LINK_DL_##hand], &dls[LINK_DL_##item])
#define SHIM_ITEM_LFIST(item) SHIM_ITEM_HAND(LFIST, item)
#define SHIM_ITEM_RFIST(item) SHIM_ITEM_HAND(RFIST, item)
#define SHIM_ITEM_LHAND(item) SHIM_ITEM_HAND(LHAND, item)
#define SHIM_ITEM_RHAND(item) SHIM_ITEM_HAND(RHAND, item)
#define SHIM_HILT_BACK(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_HILT_BACK] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK], &dls[LINK_DL_SWORD##swordNum##_HILT], gPopModelViewMtx)
#define SHIM_SWORD_SHEATHED(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHEATHED] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SWORD##swordNum##_HILT_BACK])
#define SHIM_SHIELD_BACK(shieldNum) shims[LINK_SHIMDL_SHIELD##shieldNum##_BACK] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD##shieldNum##_BACK], &dls[LINK_DL_SHIELD##shieldNum], gPopModelViewMtx)
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
    SHIM_SWORD_SHIELD_UNSHEATHED(5, 1);
    SHIM_SWORD_SHIELD_UNSHEATHED(5, 2);
    SHIM_SWORD_SHIELD_UNSHEATHED(5, 3);

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

    shims[LINK_SHIMDL_FPS_RHAND_SLINGSHOT] = createShimDisplayList(2, &dls[LINK_DL_FPS_RHAND], &dls[LINK_DL_SLINGSHOT]);
    shims[LINK_SHIMDL_FPS_RHAND_BOW] = createShimDisplayList(2, &dls[LINK_DL_FPS_RHAND], &dls[LINK_DL_BOW]);
    shims[LINK_SHIMDL_FPS_RHAND_HOOKSHOT] = createShimDisplayList(2, &dls[LINK_DL_FPS_RHAND], &dls[LINK_DL_FPS_HOOKSHOT]);

    shims[LINK_SHIMDL_SHIELD1_ODD] = createShimDisplayList(3, mtxDls[LINK_EQUIP_MATRIX_SHIELD1_ODD], &dls[LINK_DL_SHIELD1], gPopModelViewMtx);
    
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

void setDlToShims(Link_FormProxy *formProxy) {

#define PROXY_TO_SHIM(dlName) gSPDisplayList(&formProxy->wrappedDisplayLists[LINK_DL_##dlName].displayList[WRAPPED_DL_DRAW], formProxy->shimDisplayListPtrs[LINK_SHIMDL_##dlName])

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
    PROXY_TO_SHIM(SWORD5_SHIELD1_UNSHEATHED);
    PROXY_TO_SHIM(SWORD5_SHIELD2_UNSHEATHED);
    PROXY_TO_SHIM(SWORD5_SHIELD3_UNSHEATHED);

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
    PROXY_TO_SHIM(RHAND_OCARINA_FAIRY);
    PROXY_TO_SHIM(RHAND_OCARINA_TIME);

    PROXY_TO_SHIM(FPS_RHAND_BOW);
    PROXY_TO_SHIM(FPS_RHAND_SLINGSHOT);
    PROXY_TO_SHIM(FPS_RHAND_HOOKSHOT);

    PROXY_TO_SHIM(CENTER_FLOWER_PROPELLER_CLOSED);
    PROXY_TO_SHIM(CENTER_FLOWER_PROPELLER_OPEN);

#undef PROXY_TO_SHIM
}

Gfx *getListenerDL(Link_FormProxy *formProxy, Link_DisplayList target) {
    MRC_ListenerInfo *listener = MRC_getListenerFromFormAndDL(formProxy->form, target);

    if (listener) {
        Gfx *maybeReplacedDL = NULL;

        if (listener->form == MRC_PLAYER_FORM_EVERY) {
            maybeReplacedDL = gSharedDisplayLists[target];
        } else {
            maybeReplacedDL = gLinkFormProxies[formProxy->form].vanilla.models[target];
        }

        if (maybeReplacedDL != GlobalObjects_getGlobalGfxPtr(listener->objId, listener->vanillaDL)) {
            return maybeReplacedDL;
        }
    }

    return NULL;
}

Gfx *getFormProxyDL(Link_FormProxy *formProxy, Link_DisplayList target) {
    Gfx *dl = getListenerDL(formProxy, target);

    if (!dl) {
        dl = formProxy->current.models[target];
    }

    if (!dl) {
        dl = formProxy->vanilla.models[target];
    }

    if (!dl) {
        dl = gSharedDisplayLists[target];
    }

    return dl;
}

void refreshProxyDLs(Link_FormProxy *formProxy) {
    WrappedDisplayList *wDLs = formProxy->wrappedDisplayLists;
    Link_ModelInfo *current = &formProxy->current;
    Link_ModelInfo *vanilla = &formProxy->vanilla;

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        gSPDisplayList(&wDLs[i].displayList[WRAPPED_DL_DRAW], gEmptyDL);
    }

    setDlToShims(formProxy);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        Gfx *dl = getFormProxyDL(formProxy, i);

        if (dl) {
            gSPDisplayList(&wDLs[i].displayList[WRAPPED_DL_DRAW], dl);
        }
    }

    // first person hookshot workaround
    Gfx *listenerDL = getListenerDL(formProxy, LINK_DL_HOOKSHOT);
    if (listenerDL) {
        gSPDisplayList(&wDLs[LINK_DL_FPS_HOOKSHOT].displayList[WRAPPED_DL_DRAW], listenerDL);
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

void refreshFaceTextures() {
    matchFaceTexturesToProxy(GET_PLAYER_FORM_PROXY);
}

void refreshProxyMatrixes(Link_FormProxy *formProxy) {
    Link_ModelInfo *current = &formProxy->current;
    Link_ModelInfo *vanilla = &formProxy->vanilla;

    for (u32 i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        Mtx *matrix = current->equipMtx[i];

        if (!matrix) {
            matrix = vanilla->equipMtx[i];
        }

        if (!matrix) {
            matrix = gSharedMatrixes[i];
        }

        if (matrix) {
            gSPMatrix(&formProxy->mtxDisplayLists[i][0], matrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        }
    }
}

void refreshProxySkeleton(Link_FormProxy *formProxy) {
    FlexSkeletonHeader *skel = formProxy->current.skeleton;

    if (!skel) {
        skel = formProxy->vanilla.skeleton;
    }

    if (skel) {
        for (int i = 0; i < PLAYER_LIMB_COUNT; ++i) {
            StandardLimb *limb = skel->sh.segment[i];
            formProxy->skeleton.limbs[i].child = limb->child;
            formProxy->skeleton.limbs[i].sibling = limb->sibling;
            formProxy->skeleton.limbs[i].jointPos = limb->jointPos;
        }

        formProxy->skeleton.flexSkeleton.dListCount = skel->dListCount;
    }
}

void initFormProxyMatrixes(Link_FormProxy *formProxy) {
    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        formProxy->mtxDisplayLists[i] = recomp_alloc(sizeof(Gfx) * 2);

        gSPMatrix(&formProxy->mtxDisplayLists[i][0], &gZeroMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPEndDisplayList(&formProxy->mtxDisplayLists[i][1]);
    }
}

LodLimb skeletonBase[PLAYER_LIMB_COUNT] = {
    {{0, 0, 0}, PLAYER_LIMB_WAIST - 1, LIMB_DONE, {NULL, NULL}},
    {{0, 0, 0}, PLAYER_LIMB_LOWER_ROOT - 1, PLAYER_LIMB_UPPER_ROOT - 1, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_THIGH - 1, LIMB_DONE, {NULL, NULL}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_SHIN - 1, PLAYER_LIMB_LEFT_THIGH - 1, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_FOOT - 1, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_SHIN - 1, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_FOOT - 1, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_HEAD - 1, LIMB_DONE, {NULL, NULL}},
    {{0, 0, 0}, PLAYER_LIMB_HAT - 1, PLAYER_LIMB_COLLAR - 1, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, PLAYER_LIMB_LEFT_SHOULDER - 1, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_FOREARM - 1, PLAYER_LIMB_RIGHT_SHOULDER - 1, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_LEFT_HAND - 1, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_FOREARM - 1, PLAYER_LIMB_SHEATH - 1, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, PLAYER_LIMB_RIGHT_HAND - 1, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, PLAYER_LIMB_TORSO - 1, {gEmptyDisplayList, gEmptyDisplayList}},
    {{0, 0, 0}, LIMB_DONE, LIMB_DONE, {gEmptyDisplayList, gEmptyDisplayList}},
};

void initFormProxySkeleton(Link_FormProxy *formProxy) {
#define SET_LIMB_DL(playerLimb, proxyLimbName) skel->limbs[playerLimb - 1].dLists[0] = skel->limbs[playerLimb - 1].dLists[1] = &formProxy->displayLists[proxyLimbName]

    Link_SkeletonProxy *skel = &formProxy->skeleton;
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

void initFormProxyWrappers(Link_FormProxy *formProxy) {
    for (int i = 0; i < LINK_DL_MAX; ++i) {
        gSPDisplayList(&formProxy->wrappedDisplayLists[i].displayList[WRAPPED_DL_PREDRAW], gEmptyDL);
        gSPDisplayList(&formProxy->wrappedDisplayLists[i].displayList[WRAPPED_DL_DRAW], gEmptyDL);
        gSPBranchList(&formProxy->wrappedDisplayLists[i].displayList[WRAPPED_DL_POSTDRAW], sSetBilerpDL);
    }
}

void initFormProxyDLs(Link_FormProxy *formProxy) {
    for (int i = 0; i < LINK_DL_MAX; ++i) {
        gSPBranchList(&formProxy->displayLists[i], formProxy->wrappedDisplayLists[i].displayList);
    }
}

void initFormProxy(Link_FormProxy *formProxy, PlayerTransformation form) {
    initFormProxySkeleton(formProxy);
    initFormProxyMatrixes(formProxy);
    initFormProxyShims(formProxy);
    initFormProxyWrappers(formProxy);
    initFormProxyDLs(formProxy);
    formProxy->form = form;
}

void setSkeletonDLsOnModelInfo(Link_ModelInfo *info, FlexSkeletonHeader *skel) {

#define SET_LIMB_DL(pLimb, dl) \
    if (!info->models[dl])     \
    info->models[dl] = (limbs[pLimb - 1]->dLists[0]) ? (limbs[pLimb - 1]->dLists[0]) : gCallEmptyDisplayList

    if (skel) {
        LodLimb **limbs = (LodLimb **)skel->sh.segment;
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
    }

#undef SET_LIMB_DL
}

void refreshFormProxy(Link_FormProxy *formProxy) {
    refreshProxySkeleton(formProxy);
    refreshProxyDLs(formProxy);
    refreshProxyMatrixes(formProxy);
}

static bool sIsFormProxyRefreshRequested[PLAYER_FORM_MAX];

void requestRefreshFormProxy(PlayerTransformation form) {
    sIsFormProxyRefreshRequested[form] = true;
}

RECOMP_CALLBACK("*", recomp_on_play_main)
void handleRequestedRefreshes_on_Play_Main(PlayState *play) {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        if (sIsFormProxyRefreshRequested[i]) {
            refreshFormProxy(&gLinkFormProxies[i]);
            sIsFormProxyRefreshRequested[i] = false;
        }
    }
}
