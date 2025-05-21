#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_mm.h"
#include "playermodelmanager_utils.h"
#include "defines_mmo.h"

// TODO: MERGE THESE
#define SET_MODEL(dest, src) modelInfo->models[LINK_DL_##dest] = (Gfx *)&zobj[MMO_LUT_DL_##src]
#define QUICK_SET_MODEL(dlName) modelInfo->models[LINK_DL_##dlName] = (Gfx *)&zobj[MMO_LUT_DL_##dlName]

void setupZobjMmo(Link_ModelInfo *modelInfo, u8 *zobj) {
    clearLinkModelInfo(modelInfo);

    modelInfo->skeleton = (FlexSkeletonHeader *)&zobj[SEGMENT_OFFSET(zobj[MMO_SKELETON_HEADER_POINTER])];

    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_B];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_GILDED_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_HERO_BACK] = (Mtx *)&zobj[MMO_MATRIX_SHIELD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK] = (Mtx *)&zobj[MMO_MATRIX_SHIELD_B];

    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        modelInfo->eyesTextures[i] = (TexturePtr)&zobj[MMO_TEX_EYES_START + MMO_TEX_EYES_SIZE * i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        modelInfo->mouthTextures[i] = (TexturePtr)&zobj[MMO_TEX_MOUTH_START + MMO_TEX_MOUTH_SIZE * i];
    }

    QUICK_SET_MODEL(WAIST);
    QUICK_SET_MODEL(RTHIGH);
    QUICK_SET_MODEL(RSHIN);
    QUICK_SET_MODEL(RFOOT);
    QUICK_SET_MODEL(LTHIGH);
    QUICK_SET_MODEL(LSHIN);
    QUICK_SET_MODEL(LFOOT);
    QUICK_SET_MODEL(HEAD);
    QUICK_SET_MODEL(HAT);
    QUICK_SET_MODEL(COLLAR);
    QUICK_SET_MODEL(LSHOULDER);
    QUICK_SET_MODEL(LFOREARM);
    QUICK_SET_MODEL(RSHOULDER);
    QUICK_SET_MODEL(RFOREARM);
    QUICK_SET_MODEL(TORSO);
    QUICK_SET_MODEL(LHAND);
    QUICK_SET_MODEL(LFIST);
    QUICK_SET_MODEL(LHAND_BOTTLE);
    QUICK_SET_MODEL(RHAND);
    QUICK_SET_MODEL(RFIST);
    QUICK_SET_MODEL(SWORD_KOKIRI_SHEATH);
    QUICK_SET_MODEL(SWORD_RAZOR_SHEATH);
    QUICK_SET_MODEL(SWORD_GILDED_SHEATH);
    QUICK_SET_MODEL(SWORD_GILDED_HILT);
    QUICK_SET_MODEL(SWORD_GILDED_BLADE);
    QUICK_SET_MODEL(SWORD_GREATFAIRY);
    QUICK_SET_MODEL(SHIELD_HERO);
    QUICK_SET_MODEL(SHIELD_MIRROR);
    QUICK_SET_MODEL(OCARINA_TIME);
    QUICK_SET_MODEL(BOW);
    QUICK_SET_MODEL(BOW_STRING);
    QUICK_SET_MODEL(HOOKSHOT);
    QUICK_SET_MODEL(HOOKSHOT_HOOK);
    QUICK_SET_MODEL(FPS_RHAND);

}

#undef SET_MODEL
#undef QUICK_SET_MODEL
