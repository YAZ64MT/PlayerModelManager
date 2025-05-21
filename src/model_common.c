#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager.h"
#include "playermodelmanager_utils.h"

#define SHIM_ITEM_HAND(hand, item) shims[LINK_SHIMDL_##hand##_##item] = createShimDisplayList(2, &dls[LINK_DL_##hand], &dls[LINK_DL_##item])
#define SHIM_ITEM_LFIST(item) SHIM_ITEM_HAND(LFIST, item)
#define SHIM_ITEM_RFIST(item) SHIM_ITEM_HAND(RFIST, item)
#define SHIM_ITEM_LHAND(item) SHIM_ITEM_HAND(LHAND, item)
#define SHIM_ITEM_RHAND(item) SHIM_ITEM_HAND(RHAND, item)
#define SHIM_HILT_BACK(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_HILT_BACK] = createShimWithMatrix(&mtx[LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK], 1, &dls[LINK_DL_SWORD##swordNum##_HILT])
#define SHIM_SWORD_SHEATHED(swordNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHEATHED] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SWORD##swordNum##_SHEATHED])
#define SHIM_SHIELD_BACK(shieldNum) shims[LINK_SHIMDL_SHIELD##shieldNum##_BACK] = createShimWithMatrix(&mtx[LINK_EQUIP_MATRIX_SHIELD##shieldNum##_BACK], 1, &dls[LINK_DL_SHIELD##shieldNum])
#define SHIM_SWORD_SHIELD_UNSHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_UNSHEATHED] = createShimDisplayList(1, &dls[LINK_DL_SWORD##swordNum##_SHEATH])
#define SHIM_SWORD_SHIELD_SHEATH(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATH] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_SHEATH], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD_SHIELD_SHEATHED(swordNum, shieldNum) shims[LINK_SHIMDL_SWORD##swordNum##_SHIELD##shieldNum##_SHEATHED] = createShimDisplayList(3, &dls[LINK_DL_SWORD##swordNum##_SHEATHED], &dls[LINK_DL_SHIELD##shieldNum##_BACK])
#define SHIM_SWORD(swordNum) shims[LINK_SHIMDL_SWORD##swordNum] = createShimDisplayList(2, &dls[LINK_DL_SWORD##swordNum##_HILT], &dls[LINK_DL_SWORD##swordNum##_BLADE])
#define SHIM_SWORD_LFIST(swordNum) SHIM_ITEM_LFIST(SWORD##swordNum)
#define SHIM_SHIELD_RFIST(shieldNum) SHIM_ITEM_RFIST(SHIELD##shieldNum)

void initFormProxyShims(Link_FormProxy *formProxy) {
    Gfx *dls = formProxy->displayLists;
    Mtx *mtx = formProxy->equipMtx;
    Gfx **shims = formProxy->shimDisplayListPtrs;

    // init by pointing all to DF command
    for (u32 i = 0; i < LINK_SHIMDL_MAX; ++i) {
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

    shims[LINK_SHIMDL_SHIELD1_ODD] = createShimWithMatrix(&mtx[LINK_EQUIP_MATRIX_SHIELD1_ODD], 1, &dls[LINK_DL_SHIELD1]);
}

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

#define PROXY_TO_SHIM(dlName) gSPBranchList(&formProxy->displayLists[LINK_DL_##dlName], formProxy->shimDisplayListPtrs[LINK_SHIMDL_##dlName])

void setProxyToShims(Link_FormProxy *formProxy) {
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

#undef PROXY_TO_SHIM

void refreshProxyDls(Link_FormProxy *formProxy) {
    setProxyToShims(formProxy);

    Gfx *dls = formProxy->displayLists;
    Link_ModelInfo *current = &formProxy->current;
    Link_ModelInfo *vanilla = &formProxy->vanilla;

    for (u32 i = 0; i < LINK_DL_MAX; ++i) {
        Gfx* dl = current->models[i];

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

void refreshProxyFaceTextures(Link_FormProxy *formProxy) {
    Link_ModelInfo *current = &formProxy->current;
    Link_ModelInfo *vanilla = &formProxy->vanilla;

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        sPlayerEyesTextures[i] = current->eyesTextures[i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        sPlayerMouthTextures[i] = current->mouthTextures[i];
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

        if (!matrix) {
            matrix = &formProxy->equipMtx[i];
        }

        formProxy->equipMtx[i] = *matrix;
    }
}
