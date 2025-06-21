#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_mm.h"
#include "model_common.h"
#include "model_human.h"
#include "mm_adultfixes.h"
#include "playermodelmanager_utils.h"
#include "modelreplacer_api.h"
#include "externaldisplaylists.h"
#include "externs_z_player_lib.h"
#include "model_shared.h"

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());

Link_FormProxy gLinkFormProxies[PLAYER_FORM_MAX];

void changeFormPtrsToProxy(PlayerTransformation playerForm) {
    Link_FormProxy *formProxy = &gLinkFormProxies[playerForm];

    gPlayerSkeletons[playerForm] = &formProxy->skeleton.flexSkeleton;
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

    Gfx *twoHandedSwordDL = &formProxy->displayLists[LINK_DL_LFIST_SWORD_GREAT_FAIRY];
    if (playerForm == PLAYER_FORM_FIERCE_DEITY) {
        twoHandedSwordDL = &formProxy->displayLists[LINK_DL_LFIST_SWORD_FIERCE_DEITY];
    }

    gPlayerLeftHandTwoHandSwordDLs[playerForm * 2 + 0] = twoHandedSwordDL;
    gPlayerLeftHandTwoHandSwordDLs[playerForm * 2 + 1] = twoHandedSwordDL;
    gPlayerLeftHandOneHandSwordDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    gPlayerLeftHandOneHandSwordDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    gPlayerLeftHandBottleDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_LHAND_BOTTLE];
    gPlayerLeftHandBottleDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_LHAND_BOTTLE];

    gPlayerRightHandBowDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RFIST_BOW];
    gPlayerRightHandBowDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RFIST_BOW];

    sPlayerFirstPersonLeftForearmDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_LFOREARM];
    sPlayerFirstPersonLeftHandDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_LHAND];
    sPlayerFirstPersonRightShoulderDLs[playerForm] = &formProxy->displayLists[LINK_DL_RSHOULDER];
    sPlayerFirstPersonRightHandDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_RHAND_BOW];
    sPlayerFirstPersonRightHandHookshotDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_RHAND_HOOKSHOT];

    gPlayerWaistDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_WAIST];
    gPlayerWaistDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_WAIST];
}

static Gfx **sPlayerHandHoldingSwords = D_801C018C;

static Gfx **sPlayerMasks = D_801C0B20;

static Gfx **sPlayerFins = D_801C0AB4;

static Gfx **sPlayerSwimFins = D_801C0ABC;

static Gfx **sPlayerPads = D_8085D574;

void repointSharedModelsToProxy() {
    sPlayerHandHoldingSwords[0] = &gPlayerLibDLs[PLAYERLIB_DL_LFIST_SWORD_KOKIRI];
    sPlayerHandHoldingSwords[1] = &gPlayerLibDLs[PLAYERLIB_DL_LFIST_SWORD_KOKIRI];
    sPlayerHandHoldingSwords[2] = &gPlayerLibDLs[PLAYERLIB_DL_LFIST_SWORD_RAZOR];
    sPlayerHandHoldingSwords[3] = &gPlayerLibDLs[PLAYERLIB_DL_LFIST_SWORD_RAZOR];
    sPlayerHandHoldingSwords[4] = &gPlayerLibDLs[PLAYERLIB_DL_LFIST_SWORD_GILDED];
    sPlayerHandHoldingSwords[5] = &gPlayerLibDLs[PLAYERLIB_DL_LFIST_SWORD_GILDED];

    gPlayerShields[0] = &gPlayerLibDLs[PLAYERLIB_DL_SHIELD_HERO_BACK];
    gPlayerShields[1] = &gPlayerLibDLs[PLAYERLIB_DL_SHIELD_HERO_BACK];
    gPlayerShields[2] = &gPlayerLibDLs[PLAYERLIB_DL_SHIELD_MIRROR_BACK];
    gPlayerShields[3] = &gPlayerLibDLs[PLAYERLIB_DL_SHIELD_MIRROR_BACK];

    gPlayerHandHoldingShields[0] = &gPlayerLibDLs[PLAYERLIB_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[1] = &gPlayerLibDLs[PLAYERLIB_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[2] = &gPlayerLibDLs[PLAYERLIB_DL_RFIST_SHIELD_MIRROR];
    gPlayerHandHoldingShields[3] = &gPlayerLibDLs[PLAYERLIB_DL_RFIST_SHIELD_MIRROR];

    gPlayerSheathedSwords[0] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[1] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[2] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[3] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[4] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_GILDED_SHEATHED];
    gPlayerSheathedSwords[5] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_GILDED_SHEATHED];

    gPlayerSwordSheaths[0] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[1] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[2] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[3] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[4] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_GILDED_SHEATH];
    gPlayerSwordSheaths[5] = &gPlayerLibDLs[PLAYERLIB_DL_SWORD_GILDED_SHEATH];

    sPlayerFins[0] = &gPlayerLibDLs[PLAYERLIB_DL_LFIN];
    sPlayerFins[1] = &gPlayerLibDLs[PLAYERLIB_DL_RFIN];

    sPlayerSwimFins[0] = &gPlayerLibDLs[PLAYERLIB_DL_LFIN_SWIM];
    sPlayerSwimFins[1] = &gPlayerLibDLs[PLAYERLIB_DL_RFIN_SWIM];

    sPlayerPads[0] = &gPlayerLibDLs[PLAYERLIB_DL_PAD_GRASS];
    sPlayerPads[1] = &gPlayerLibDLs[PLAYERLIB_DL_PAD_WOOD];
    sPlayerPads[2] = &gPlayerLibDLs[PLAYERLIB_DL_PAD_OPENING];
}

