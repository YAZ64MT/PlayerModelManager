#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager.h"
#include "simplefileloader.h"

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

Gfx dfCommand[] = {
    gsSPEndDisplayList(),
};

Gfx callDfCommand[] = {
    gsSPBranchList(dfCommand),
};

Mtx zeroMtx = {0};

static Link_FormModel sLinkFormModels[PLAYER_FORM_MAX];

const char *zobjDir = NULL;

void updateProxy(PlayerTransformation playerForm, Gfx** models) {
    Link_FormModel *modelInfo = &sLinkFormModels[playerForm];

    for (u32 i = 0; i < LINK_DL_MAX; ++i) {
        modelInfo->proxyDisplayLists[i].words.w1 = (u32)models[i];
    }
}

void restoreVanillaModel(PlayerTransformation playerForm) {
    updateProxy(playerForm, sLinkFormModels[playerForm].vanilla.models);
}

void changeFormPtrsToProxy(PlayerTransformation playerForm) {
    Link_FormModel *modelInfo = &sLinkFormModels[playerForm];

    gPlayerSkeletons[playerForm] = &modelInfo->proxySkeleton;
    gPlayerRightHandOpenDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_RHAND];
    gPlayerRightHandOpenDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_RHAND];
    gPlayerRightHandClosedDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_RFIST];
    gPlayerRightHandClosedDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_RFIST];
    gPlayerRightHandInstrumentDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_RHAND_OCARINA_TIME];
    gPlayerRightHandInstrumentDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_RHAND_OCARINA_TIME];
    gPlayerRightHandHookshotDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_RFIST_HOOKSHOT];
    gPlayerRightHandHookshotDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_RFIST_HOOKSHOT];

    gPlayerLeftHandOpenDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_LHAND];
    gPlayerLeftHandOpenDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_LHAND];
    gPlayerLeftHandClosedDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST];
    gPlayerLeftHandClosedDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST];
    gPlayerLeftHandTwoHandSwordDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD4];
    gPlayerLeftHandTwoHandSwordDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD4];
    gPlayerLeftHandOneHandSwordDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD1];
    gPlayerLeftHandOneHandSwordDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD1];
    gPlayerLeftHandBottleDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_LHAND_BOTTLE];
    gPlayerLeftHandBottleDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_LHAND_BOTTLE];

    gPlayerRightHandBowDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_RFIST_BOW];
    gPlayerRightHandBowDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_RFIST_BOW];

    sPlayerFirstPersonLeftForearmDLs[playerForm] = &modelInfo->proxyDisplayLists[LINK_DL_FPS_LFOREARM];
    sPlayerFirstPersonLeftHandDLs[playerForm] = &modelInfo->proxyDisplayLists[LINK_DL_FPS_LHAND];
    sPlayerFirstPersonRightShoulderDLs[playerForm] = &modelInfo->proxyDisplayLists[LINK_DL_RSHOULDER];
    sPlayerFirstPersonRightHandDLs[playerForm] = &modelInfo->proxyDisplayLists[LINK_DL_FPS_RARM_BOW];
    sPlayerFirstPersonRightHandHookshotDLs[playerForm] = &modelInfo->proxyDisplayLists[LINK_DL_FPS_RARM_HOOKSHOT];

    gPlayerWaistDLs[playerForm * 2 + 0] = &modelInfo->proxyDisplayLists[LINK_DL_WAIST];
    gPlayerWaistDLs[playerForm * 2 + 1] = &modelInfo->proxyDisplayLists[LINK_DL_WAIST];
}

void repointHumanSpecificModelsToProxy() {
    Link_FormModel *modelInfo = &sLinkFormModels[PLAYER_FORM_HUMAN];

    D_801C018C[0] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD1];
    D_801C018C[1] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD1];
    D_801C018C[2] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD2];
    D_801C018C[3] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD2];
    D_801C018C[4] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD3];
    D_801C018C[5] = &modelInfo->proxyDisplayLists[LINK_DL_LFIST_SWORD4];

    gPlayerShields[0] = &modelInfo->proxyDisplayLists[LINK_DL_SHIELD1_BACK];
    gPlayerShields[1] = &modelInfo->proxyDisplayLists[LINK_DL_SHIELD1_BACK];
    gPlayerShields[2] = &modelInfo->proxyDisplayLists[LINK_DL_SHIELD2_BACK];
    gPlayerShields[3] = &modelInfo->proxyDisplayLists[LINK_DL_SHIELD2_BACK];

    gPlayerSheathedSwords[0] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD1_SHEATHED];
    gPlayerSheathedSwords[1] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD1_SHEATHED];
    gPlayerSheathedSwords[2] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD2_SHEATHED];
    gPlayerSheathedSwords[3] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD2_SHEATHED];
    gPlayerSheathedSwords[4] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD3_SHEATHED];
    gPlayerSheathedSwords[5] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD3_SHEATHED];

    gPlayerSwordSheaths[0] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD_SHEATH_1];
    gPlayerSwordSheaths[1] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD_SHEATH_1];
    gPlayerSwordSheaths[2] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD_SHEATH_2];
    gPlayerSwordSheaths[3] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD_SHEATH_2];
    gPlayerSwordSheaths[4] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD_SHEATH_3];
    gPlayerSwordSheaths[5] = &modelInfo->proxyDisplayLists[LINK_DL_SWORD_SHEATH_3];
}

// initialize player models as blank display lists
void initPlayerModels() {
    for (u8 i = 0; i < PLAYER_FORM_MAX; ++i) {
        sLinkFormModels[i].vanilla.skeleton = sLinkFormModels[i].current.skeleton = gPlayerSkeletons[i];

        for (u16 j = 0; j < LINK_DL_MAX; ++j) {
            sLinkFormModels[i].proxyDisplayLists[j].words = callDfCommand[0].words;
        }

        for (u16 j = 0; j < LINK_EQUIP_MATRIX_MAX; ++j) {
            sLinkFormModels[i].vanilla.equipMtx[j] = &zeroMtx;
        }

        // uncomment these when all forms supported
        // changeFormPtrsToProxy(i);
    }

    // Only human is supported for now
    changeFormPtrsToProxy(PLAYER_FORM_HUMAN);
    repointHumanSpecificModelsToProxy();
    restoreVanillaModel(PLAYER_FORM_HUMAN);
}

RECOMP_CALLBACK("*", recomp_on_play_main)
void onPlayMain(PlayState *play) {
    if (!zobjDir) {
        char *modsPath = (char *)recomp_get_mod_folder_path();
        zobjDir = SFL_getCombinedPath(2, modsPath, zobjDir);
        recomp_free(modsPath);
    }
    initPlayerModels();
}
