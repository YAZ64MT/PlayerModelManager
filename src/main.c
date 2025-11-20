#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_utils.h"
#include "modelreplacer_api.h"
#include "modelreplacer_compat.h"
#include "globalobjects_api.h"
#include "externs_z_player_lib.h"
#include "model_shared.h"
#include "equipmentoverrides.h"
#include "z64recomp_api.h"
#include "playerproxy.h"
#include "formproxy.h"
#include "proxyactorext.h"
#include "playerproxymanager.h"

static bool sShouldSkipFormInterpolation[PLAYER_FORM_MAX];
static bool sShouldSkipMirrorShieldInterpolation;
static bool sShouldSkipHookshotInterpolation;

PlayerProxy *gPlayer1Proxy;

void repointFormPtrsToProxy(FormProxy *formProxy, PlayerTransformation playerForm) {
    gPlayerRightHandOpenDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RHAND];
    gPlayerRightHandOpenDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RHAND];
    gPlayerRightHandClosedDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RFIST];
    gPlayerRightHandClosedDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RFIST];

    if (playerForm == PLAYER_FORM_HUMAN || playerForm == PLAYER_FORM_FIERCE_DEITY) {
        gPlayerRightHandInstrumentDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RHAND_OCARINA_TIME];
        gPlayerRightHandInstrumentDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RHAND_OCARINA_TIME];
    } else {
        gPlayerRightHandInstrumentDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_RHAND];
        gPlayerRightHandInstrumentDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_RHAND];
    }

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

static Gfx **sPlayerMaskDLs = D_801C0B20;

static EnBoomStruct *sPlayerBoomerangInfo = D_808A3078;

