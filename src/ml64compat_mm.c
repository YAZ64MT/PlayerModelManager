#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_mm.h"
#include "playermodelmanager_utils.h"
#include "defines_mmo.h"
#include "defines_ooto.h"
#include "globalobjects_api.h"
#include "model_common.h"
#include "ml64compat.h"
#include "defines_modelinfo.h"

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
        ZGlobalObj_globalizeSegmentedDL(zobj, (Gfx *)SEGMENT_ADDR(0x06, current));
        current += 8;
    }
}

void handleZobjSkeleton(Link_ModelInfo *modelInfo, u8 *zobj) {
    u32 skelHeader = readU32(zobj, Z64O_SKELETON_HEADER_POINTER);

    ZGlobalObj_globalizeLodLimbSkeleton(zobj, (void *)skelHeader);

    FlexSkeletonHeader *flexHeader = SEGMENTED_TO_GLOBAL_PTR(zobj, skelHeader);
    LodLimb **limbs = (LodLimb **)flexHeader->sh.segment;
    for (int i = 0; i < PLAYER_LIMB_COUNT; ++i) {
        modelInfo->limbTranslations[i] = limbs[i]->jointPos;
    }
}

#define SET_MODEL(dest, src) modelInfo->models[dest] = (Gfx *)&zobj[src]
#define SET_Z64O_MODEL(dest, src, modName) SET_MODEL(LINK_DL_##dest, modName##_LUT_DL_##src)

#define SET_MMO_MODEL(dest, src) SET_Z64O_MODEL(dest, src, MMO)
#define QSET_MMO_MODEL(dlName) SET_MMO_MODEL(dlName, dlName)

