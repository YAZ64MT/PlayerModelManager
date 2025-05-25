#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_mm.h"
#include "playermodelmanager_utils.h"
#include "defines_mmo.h"
#include "defines_ooto.h"
#include "zobjutils.h"
#include "model_common.h"

void setupFaceTextures(Link_ModelInfo *modelInfo, u8 *zobj) {
    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        modelInfo->eyesTextures[i] = (TexturePtr)&zobj[Z64O_TEX_EYES_START + Z64O_TEX_EYES_SIZE * i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        modelInfo->mouthTextures[i] = (TexturePtr)&zobj[Z64O_TEX_MOUTH_START + Z64O_TEX_MOUTH_SIZE * i];
    }
}

void repointZobjDls(u8 *zobj, u32 start, u32 end) {
    u32 current = start;

    while (current < end) {
        ZobjUtils_repointDisplayList(zobj, current, 0x06, zobj);
        current += 8;
    }
}

#define SET_LIMB_DL(playerLimb, modelName) modelInfo->models[modelName] = (Gfx *)limbs[playerLimb - 1]->dLists[0] == NULL ? callDfCommand : (Gfx *)limbs[playerLimb - 1]->dLists[0]

void handleZobjSkeleton(Link_ModelInfo *modelInfo, u8 *zobj) {
    u32 skelHeader = SEGMENT_OFFSET(readU32(zobj, Z64O_SKELETON_HEADER_POINTER));

    ZobjUtils_repointFlexSkeleton(zobj, skelHeader, 0x06, zobj);

    FlexSkeletonHeader *flexHeader = (FlexSkeletonHeader *)&zobj[skelHeader];
    LodLimb **limbs = (LodLimb **)flexHeader->sh.segment;
    for (u32 i = 0; i < PLAYER_LIMB_COUNT; ++i) {
        modelInfo->limbTranslations[i] = limbs[i]->jointPos;
    }
}

void clearSkeletonDls(Link_ModelInfo *modelInfo, u8 *zobj) {
    u32 skelHeader = SEGMENT_OFFSET(readU32(zobj, Z64O_SKELETON_HEADER_POINTER));

    FlexSkeletonHeader *flexHeader = (FlexSkeletonHeader *)&zobj[skelHeader];
    LodLimb **limbs = (LodLimb **)flexHeader->sh.segment;

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

void setSkeletonDls(Link_ModelInfo *modelInfo, u8 *zobj) {
    u32 skelHeader = SEGMENT_OFFSET(readU32(zobj, Z64O_SKELETON_HEADER_POINTER));

    FlexSkeletonHeader *flexHeader = (FlexSkeletonHeader *)&zobj[skelHeader];
    LodLimb **limbs = (LodLimb **)flexHeader->sh.segment;

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

#define SET_MODEL(dest, src) modelInfo->models[dest] = (Gfx *)&zobj[src]
#define SET_Z64O_MODEL(dest, src, modName) SET_MODEL(LINK_DL_##dest, modName##_LUT_DL_##src)

#define SET_MMO_MODEL(dest, src) SET_Z64O_MODEL(dest, src, MMO)
#define QSET_MMO_MODEL(dlName) SET_MMO_MODEL(dlName, dlName)

void setupZobjMmo(Link_ModelInfo *modelInfo, u8 *zobj) {

    clearLinkModelInfo(modelInfo);

    repointZobjDls(zobj, MMO_LUT_DL_WAIST, MMO_LUT_DL_DF_COMMAND);

    handleZobjSkeleton(modelInfo, zobj);

    setupFaceTextures(modelInfo, zobj);

    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_B];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_GILDED_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_HERO_BACK] = (Mtx *)&zobj[MMO_MATRIX_SHIELD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK] = (Mtx *)&zobj[MMO_MATRIX_SHIELD_B];

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
    SET_MMO_MODEL(FPS_RFOREARM, DF_COMMAND);
    QSET_MMO_MODEL(FPS_RHAND);
}