void refreshSharedModels() {
    Link_FormProxy *currProxy = GET_PLAYER_FORM_PROXY;
    Link_FormProxy *human = &gLinkFormProxies[PLAYER_FORM_HUMAN];

#define REFRESH_DL(name) gSPBranchList(&gPlayerLibDLs[PLAYERLIB_DL_##name], &currProxy->displayLists[LINK_DL_##name])

    REFRESH_DL(LFIST_SWORD_KOKIRI);
    REFRESH_DL(LFIST_SWORD_RAZOR);
    REFRESH_DL(LFIST_SWORD_GILDED);

    REFRESH_DL(SHIELD_HERO_BACK);
    REFRESH_DL(SHIELD_MIRROR_BACK);

    REFRESH_DL(RFIST_SHIELD_HERO);
    REFRESH_DL(RFIST_SHIELD_MIRROR);

    REFRESH_DL(SWORD_KOKIRI_SHEATHED);
    REFRESH_DL(SWORD_RAZOR_SHEATHED);
    REFRESH_DL(SWORD_GILDED_SHEATHED);

    REFRESH_DL(SWORD_KOKIRI_SHEATH);
    REFRESH_DL(SWORD_RAZOR_SHEATH);
    REFRESH_DL(SWORD_GILDED_SHEATH);

    REFRESH_DL(LFIN);
    REFRESH_DL(RFIN);
    REFRESH_DL(LFIN_SWIM);
    REFRESH_DL(RFIN_SWIM);

    REFRESH_DL(PAD_GRASS);
    REFRESH_DL(PAD_WOOD);
    REFRESH_DL(PIPE_MOUTH);

    REFRESH_DL(PIPE_MOUTH);
    REFRESH_DL(PIPE_RIGHT);
    REFRESH_DL(PIPE_UP);
    REFRESH_DL(PIPE_DOWN);
    REFRESH_DL(PIPE_LEFT);
    REFRESH_DL(PIPE_A);

#undef REFRESH_DL
}

RECOMP_DECLARE_EVENT(_internal_setupVanillaModels());

// initialize player models as blank display lists
void initFormProxies() {
    for (u8 i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_FormProxy *formProxy = &gLinkFormProxies[i];

        clearLinkModelInfo(&formProxy->current);

        initFormProxy(formProxy, i);

        // vanilla forms share these segmented ptrs
        for (u32 j = 0; j < PLAYER_EYES_MAX; ++j) {
            formProxy->vanilla.eyesTextures[j] = sPlayerEyesTextures[j];
        }

        for (u32 j = 0; j < PLAYER_MOUTH_MAX; ++j) {
            formProxy->vanilla.mouthTextures[j] = sPlayerMouthTextures[j];
        }

        requestRefreshFormProxy(i);
        changeFormPtrsToProxy(i);
    }

    _internal_setupVanillaModels();

    repointSharedModelsToProxy();

    refreshFaceTextures();
}

RECOMP_HOOK_RETURN("Player_Init")
void refreshDLs_on_return_PlayerInit() {
    refreshFaceTextures();

    refreshExternalDLs(GET_PLAYER_FORM_PROXY);

    refreshSharedModels();

    gIsAgePropertyRefreshRequested = true;
}

RECOMP_DECLARE_EVENT(_internal_onReadyFormProxies());

RECOMP_CALLBACK(".", _internal_onReadyModelReplacerCompat)
void initFormProxies_on_event() {
    initFormProxies();
    _internal_onReadyFormProxies();
}

RECOMP_CALLBACK(".", _internal_onModelApplied)
void refreshSharedModelsOnModelApply(PlayerTransformation form) {
    requestRefreshFormProxy(form);
    gIsAgePropertyRefreshRequested = true;

    if (form == GET_PLAYER_FORM) {
        refreshSharedModels();
        refreshFaceTextures();
    }
}
