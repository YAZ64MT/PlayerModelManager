#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_mm.h"
#include "simplefileloader.h"
#include "model_common.h"

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());

extern Gfx *gPlayerRightHandOpenDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerRightHandClosedDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerRightHandBowDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerRightHandInstrumentDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerRightHandHookshotDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerHandHoldingShields[];

extern Gfx *gPlayerLeftHandOpenDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerLeftHandClosedDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerLeftHandTwoHandSwordDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerLeftHandOneHandSwordDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerLeftHandBottleDLs[2 * PLAYER_FORM_MAX];
extern Gfx *gPlayerWaistDLs[2 * PLAYER_FORM_MAX];

extern Gfx *sPlayerFirstPersonLeftHandDLs[PLAYER_FORM_MAX];
extern Gfx *sPlayerFirstPersonLeftForearmDLs[PLAYER_FORM_MAX];
extern Gfx *sPlayerFirstPersonRightShoulderDLs[PLAYER_FORM_MAX];
extern Gfx *sPlayerFirstPersonRightHandDLs[PLAYER_FORM_MAX];
extern Gfx *sPlayerFirstPersonRightHandHookshotDLs[PLAYER_FORM_MAX];

extern Gfx *gLinkHumanGildedSwordHandleDL[];
extern Gfx *gLinkHumanGildedSwordBladeDL[];

extern Gfx *gPlayerRightHandBowDLs[2 * PLAYER_FORM_MAX];

extern Gfx *D_801C018C[];

extern Gfx *gPlayerSheathedSwords[];
extern Gfx *gPlayerSwordSheaths[];
extern Gfx *gPlayerShields[];

extern TexturePtr sPlayerEyesTextures[];
extern TexturePtr sPlayerMouthTextures[];

extern TexturePtr gLinkHumanEyesOpenTex;        // PLAYER_EYES_OPEN
extern TexturePtr gLinkHumanEyesHalfTex;        // PLAYER_EYES_HALF
extern TexturePtr gLinkHumanEyesClosedTex;      // PLAYER_EYES_CLOSED
extern TexturePtr gLinkHumanEyesRollRightTex;   // PLAYER_EYES_ROLL_RIGHT
extern TexturePtr gLinkHumanEyesRollLeftTex;    // PLAYER_EYES_ROLL_LEFT
extern TexturePtr gLinkHumanEyesRollUpTex;      // PLAYER_EYES_ROLL_UP
extern TexturePtr gLinkHumanEyesRollDownTex;    // PLAYER_EYES_ROLL_DOWN
extern TexturePtr object_link_child_Tex_003800; // PLAYER_EYES_7

extern TexturePtr gLinkHumanMouthClosedTex; // PLAYER_MOUTH_CLOSED
extern TexturePtr gLinkHumanMouthTeethTex;  // PLAYER_MOUTH_TEETH
extern TexturePtr gLinkHumanMouthAngryTex;  // PLAYER_MOUTH_ANGRY
extern TexturePtr gLinkHumanMouthHappyTex;  // PLAYER_MOUTH_HAPPY

static Link_FormProxy sLinkFormProxies[PLAYER_FORM_MAX];

RECOMP_EXPORT Link_FormProxy* getLinkFormProxies() {
    return sLinkFormProxies;
}

void changeFormPtrsToProxy(PlayerTransformation playerForm) {
    Link_FormProxy *formProxy = &sLinkFormProxies[playerForm];

    gPlayerSkeletons[playerForm] = &formProxy->skeleton;
    gPlayerRightHandOpenDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RHAND];
    gPlayerRightHandOpenDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RHAND];
    gPlayerRightHandClosedDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RFIST];
    gPlayerRightHandClosedDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RFIST];
    gPlayerRightHandInstrumentDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RHAND_OCARINA_TIME];
    gPlayerRightHandInstrumentDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RHAND_OCARINA_TIME];
    gPlayerRightHandHookshotDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RFIST_HOOKSHOT];
    gPlayerRightHandHookshotDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RFIST_HOOKSHOT];

    gPlayerLeftHandOpenDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_LHAND];
    gPlayerLeftHandOpenDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_LHAND];
    gPlayerLeftHandClosedDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_LFIST];
    gPlayerLeftHandClosedDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_LFIST];
    gPlayerLeftHandTwoHandSwordDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_LFIST_SWORD4]; // Great Fairy Sword / Fierce Deity Sword
    gPlayerLeftHandTwoHandSwordDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_LFIST_SWORD4];
    gPlayerLeftHandOneHandSwordDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    gPlayerLeftHandOneHandSwordDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    gPlayerLeftHandBottleDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_LHAND_BOTTLE];
    gPlayerLeftHandBottleDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_LHAND_BOTTLE];

    gPlayerRightHandBowDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RFIST_BOW];
    gPlayerRightHandBowDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RFIST_BOW];

    sPlayerFirstPersonLeftForearmDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_LFOREARM];
    sPlayerFirstPersonLeftHandDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_LHAND];
    sPlayerFirstPersonRightShoulderDLs[playerForm] = &formProxy->displayLists[LINK_DL_RSHOULDER];
    sPlayerFirstPersonRightHandDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_RHAND];
    sPlayerFirstPersonRightHandHookshotDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_RHAND_HOOKSHOT];

    gPlayerWaistDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_WAIST];
    gPlayerWaistDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_WAIST];
}