void setupZobjMmoHuman(Link_ModelInfo *modelInfo, u8 *zobj) {

    clearLinkModelInfo(modelInfo);

    if (zobj[Z64O_FORM_BYTE] == MMO_FORM_BYTE_ADULT) {
        modelInfo->flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;
    }

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

void setupZobjOotoChild(Link_ModelInfo *modelInfo, u8 *zobj) {

    clearLinkModelInfo(modelInfo);

    // OotoFixHeaderSkelPtr MUST run before OotoFixChildLeftShoulder to ensure the latter reads the right offset for the skeleton in old zobjs
    // OotoFixChildLeftShoulder MUST run before repointZobjDls to ensure the left shoulder is repointed in old zobjs

    // old versions of manifest did not write header ptr
    OotoFixHeaderSkelPtr(zobj);
    // old versions of manifests had a typo that pointed the left shoulder entry to the left forearm
    OotoFixChildLeftShoulder(zobj);

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
    QSET_OOTO_CHILD_MODEL(DEKU_STICK);
    QSET_OOTO_CHILD_MODEL(BOTTLE_GLASS);
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

#define SET_OOTO_ADULT_MODEL(dest, src) SET_Z64O_MODEL(dest, src, OOTO_ADULT)
#define QSET_OOTO_ADULT_MODEL(dlName) SET_OOTO_ADULT_MODEL(dlName, dlName)

void setupZobjOotoAdult(Link_ModelInfo *modelInfo, u8 *zobj) {

    clearLinkModelInfo(modelInfo);

    modelInfo->flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;

    // OotoFixHeaderSkelPtr MUST run before repointZobjDls to ensure the latter reads the right offset for the skeleton in old zobjs

    // old versions of manifest did not write header ptr
    OotoFixHeaderSkelPtr(zobj);

    repointZobjDls(zobj, OOTO_ADULT_LUT_DL_WAIST, OOTO_ADULT_LUT_DL_FPS_LHAND_HOOKSHOT);

    handleZobjSkeleton(modelInfo, zobj);

    setupFaceTextures(modelInfo, zobj);

    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_GILDED_BACK] = (Mtx *)&zobj[OOTO_ADULT_MATRIX_SWORD_BACK];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_HERO_BACK] = (Mtx *)&zobj[OOTO_ADULT_MATRIX_SHIELD_BACK];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK] = (Mtx *)&zobj[OOTO_ADULT_MATRIX_SHIELD_BACK];

    QSET_OOTO_ADULT_MODEL(WAIST);
    QSET_OOTO_ADULT_MODEL(RTHIGH);
    QSET_OOTO_ADULT_MODEL(RSHIN);
    QSET_OOTO_ADULT_MODEL(RFOOT);
    QSET_OOTO_ADULT_MODEL(LTHIGH);
    QSET_OOTO_ADULT_MODEL(LSHIN);
    QSET_OOTO_ADULT_MODEL(LFOOT);
    QSET_OOTO_ADULT_MODEL(HEAD);
    QSET_OOTO_ADULT_MODEL(HAT);
    QSET_OOTO_ADULT_MODEL(COLLAR);
    QSET_OOTO_ADULT_MODEL(LSHOULDER);
    QSET_OOTO_ADULT_MODEL(LFOREARM);
    QSET_OOTO_ADULT_MODEL(RSHOULDER);
    QSET_OOTO_ADULT_MODEL(RFOREARM);
    QSET_OOTO_ADULT_MODEL(TORSO);
    QSET_OOTO_ADULT_MODEL(LHAND);
    QSET_OOTO_ADULT_MODEL(LFIST);
    QSET_OOTO_ADULT_MODEL(LHAND_BOTTLE);
    QSET_OOTO_ADULT_MODEL(RHAND);
    QSET_OOTO_ADULT_MODEL(RFIST);
    SET_OOTO_ADULT_MODEL(SWORD_GILDED_SHEATH, SWORD_MASTER_SHEATH);
    SET_OOTO_ADULT_MODEL(SWORD_GILDED_HILT, SWORD_MASTER_HILT);
    SET_OOTO_ADULT_MODEL(SWORD_GILDED_BLADE, SWORD_MASTER_BLADE);
    SET_OOTO_ADULT_MODEL(SWORD4_HILT, BIGSWORD_HILT);
    SET_OOTO_ADULT_MODEL(SWORD4_BLADE, BIGSWORD_BLADE);
    SET_OOTO_ADULT_MODEL(SHIELD_HERO, SHIELD_HYLIAN);
    QSET_OOTO_ADULT_MODEL(SHIELD_MIRROR);
    QSET_OOTO_ADULT_MODEL(OCARINA_TIME);
    QSET_OOTO_ADULT_MODEL(HOOKSHOT_HOOK);
    QSET_OOTO_ADULT_MODEL(HOOKSHOT_RETICLE);
    QSET_OOTO_ADULT_MODEL(HOOKSHOT);
    QSET_OOTO_ADULT_MODEL(BOW);
    QSET_OOTO_ADULT_MODEL(BOW_STRING);
    QSET_OOTO_ADULT_MODEL(BOTTLE_GLASS);
    QSET_OOTO_ADULT_MODEL(FPS_LFOREARM);
    QSET_OOTO_ADULT_MODEL(FPS_LHAND);
    QSET_OOTO_ADULT_MODEL(FPS_RFOREARM);
    QSET_OOTO_ADULT_MODEL(FPS_RHAND);
    QSET_OOTO_ADULT_MODEL(FPS_HOOKSHOT);
}

void setupZobjZ64o(Link_ModelInfo *modelInfo, u8 *zobj) {
    switch (zobj[Z64O_FORM_BYTE]) {
    case MMO_FORM_BYTE_CHILD:
    case MMO_FORM_BYTE_ADULT:
        setupZobjMmoHuman(modelInfo, zobj);
        break;

    case OOTO_FORM_BYTE_CHILD:
        setupZobjOotoChild(modelInfo, zobj);
        break;

    case OOTO_FORM_BYTE_ADULT:
        setupZobjOotoAdult(modelInfo, zobj);
        break;

    default:
        break;
    }
}
