#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_mm.h"
#include "playermodelmanager_utils.h"
#include "defines_mmo.h"

// TODO: MERGE THESE
#define SET_MODEL(dest, src) modelInfo->models[dest] = (Gfx *)&zobj[src]
#define SET_Z64O_MODEL(dest, src, modName) SET_MODEL(LINK_DL_##dest, modName##_LUT_DL_##src)

#define SET_MMO_MODEL(dest, src) SET_Z64O_MODEL(dest, src, MMO)
#define QSET_MMO_MODEL(dlName) SET_MMO_MODEL(dlName, dlName)

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

    QSET_MMO_MODEL(WAIST);
    QSET_MMO_MODEL(RTHIGH);
    QSET_MMO_MODEL(RSHIN);
    QSET_MMO_MODEL(RFOOT);
    QSET_MMO_MODEL(LTHIGH);
    QSET_MMO_MODEL(LSHIN);
    QSET_MMO_MODEL(LFOOT);
    QSET_MMO_MODEL(HEAD);
    QSET_MMO_MODEL(HAT);
    QSET_MMO_MODEL(COLLAR);
    QSET_MMO_MODEL(LSHOULDER);
    QSET_MMO_MODEL(LFOREARM);
    QSET_MMO_MODEL(RSHOULDER);
    QSET_MMO_MODEL(RFOREARM);
    QSET_MMO_MODEL(TORSO);
    QSET_MMO_MODEL(LHAND);
    QSET_MMO_MODEL(LFIST);
    QSET_MMO_MODEL(LHAND_BOTTLE);
    QSET_MMO_MODEL(RHAND);
    QSET_MMO_MODEL(RFIST);
    QSET_MMO_MODEL(SWORD_KOKIRI_SHEATH);
    QSET_MMO_MODEL(SWORD_RAZOR_SHEATH);
    QSET_MMO_MODEL(SWORD_GILDED_SHEATH);
    QSET_MMO_MODEL(SWORD_GILDED_HILT);
    QSET_MMO_MODEL(SWORD_GILDED_BLADE);
    QSET_MMO_MODEL(SWORD_GREATFAIRY);
    QSET_MMO_MODEL(SHIELD_HERO);
    QSET_MMO_MODEL(SHIELD_MIRROR);
    QSET_MMO_MODEL(OCARINA_TIME);
    QSET_MMO_MODEL(BOW);
    QSET_MMO_MODEL(BOW_STRING);
    QSET_MMO_MODEL(HOOKSHOT);
    QSET_MMO_MODEL(HOOKSHOT_HOOK);
    SET_MMO_MODEL(FPS_HOOKSHOT, HOOKSHOT);
    SET_MMO_MODEL(FPS_LFOREARM, LFOREARM);
    SET_MMO_MODEL(FPS_LHAND, LFIST);
    QSET_MMO_MODEL(FPS_RHAND);
}