void repointHumanEquipmentModelsToProxy(PlayerTransformation playerForm) {
    Link_FormProxy *formProxy = &sLinkFormProxies[playerForm];

    D_801C018C[0] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    D_801C018C[1] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    D_801C018C[2] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_RAZOR];
    D_801C018C[3] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_RAZOR];
    D_801C018C[4] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_GILDED];
    D_801C018C[5] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_GILDED];

    gPlayerShields[0] = &formProxy->displayLists[LINK_DL_SHIELD_HERO_BACK];
    gPlayerShields[1] = &formProxy->displayLists[LINK_DL_SHIELD_HERO_BACK];
    gPlayerShields[2] = &formProxy->displayLists[LINK_DL_SHIELD_MIRROR_BACK];
    gPlayerShields[3] = &formProxy->displayLists[LINK_DL_SHIELD_MIRROR_BACK];

    gPlayerSheathedSwords[0] = &formProxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[1] = &formProxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[2] = &formProxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[3] = &formProxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[4] = &formProxy->displayLists[LINK_DL_SWORD_GILDED_SHEATHED];
    gPlayerSheathedSwords[5] = &formProxy->displayLists[LINK_DL_SWORD_GILDED_SHEATHED];

    gPlayerSwordSheaths[0] = &formProxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[1] = &formProxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[2] = &formProxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[3] = &formProxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[4] = &formProxy->displayLists[LINK_DL_SWORD_GILDED_SHEATH];
    gPlayerSwordSheaths[5] = &formProxy->displayLists[LINK_DL_SWORD_GILDED_SHEATH];
}

// initialize player models as blank display lists
void initFormProxies() {
    for (u8 i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_FormProxy *formProxy = &sLinkFormProxies[i];

        formProxy->vanilla.skeleton = formProxy->current.skeleton = gPlayerSkeletons[i];

        initFormProxy(formProxy);

        // vanilla forms share these ptrs
        TexturePtr* eyesTex = formProxy->vanilla.eyesTextures;
        eyesTex[0] = gLinkHumanEyesOpenTex;
        eyesTex[1] = gLinkHumanEyesHalfTex;
        eyesTex[2] = gLinkHumanEyesClosedTex;
        eyesTex[3] = gLinkHumanEyesRollRightTex;
        eyesTex[4] = gLinkHumanEyesRollLeftTex;
        eyesTex[5] = gLinkHumanEyesRollUpTex;
        eyesTex[6] = gLinkHumanEyesRollDownTex;
        eyesTex[7] = object_link_child_Tex_003800;

        TexturePtr *mouthTex = formProxy->vanilla.mouthTextures;
        eyesTex[0] = gLinkHumanMouthClosedTex;
        eyesTex[1] = gLinkHumanMouthTeethTex;
        eyesTex[2] = gLinkHumanMouthAngryTex;
        eyesTex[3] = gLinkHumanMouthHappyTex;

        // uncomment these when all forms supported
        // changeFormPtrsToProxy(i);
    }

    // Only human is supported for now
    changeFormPtrsToProxy(PLAYER_FORM_HUMAN);
    repointHumanEquipmentModelsToProxy(PLAYER_FORM_HUMAN);
    refreshFormProxy(&sLinkFormProxies[PLAYER_FORM_HUMAN]);
}

RECOMP_CALLBACK("*", recomp_on_play_main)
void onPlayMain(PlayState *play) {
    initFormProxies();
}
