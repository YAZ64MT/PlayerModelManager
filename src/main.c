#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_mm.h"
#include "model_common.h"
#include "model_human.h"
#include "defines_modelinfo.h"
#include "playermodelmanager_utils.h"
#include "modelreplacer_api.h"
#include "modelreplacer_compat.h"
#include "globalobjects_api.h"
#include "externs_z_player_lib.h"
#include "model_shared.h"

U32ValueHashmapHandle gLinkEquipmentGfxOverrides;
U32ValueHashmapHandle gLinkEquipmentMtxOverrides;

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

static Gfx **sPlayerMaskDLs = D_801C0B20;

void repointSharedModelsToProxy(Link_FormProxy *proxy) {
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
    if (!gLinkEquipmentGfxOverrides) {
        gLinkEquipmentGfxOverrides = recomputil_create_u32_value_hashmap();
    }

    if (!gLinkEquipmentMtxOverrides) {
        gLinkEquipmentMtxOverrides = recomputil_create_u32_value_hashmap();
    }

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_FormProxy *formProxy = &gLinkFormProxies[i];

        formProxy->gfxOverrides = gLinkEquipmentGfxOverrides;

        formProxy->mtxOverrides = gLinkEquipmentMtxOverrides;

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
}

bool sPlayerAppearanceNeedsUpdate = false;

RECOMP_HOOK("Player_Init")
void refreshDLs_on_PlayerInit(Actor *thisx, PlayState *play) {
    Player *player = (Player *)thisx;

    if (thisx->id == ACTOR_PLAYER) {
        sPlayerAppearanceNeedsUpdate = true;
    }
}

extern Gfx *D_801C0D94; // Bow string DL ptr

RECOMP_HOOK("Player_Draw")
void fixFaceTextures_on_Player_Draw(Actor *thisx, PlayState *play) {

    // Multiple Link actors appear in Milk Bar sound check cutscene
    bool isMultipleLinksExist = GET_PLAYER(play)->actor.next;

    if (isMultipleLinksExist || sPlayerAppearanceNeedsUpdate) {
        sPlayerAppearanceNeedsUpdate = isMultipleLinksExist;

        Link_FormProxy *fp = &gLinkFormProxies[((Player *)thisx)->transformation];

        matchFaceTexturesToProxy(fp);

        repointSharedModelsToProxy(fp);
    }
}

RECOMP_DECLARE_EVENT(_internal_onReadyFormProxies());

static bool sIsFormProxiesInitialized = false;

void doInitFormProxies() {
    if (!sIsFormProxiesInitialized) {
        sIsFormProxiesInitialized = true;
        initFormProxies();
        _internal_onReadyFormProxies();
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

RECOMP_CALLBACK(".", _internal_onModelApplied)
void refreshSharedModelsOnModelApply(PlayerTransformation form) {
    requestRefreshFormProxy(&gLinkFormProxies[form]);

    if (form == GET_PLAYER_FORM) {
        sPlayerAppearanceNeedsUpdate = true;
    }
}

RECOMP_DECLARE_EVENT(_internal_initHashObjects());

RECOMP_CALLBACK("*", recomp_on_init)
void handleInits() {
    _internal_initHashObjects();
}
