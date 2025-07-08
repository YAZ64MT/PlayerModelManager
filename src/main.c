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
#include "modelreplacements.h"
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
    REFRESH_DL(PAD_OPENING);

#undef REFRESH_DL
}

RECOMP_DECLARE_EVENT(_internal_setupVanillaModels());

// initialize player models as blank display lists
void initFormProxies() {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_FormProxy *formProxy = &gLinkFormProxies[i];

        clearLinkModelInfo(&formProxy->current);

        initFormProxy(formProxy, i);

        // vanilla forms share these segmented ptrs
        for (int j = 0; j < PLAYER_EYES_MAX; ++j) {
            formProxy->vanilla.eyesTextures[j] = sPlayerEyesTextures[j];
        }

        for (int j = 0; j < PLAYER_MOUTH_MAX; ++j) {
            formProxy->vanilla.mouthTextures[j] = sPlayerMouthTextures[j];
        }

        requestRefreshFormProxy(&gLinkFormProxies[i]);
        changeFormPtrsToProxy(i);
    }

    _internal_setupVanillaModels();

    repointSharedModelsToProxy();

    requestRefreshFaceTextures();
}

RECOMP_HOOK("Player_Init")
void refreshDLs_on_PlayerInit(Actor *thisx, PlayState *play) {
    if ((Player *)thisx == GET_PLAYER(play)) {
        requestRefreshFaceTextures();

        refreshExternalDLs(GET_PLAYER_FORM_PROXY);

        refreshSharedModels();

        gIsAgePropertyRefreshRequested = true;
    }
}

static bool sPushedMaskMatrix = false;
extern LinkAnimationHeader gPlayerAnim_cl_setmask;
RECOMP_HOOK("Player_PostLimbDrawGameplay")
void useMaskMtx_on_Player_PostLimbDrawGameplay(PlayState *play, s32 limbIndex, Gfx **dList1, Gfx **dList2, Vec3s *rot, Actor *actor) {
    sPushedMaskMatrix = false;

    Mtx *maskMtx = GET_PLAYER_FORM_PROXY->current.equipMtx[LINK_EQUIP_MATRIX_MASKS];

    if (limbIndex == PLAYER_LIMB_HEAD && maskMtx) {
        Player *player = (Player *)actor;
        if (((*dList1 != NULL) && ((u32)player->currentMask != PLAYER_MASK_NONE)) &&
            (((player->transformation == PLAYER_FORM_HUMAN) &&
              ((player->skelAnime.animation != &gPlayerAnim_cl_setmask) || (player->skelAnime.curFrame >= 12.0f))) ||
             ((((player->transformation != PLAYER_FORM_HUMAN) && (player->currentMask >= PLAYER_MASK_FIERCE_DEITY)) &&
               ((player->transformation + PLAYER_MASK_FIERCE_DEITY) != player->currentMask)) &&
              (player->skelAnime.curFrame >= 10.0f)))) {
            s32 maskMinusOne = player->currentMask - 1;
            OPEN_DISPS(play->state.gfxCtx);
            Matrix_Push();
            sPushedMaskMatrix = true;
            static MtxF sMaskMtxF;
            Matrix_MtxToMtxF(maskMtx, &sMaskMtxF);
            Matrix_Mult(&sMaskMtxF, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            CLOSE_DISPS(play->state.gfxCtx);
        }
    }
}

RECOMP_HOOK_RETURN("Player_PostLimbDrawGameplay")
void useMaskMtx_on_return_Player_PostLimbDrawGameplay(void) {
    if (sPushedMaskMatrix) {
        Matrix_Pop();
    }
}

RECOMP_DECLARE_EVENT(_internal_onReadyFormProxies());

RECOMP_CALLBACK(".", _internal_onReadyModelReplacerCompat)
void initFormProxies_on_event() {
    initFormProxies();
    _internal_onReadyFormProxies();
}

RECOMP_CALLBACK(".", _internal_onModelApplied)
void refreshSharedModelsOnModelApply(PlayerTransformation form) {
    requestRefreshFormProxy(&gLinkFormProxies[form]);
    gIsAgePropertyRefreshRequested = true;

    if (form == GET_PLAYER_FORM) {
        refreshSharedModels();
        requestRefreshFaceTextures();
    }
}

RECOMP_DECLARE_EVENT(_internal_initHashObjects());

RECOMP_DECLARE_EVENT(_internal_initDictionary());

RECOMP_CALLBACK("*", recomp_on_init)
void handleInits() {
    _internal_initDictionary();
    _internal_initHashObjects();
}