void fixTunicColor(PlayState *play) {
    OPEN_DISPS(play->state.gfxCtx);

    gDPSetEnvColor(POLY_OPA_DISP++, 30, 105, 27, 0);

    CLOSE_DISPS(play->state.gfxCtx);
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayDefault")
void Recolor_OverrideLimbDrawDefault(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    fixTunicColor(play);
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson")
void Recolor_OverrideLimbDrawFirstPerson(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    fixTunicColor(play);
}

#define SET_OOTO_CHILD_MODEL(dest, src) SET_Z64O_MODEL(dest, src, OOTO_CHILD)
#define QSET_OOTO_CHILD_MODEL(dlName) SET_OOTO_CHILD_MODEL(dlName, dlName)

// TODO: FIGURE OUT WHY LEFT SHOULDER ISN'T IMPORTED CORRECTLY
void setupZobjOotoChild(Link_ModelInfo *modelInfo, u8 *zobj) {

    clearLinkModelInfo(modelInfo);

    repointZobjDls(zobj, OOTO_CHILD_LUT_DL_WAIST, OOTO_CHILD_LUT_DL_FPS_RARM_SLINGSHOT);

    handleZobjSkeleton(modelInfo, zobj);

    setupFaceTextures(modelInfo, zobj);

    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK] = (Mtx *)&zobj[OOTO_CHILD_MATRIX_SWORD_BACK];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_HERO_BACK] = (Mtx *)&zobj[OOTO_CHILD_MATRIX_SHIELD_BACK];

    QSET_OOTO_CHILD_MODEL(WAIST);
    QSET_OOTO_CHILD_MODEL(RTHIGH);
    QSET_OOTO_CHILD_MODEL(RSHIN);
    QSET_OOTO_CHILD_MODEL(RFOOT);
    QSET_OOTO_CHILD_MODEL(LTHIGH);
    QSET_OOTO_CHILD_MODEL(LSHIN);
    QSET_OOTO_CHILD_MODEL(LFOOT);
    QSET_OOTO_CHILD_MODEL(HEAD);
    QSET_OOTO_CHILD_MODEL(HAT);
    QSET_OOTO_CHILD_MODEL(COLLAR);
    QSET_OOTO_CHILD_MODEL(LSHOULDER);
    QSET_OOTO_CHILD_MODEL(LFOREARM);
    QSET_OOTO_CHILD_MODEL(RSHOULDER);
    QSET_OOTO_CHILD_MODEL(RFOREARM);
    QSET_OOTO_CHILD_MODEL(TORSO);
    QSET_OOTO_CHILD_MODEL(LHAND);
    QSET_OOTO_CHILD_MODEL(LFIST);
    QSET_OOTO_CHILD_MODEL(LHAND_BOTTLE);
    QSET_OOTO_CHILD_MODEL(RHAND);
    QSET_OOTO_CHILD_MODEL(RFIST);
    QSET_OOTO_CHILD_MODEL(SWORD_KOKIRI_SHEATH);
    QSET_OOTO_CHILD_MODEL(SWORD_KOKIRI_HILT);
    QSET_OOTO_CHILD_MODEL(SWORD_KOKIRI_BLADE);
    SET_OOTO_CHILD_MODEL(SHIELD_HERO, SHIELD_DEKU);
    QSET_OOTO_CHILD_MODEL(OCARINA_TIME);
    SET_OOTO_CHILD_MODEL(FPS_LFOREARM, LFOREARM);
    SET_OOTO_CHILD_MODEL(FPS_LHAND, LFIST);
    modelInfo->models[LINK_DL_FPS_RFOREARM] = dfCommand;
    QSET_OOTO_CHILD_MODEL(FPS_RHAND);
}

void setupZobjZ64o(Link_ModelInfo *modelInfo, u8 *zobj) {
    switch (zobj[Z64O_FORM_BYTE]) {
    case MMO_FORM_BYTE_CHILD:
        setupZobjMmo(modelInfo, zobj);
        break;

    case OOTO_FORM_BYTE_CHILD:
        setupZobjOotoChild(modelInfo, zobj);
        break;

    default:
        break;
    }
}
