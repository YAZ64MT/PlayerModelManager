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
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "libc/string.h"

void setupFaceTextures(Link_ModelInfo *modelInfo, u8 *zobj) {
    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        modelInfo->eyesTextures[i] = (TexturePtr)&zobj[Z64O_TEX_EYES_START + Z64O_TEX_EYES_SIZE * i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        modelInfo->mouthTextures[i] = (TexturePtr)&zobj[Z64O_TEX_MOUTH_START + Z64O_TEX_MOUTH_SIZE * i];
    }
}

void repointExternalSegments(u8 *zobj, u32 start, u32 end) {
    u32 current = start;

    while (current < end) {
        applySegmentPtrRemaps(zobj, (Gfx *)SEGMENT_ADDR(0x06, current));
        current += 8;
    }
}

void repointZobjDls(u8 *zobj, u32 start, u32 end) {
    u32 current = start;

    while (current < end) {
        GlobalObjects_globalizeSegmentedDL(zobj, (Gfx *)SEGMENT_ADDR(0x06, current));
        current += 8;
    }
}

typedef struct {
    PlayerLimb limb;
    u32 aliasTableIndex;
} LimbToAlias;

#define DECLARE_Z64O_LIMB_ALIAS(aliasName, modName, sheathDL)                                \
    static LimbToAlias aliasName[] = {                                                       \
        {.limb = PLAYER_LIMB_WAIST, .aliasTableIndex = modName##_LUT_DL_WAIST},              \
        {.limb = PLAYER_LIMB_RIGHT_THIGH, .aliasTableIndex = modName##_LUT_DL_RTHIGH},       \
        {.limb = PLAYER_LIMB_RIGHT_SHIN, .aliasTableIndex = modName##_LUT_DL_RSHIN},         \
        {.limb = PLAYER_LIMB_RIGHT_FOOT, .aliasTableIndex = modName##_LUT_DL_RFOOT},         \
        {.limb = PLAYER_LIMB_LEFT_THIGH, .aliasTableIndex = modName##_LUT_DL_LTHIGH},        \
        {.limb = PLAYER_LIMB_LEFT_SHIN, .aliasTableIndex = modName##_LUT_DL_LSHIN},          \
        {.limb = PLAYER_LIMB_LEFT_FOOT, .aliasTableIndex = modName##_LUT_DL_LFOOT},          \
        {.limb = PLAYER_LIMB_HEAD, .aliasTableIndex = modName##_LUT_DL_HEAD},                \
        {.limb = PLAYER_LIMB_HAT, .aliasTableIndex = modName##_LUT_DL_HAT},                  \
        {.limb = PLAYER_LIMB_COLLAR, .aliasTableIndex = modName##_LUT_DL_COLLAR},            \
        {.limb = PLAYER_LIMB_LEFT_SHOULDER, .aliasTableIndex = modName##_LUT_DL_LSHOULDER},  \
        {.limb = PLAYER_LIMB_LEFT_FOREARM, .aliasTableIndex = modName##_LUT_DL_LFOREARM},    \
        {.limb = PLAYER_LIMB_LEFT_HAND, .aliasTableIndex = modName##_LUT_DL_LHAND},          \
        {.limb = PLAYER_LIMB_RIGHT_SHOULDER, .aliasTableIndex = modName##_LUT_DL_RSHOULDER}, \
        {.limb = PLAYER_LIMB_RIGHT_FOREARM, .aliasTableIndex = modName##_LUT_DL_RFOREARM},   \
        {.limb = PLAYER_LIMB_RIGHT_HAND, .aliasTableIndex = modName##_LUT_DL_RHAND},         \
        {.limb = PLAYER_LIMB_SHEATH, .aliasTableIndex = sheathDL},                           \
        {.limb = PLAYER_LIMB_TORSO, .aliasTableIndex = modName##_LUT_DL_TORSO},              \
    };

DECLARE_Z64O_LIMB_ALIAS(sMMOLimbs, MMO, MMO_LUT_DL_SWORD_KOKIRI_SHEATH);
DECLARE_Z64O_LIMB_ALIAS(sOoTOChildLimbs, OOTO_CHILD, OOTO_CHILD_LUT_DL_SWORD_KOKIRI_SHEATH);
DECLARE_Z64O_LIMB_ALIAS(sOoTOAdultLimbs, OOTO_ADULT, OOTO_ADULT_LUT_DL_SWORD_MASTER_SHEATH);

void handleZobjSkeleton(Link_ModelInfo *modelInfo, u8 *zobj, LimbToAlias limbsToAliases[]) {
    FlexSkeletonHeader *flexHeader = SEGMENTED_TO_GLOBAL_PTR(zobj, readU32(zobj, Z64O_SKELETON_HEADER_POINTER));

    GlobalObjects_globalizeLodLimbSkeleton(zobj, flexHeader);

    for (size_t i = 0; i < ARRAY_COUNT(sMMOLimbs); ++i) {
        LimbToAlias *l2a = &limbsToAliases[i];

        int limbIdx = l2a->limb - 1;
        
        LodLimb *limb = flexHeader->sh.segment[limbIdx];
        
        if (!limb->dLists[0]) {
            gSPBranchList(&zobj[l2a->aliasTableIndex], gCallEmptyDisplayList);
        }
    }

    modelInfo->skeleton = flexHeader;
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

    handleZobjSkeleton(modelInfo, zobj, sMMOLimbs);

    setupFaceTextures(modelInfo, zobj);

    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_B];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_GILDED_BACK] = (Mtx *)&zobj[MMO_MATRIX_SWORD_A];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_HERO_BACK] = (Mtx *)&zobj[MMO_MATRIX_SHIELD_HERO];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK] = (Mtx *)&zobj[MMO_MATRIX_SHIELD_MIRROR];

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
    QSET_MMO_MODEL(SWORD_GREAT_FAIRY_BLADE);
    QSET_MMO_MODEL(SHIELD_HERO);
    modelInfo->models[LINK_DL_SHIELD_MIRROR] = (Gfx *)&zobj[MMO_DL_SHIELD_MIRROR_COMBINED];
    QSET_MMO_MODEL(OCARINA_TIME);
    QSET_MMO_MODEL(BOW);
    QSET_MMO_MODEL(BOW_STRING);
    QSET_MMO_MODEL(HOOKSHOT);
    QSET_MMO_MODEL(HOOKSHOT_HOOK);
    SET_MMO_MODEL(FPS_HOOKSHOT, HOOKSHOT);
    SET_MMO_MODEL(FPS_LFOREARM, LFOREARM);
    SET_MMO_MODEL(FPS_LHAND, LFIST);
    modelInfo->models[LINK_DL_FPS_RFOREARM] = gEmptyDisplayList;
    QSET_MMO_MODEL(FPS_RHAND);
}

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} TunicColor;

static TunicColor sTunicColor = {
    .r = 30,
    .g = 105,
    .b = 27,
};

void fixTunicColor(PlayState *play) {
    if (recomp_get_config_u32("is_modify_tunic_color")) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPSetEnvColor(POLY_OPA_DISP++, sTunicColor.r, sTunicColor.g, sTunicColor.b, 0);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

// Convert char to its numeric value
// Returns 0xFF if invalid character
u8 cToNum(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    
    return 0xFF;
}

u8 sToU8(const char *s) {
    return (cToNum(s[0]) << 4) | cToNum(s[1]);
}

bool isValidHexString(const char* s) {
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

RECOMP_HOOK("Player_Draw")
void readTunicColor_on_Player_DrawGameplay(PlayState *play, Player *this, s32 lod, Gfx *cullDList, OverrideLimbDrawFlex overrideLimbDraw) {
    if (recomp_get_config_u32("is_modify_tunic_color")) {
        char *color = recomp_get_config_string("tunic_color");

        if (color) {
            if (isValidHexString(color)) {
                sTunicColor.r = sToU8(color);
                sTunicColor.g = sToU8(color + 2);
                sTunicColor.b = sToU8(color + 4);
            }
        }

        recomp_free_config_string(color);
    }
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayDefault")
void Recolor_OverrideLimbDrawDefault(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    fixTunicColor(play);
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson")
void Recolor_OverrideLimbDrawFirstPerson(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    fixTunicColor(play);
}

extern Color_RGB8 sPlayerBottleColors[];

RECOMP_HOOK("func_80128640")
void fixBottleEnv_func_80128640(PlayState *play, Player *player, Gfx *dList) {
    if (player->leftHandType == PLAYER_MODELTYPE_LH_BOTTLE) {
        PlayerBottle bottle = Player_BottleFromIA(player, player->itemAction);

        OPEN_DISPS(play->state.gfxCtx);

        Color_RGB8 *bottleColor = &sPlayerBottleColors[bottle];

        gDPSetEnvColor(POLY_XLU_DISP++, bottleColor->r, bottleColor->g, bottleColor->b, 0);

        CLOSE_DISPS(play->state.gfxCtx);
    }
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

    repointExternalSegments(zobj, OOTO_CHILD_LUT_DL_WAIST, OOTO_CHILD_LUT_DL_FPS_RARM_SLINGSHOT);

    repointZobjDls(zobj, OOTO_CHILD_LUT_DL_WAIST, OOTO_CHILD_LUT_DL_FPS_RARM_SLINGSHOT);

    handleZobjSkeleton(modelInfo, zobj, sOoTOChildLimbs);

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
    modelInfo->models[LINK_DL_BOTTLE_CONTENTS] = gEmptyDisplayList;
    QSET_OOTO_CHILD_MODEL(SWORD_KOKIRI_SHEATH);
    QSET_OOTO_CHILD_MODEL(SWORD_KOKIRI_HILT);
    QSET_OOTO_CHILD_MODEL(SWORD_KOKIRI_BLADE);
    SET_OOTO_CHILD_MODEL(SHIELD_HERO, SHIELD_DEKU);
    QSET_OOTO_CHILD_MODEL(OCARINA_TIME);
    SET_OOTO_CHILD_MODEL(FPS_LFOREARM, LFOREARM);
    SET_OOTO_CHILD_MODEL(FPS_LHAND, LFIST);
    modelInfo->models[LINK_DL_FPS_RFOREARM] = gEmptyDisplayList;
    QSET_OOTO_CHILD_MODEL(FPS_RHAND);
}

static Mtx sHookshotHookMatrix;
static Gfx sHookshotHookTranslatedDL[] = {
    gsSPMatrix(&sHookshotHookMatrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW),
    gsSPDisplayList(gEmptyDL),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};

#define SET_OOTO_ADULT_MODEL(dest, src) SET_Z64O_MODEL(dest, src, OOTO_ADULT)
#define QSET_OOTO_ADULT_MODEL(dlName) SET_OOTO_ADULT_MODEL(dlName, dlName)

void setupZobjOotoAdult(Link_ModelInfo *modelInfo, u8 *zobj) {

    clearLinkModelInfo(modelInfo);

    modelInfo->flags |= LINK_MODELINFO_FLAG_MM_ADULT_FIX;

    // OotoFixHeaderSkelPtr MUST run before repointZobjDls to ensure the latter reads the right offset for the skeleton in old zobjs

    // old versions of manifest did not write header ptr
    OotoFixHeaderSkelPtr(zobj);

    repointExternalSegments(zobj, OOTO_ADULT_LUT_DL_WAIST, OOTO_ADULT_LUT_DL_FPS_LHAND_HOOKSHOT);

    repointZobjDls(zobj, OOTO_ADULT_LUT_DL_WAIST, OOTO_ADULT_LUT_DL_FPS_LHAND_HOOKSHOT);

    handleZobjSkeleton(modelInfo, zobj, sOoTOAdultLimbs);

    setupFaceTextures(modelInfo, zobj);

    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK] = (Mtx *)&zobj[OOTO_ADULT_MATRIX_SWORD_BACK];
    modelInfo->equipMtx[LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK] = (Mtx *)&zobj[OOTO_ADULT_MATRIX_SWORD_BACK];
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
    SET_OOTO_ADULT_MODEL(SWORD_KOKIRI_SHEATH, SWORD_MASTER_SHEATH);
    SET_OOTO_ADULT_MODEL(SWORD_KOKIRI_HILT, SWORD_MASTER_HILT);
    SET_OOTO_ADULT_MODEL(SWORD_KOKIRI_BLADE, SWORD_MASTER_BLADE);
    SET_OOTO_ADULT_MODEL(SWORD_RAZOR_SHEATH, SWORD_MASTER_SHEATH);
    SET_OOTO_ADULT_MODEL(SWORD_RAZOR_HILT, SWORD_MASTER_HILT);
    SET_OOTO_ADULT_MODEL(SWORD_RAZOR_BLADE, SWORD_MASTER_BLADE);
    SET_OOTO_ADULT_MODEL(SWORD_GILDED_SHEATH, SWORD_MASTER_SHEATH);
    SET_OOTO_ADULT_MODEL(SWORD_GILDED_HILT, SWORD_MASTER_HILT);
    SET_OOTO_ADULT_MODEL(SWORD_GILDED_BLADE, SWORD_MASTER_BLADE);
    SET_OOTO_ADULT_MODEL(SWORD_GREAT_FAIRY_HILT, BIGSWORD_HILT);
    SET_OOTO_ADULT_MODEL(SWORD_GREAT_FAIRY_BLADE, BIGSWORD_BLADE);
    SET_OOTO_ADULT_MODEL(SWORD_FIERCE_DEITY_HILT, BIGSWORD_HILT);
    SET_OOTO_ADULT_MODEL(SWORD_FIERCE_DEITY_BLADE, BIGSWORD_BLADE);
    SET_OOTO_ADULT_MODEL(SHIELD_HERO, SHIELD_HYLIAN);
    QSET_OOTO_ADULT_MODEL(SHIELD_MIRROR);
    QSET_OOTO_ADULT_MODEL(OCARINA_TIME);

    modelInfo->models[LINK_DL_HOOKSHOT_HOOK] = sHookshotHookTranslatedDL;
    gSPDisplayList(&sHookshotHookTranslatedDL[1], &zobj[OOTO_ADULT_LUT_DL_HOOKSHOT_HOOK]);

    QSET_OOTO_ADULT_MODEL(HOOKSHOT_CHAIN);
    QSET_OOTO_ADULT_MODEL(HOOKSHOT_RETICLE);
    QSET_OOTO_ADULT_MODEL(HOOKSHOT);
    QSET_OOTO_ADULT_MODEL(BOW);
    QSET_OOTO_ADULT_MODEL(BOW_STRING);
    QSET_OOTO_ADULT_MODEL(BOTTLE_GLASS);
    modelInfo->models[LINK_DL_BOTTLE_CONTENTS] = gEmptyDisplayList;
    QSET_OOTO_ADULT_MODEL(FPS_LFOREARM);
    QSET_OOTO_ADULT_MODEL(FPS_LHAND);
    QSET_OOTO_ADULT_MODEL(FPS_RFOREARM);
    QSET_OOTO_ADULT_MODEL(FPS_RHAND);
    QSET_OOTO_ADULT_MODEL(FPS_HOOKSHOT);
}

void setupZobjZ64O(Link_ModelInfo *modelInfo, u8 *zobj) {
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

u64 gOotHylianShieldTex[] = {
#include "crossgame/oot/assets/objects/gameplay_keep/hylian_shield_design.rgba16.inc.c"
};

u64 gOotBottleGlassTex[] = {
#include "crossgame/oot/assets/objects/gameplay_keep/bottle_glass.rgba16.inc.c"
};

void remapSegmentPtrs() {

#define OOT_GK_HILITE_1_TEX 0x04000000
#define OOT_GK_OCARINA_OF_TIME_TEX 0x04001400
#define OOT_GK_DEKU_STICK_TEX 0x04001A00
#define OOT_GK_HYLIAN_SHIELD_TEX 0x04000400
#define OOT_GK_BOTTLE_GLASS_TEX 0x04001800
#define PTR_OBJ_REMAP_SET(obj, key, val) setSegmentPtrRemap(key, (u32)SEGMENTED_TO_GLOBAL_PTR(obj, val))
#define PTR_GK_REMAP_SET(key, val) PTR_OBJ_REMAP_SET(gk, key, val);

    void *gk = GlobalObjects_getGlobalObject(GAMEPLAY_KEEP);

    void *human = GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD);

    PTR_GK_REMAP_SET(OOT_GK_HILITE_1_TEX, gameplay_keep_Tex_00C830);
    PTR_GK_REMAP_SET(OOT_GK_DEKU_STICK_TEX, gDekuStickTex);

    PTR_OBJ_REMAP_SET(human, OOT_GK_OCARINA_OF_TIME_TEX, gLinkHumanOcarinaTex);

    setSegmentPtrRemap(OOT_GK_HYLIAN_SHIELD_TEX, (u32)gOotHylianShieldTex);
    setSegmentPtrRemap(OOT_GK_BOTTLE_GLASS_TEX, (u32)gOotBottleGlassTex);
}

RECOMP_DECLARE_EVENT(_internal_onReadyML64Compat());

RECOMP_CALLBACK(".", _internal_onReadyML64CompatBase)
void initML64CompatMM_onReadyML64CompatBase() {
    remapSegmentPtrs();

    guPosition(&sHookshotHookMatrix, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 800.0f);

    _internal_onReadyML64Compat();
}