void repointSharedModelsToProxy(FormProxy *proxy) {
    sPlayerHandHoldingSwords[0] = &proxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    sPlayerHandHoldingSwords[1] = &proxy->displayLists[LINK_DL_LFIST_SWORD_KOKIRI];
    sPlayerHandHoldingSwords[2] = &proxy->displayLists[LINK_DL_LFIST_SWORD_RAZOR];
    sPlayerHandHoldingSwords[3] = &proxy->displayLists[LINK_DL_LFIST_SWORD_RAZOR];
    sPlayerHandHoldingSwords[4] = &proxy->displayLists[LINK_DL_LFIST_SWORD_GILDED];
    sPlayerHandHoldingSwords[5] = &proxy->displayLists[LINK_DL_LFIST_SWORD_GILDED];

    gPlayerShields[0] = &proxy->displayLists[LINK_DL_SHIELD_HERO_BACK];
    gPlayerShields[1] = &proxy->displayLists[LINK_DL_SHIELD_HERO_BACK];
    gPlayerShields[2] = &proxy->displayLists[LINK_DL_SHIELD_MIRROR_BACK];
    gPlayerShields[3] = &proxy->displayLists[LINK_DL_SHIELD_MIRROR_BACK];

    gPlayerHandHoldingShields[0] = &proxy->displayLists[LINK_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[1] = &proxy->displayLists[LINK_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[2] = &proxy->displayLists[LINK_DL_RFIST_SHIELD_MIRROR];
    gPlayerHandHoldingShields[3] = &proxy->displayLists[LINK_DL_RFIST_SHIELD_MIRROR];

    gPlayerSheathedSwords[0] = &proxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[1] = &proxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[2] = &proxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[3] = &proxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[4] = &proxy->displayLists[LINK_DL_SWORD_GILDED_SHEATHED];
    gPlayerSheathedSwords[5] = &proxy->displayLists[LINK_DL_SWORD_GILDED_SHEATHED];

    gPlayerSwordSheaths[0] = &proxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[1] = &proxy->displayLists[LINK_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[2] = &proxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[3] = &proxy->displayLists[LINK_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[4] = &proxy->displayLists[LINK_DL_SWORD_GILDED_SHEATH];
    gPlayerSwordSheaths[5] = &proxy->displayLists[LINK_DL_SWORD_GILDED_SHEATH];

    sPlayerFins[0] = &proxy->displayLists[LINK_DL_LFIN];
    sPlayerFins[1] = &proxy->displayLists[LINK_DL_RFIN];

    sPlayerSwimFins[0] = &proxy->displayLists[LINK_DL_LFIN_SWIM];
    sPlayerSwimFins[1] = &proxy->displayLists[LINK_DL_RFIN_SWIM];

    sPlayerPads[0] = &proxy->displayLists[LINK_DL_PAD_GRASS];
    sPlayerPads[1] = &proxy->displayLists[LINK_DL_PAD_WOOD];
    sPlayerPads[2] = &proxy->displayLists[LINK_DL_PAD_OPENING];

    sPlayerBoomerangInfo[0].unk_00 = &proxy->displayLists[LINK_DL_LFIN_BOOMERANG];
    sPlayerBoomerangInfo[1].unk_00 = &proxy->displayLists[LINK_DL_RFIN_BOOMERANG];

    sShellDLists[PLAYER_FORM_HUMAN] = &proxy->displayLists[LINK_DL_ELEGY_OF_EMPTINESS_SHELL_HUMAN];
    sShellDLists[PLAYER_FORM_DEKU] = &proxy->displayLists[LINK_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU];
    sShellDLists[PLAYER_FORM_GORON] = &proxy->displayLists[LINK_DL_ELEGY_OF_EMPTINESS_SHELL_GORON];
    sShellDLists[PLAYER_FORM_ZORA] = &proxy->displayLists[LINK_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA];
    sShellDLists[PLAYER_FORM_FIERCE_DEITY] = &proxy->displayLists[LINK_DL_ELEGY_OF_EMPTINESS_SHELL_FIERCE_DEITY];

#define SET_MASK_DL(playerMaskIndex, linkDLIndex) (sPlayerMaskDLs[playerMaskIndex - 1] = &proxy->displayLists[linkDLIndex])

    SET_MASK_DL(PLAYER_MASK_TRUTH, LINK_DL_MASK_TRUTH);
    SET_MASK_DL(PLAYER_MASK_KAFEIS_MASK, LINK_DL_MASK_KAFEIS_MASK);
    SET_MASK_DL(PLAYER_MASK_ALL_NIGHT, LINK_DL_MASK_ALL_NIGHT);
    SET_MASK_DL(PLAYER_MASK_BUNNY, LINK_DL_MASK_BUNNY);
    SET_MASK_DL(PLAYER_MASK_KEATON, LINK_DL_MASK_KEATON);
    SET_MASK_DL(PLAYER_MASK_GARO, LINK_DL_MASK_GARO);
    SET_MASK_DL(PLAYER_MASK_ROMANI, LINK_DL_MASK_ROMANI);
    SET_MASK_DL(PLAYER_MASK_CIRCUS_LEADER, LINK_DL_MASK_CIRCUS_LEADER);
    SET_MASK_DL(PLAYER_MASK_POSTMAN, LINK_DL_MASK_POSTMAN);
    SET_MASK_DL(PLAYER_MASK_COUPLE, LINK_DL_MASK_COUPLE);
    SET_MASK_DL(PLAYER_MASK_GREAT_FAIRY, LINK_DL_MASK_GREAT_FAIRY);
    SET_MASK_DL(PLAYER_MASK_GIBDO, LINK_DL_MASK_GIBDO);
    SET_MASK_DL(PLAYER_MASK_DON_GERO, LINK_DL_MASK_DON_GERO);
    SET_MASK_DL(PLAYER_MASK_KAMARO, LINK_DL_MASK_KAMARO);
    SET_MASK_DL(PLAYER_MASK_CAPTAIN, LINK_DL_MASK_CAPTAIN);
    SET_MASK_DL(PLAYER_MASK_STONE, LINK_DL_MASK_STONE);
    SET_MASK_DL(PLAYER_MASK_BREMEN, LINK_DL_MASK_BREMEN);
    SET_MASK_DL(PLAYER_MASK_BLAST, LINK_DL_MASK_BLAST);
    SET_MASK_DL(PLAYER_MASK_SCENTS, LINK_DL_MASK_SCENTS);
    SET_MASK_DL(PLAYER_MASK_GIANT, LINK_DL_MASK_GIANT);
    SET_MASK_DL(PLAYER_MASK_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY);
    SET_MASK_DL(PLAYER_MASK_GORON, LINK_DL_MASK_GORON);
    SET_MASK_DL(PLAYER_MASK_ZORA, LINK_DL_MASK_ZORA);
    SET_MASK_DL(PLAYER_MASK_DEKU, LINK_DL_MASK_DEKU);
    SET_MASK_DL(PLAYER_MASK_DEKU + 1, LINK_DL_MASK_FIERCE_DEITY_SCREAM);
    SET_MASK_DL(PLAYER_MASK_DEKU + 2, LINK_DL_MASK_GORON_SCREAM);
    SET_MASK_DL(PLAYER_MASK_DEKU + 3, LINK_DL_MASK_ZORA_SCREAM);
    SET_MASK_DL(PLAYER_MASK_DEKU + 4, LINK_DL_MASK_DEKU_SCREAM);

#undef SET_MASK_DL
}

RECOMP_DECLARE_EVENT(_internal_setupVanillaModels());

// initialize player models as blank display lists
void initFormProxies() {
    _internal_setupVanillaModels();

    gPlayer1Proxy = PlayerProxyManager_createPlayerProxy();
}

RECOMP_HOOK("Player_Draw")
void updateAssets_on_Player_Draw(Actor *thisx, PlayState *play) {
    FormProxy *fp = ProxyActorExt_getFormProxy(thisx);

    if (fp) {
        Player *player = (Player *)thisx;
        FormProxy_refreshPlayerFaceTextures(fp);
        repointSharedModelsToProxy(fp);
        repointFormPtrsToProxy(fp, player->transformation);
    }
}

void doInitFormProxies() {
    static bool isFormProxiesInitialized;

    if (!isFormProxiesInitialized) {
        isFormProxiesInitialized = true;
        initFormProxies();
        PlayerProxyManager_refreshAll();
    }
}

RECOMP_CALLBACK(".", _internal_onReadyModelReplacerCompat)
void initFormProxies_on_mrc() {
    doInitFormProxies();
}

GLOBAL_OBJECTS_CALLBACK_ON_READY
void initFormProxies_on_go() {
    if (!MRC_isMRCEnabled()) {
        doInitFormProxies();
    }
}

RECOMP_CALLBACK(".", _internal_onFormModelApplied)
void refreshSharedModelsOnFormModelApply(Link_CustomModelCategory form) {
    sShouldSkipFormInterpolation[form] = true;
}

RECOMP_CALLBACK(".", _internal_onEquipmentModelApplied)
void refreshEquipmentOnEquipmentModelApplied(Link_EquipmentReplacement eq) {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        sShouldSkipFormInterpolation[i] = true;
    }

    if (eq == LINK_DL_REPLACE_SHIELD3) {
        sShouldSkipMirrorShieldInterpolation = true;
    }

    if (eq == LINK_DL_REPLACE_HOOKSHOT) {
        sShouldSkipHookshotInterpolation = true;
    }
}

RECOMP_DECLARE_EVENT(_internal_preInitHashObjects());
RECOMP_DECLARE_EVENT(_internal_initHashObjects());
RECOMP_DECLARE_EVENT(_internal_postInitHashObjects());

RECOMP_CALLBACK("*", recomp_on_init)
void handleInits() {
    _internal_preInitHashObjects();
    _internal_initHashObjects();
    _internal_postInitHashObjects();
}

RECOMP_CALLBACK("*", recomp_on_play_main)
void skipInterpolationOnPlay(PlayState *play) {
    Player *player = GET_PLAYER(play);

    while (player) {
        if (sShouldSkipFormInterpolation[player->transformation] && player->actor.scale.y >= 0.0f) {
            actor_set_interpolation_skipped(&player->actor);
        }

        player = (Player *)player->actor.next;
    }

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        sShouldSkipFormInterpolation[i] = false;
    }

    if (sShouldSkipMirrorShieldInterpolation || sShouldSkipHookshotInterpolation) {
        Actor *actor = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].first;

        while (actor) {
            if ((sShouldSkipMirrorShieldInterpolation && actor->id == ACTOR_MIR_RAY3) ||
                (sShouldSkipHookshotInterpolation && actor->id == ACTOR_ARMS_HOOK)) {
                actor_set_interpolation_skipped(actor);
            }

            actor = actor->next;
        }

        sShouldSkipMirrorShieldInterpolation = false;
        sShouldSkipHookshotInterpolation = false;
    }
}
