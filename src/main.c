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
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "externaldisplaylists.h"
#include "externs_z_player_lib.h"

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());

Link_FormProxy gLinkFormProxies[PLAYER_FORM_MAX];

typedef enum {
    SHARED_DL_LFIST_SWORD_KOKIRI,
    SHARED_DL_LFIST_SWORD_RAZOR,
    SHARED_DL_LFIST_SWORD_GILDED,
    SHARED_DL_SHIELD_HERO_BACK,
    SHARED_DL_SHIELD_MIRROR_BACK,
    SHARED_DL_RFIST_SHIELD_HERO,
    SHARED_DL_RFIST_SHIELD_MIRROR,
    SHARED_DL_SWORD_KOKIRI_SHEATHED,
    SHARED_DL_SWORD_RAZOR_SHEATHED,
    SHARED_DL_SWORD_GILDED_SHEATHED,
    SHARED_DL_SWORD_KOKIRI_SHEATH,
    SHARED_DL_SWORD_RAZOR_SHEATH,
    SHARED_DL_SWORD_GILDED_SHEATH,
    SHARED_DL_MAX
} SharedDisplayList;

static Gfx sSharedDLs[SHARED_DL_MAX];

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
    sPlayerFirstPersonRightHandDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_RHAND_BOW];
    sPlayerFirstPersonRightHandHookshotDLs[playerForm] = &formProxy->displayLists[LINK_DL_FPS_RHAND_HOOKSHOT];

    gPlayerWaistDLs[playerForm * 2 + 0] = &formProxy->displayLists[LINK_DL_WAIST];
    gPlayerWaistDLs[playerForm * 2 + 1] = &formProxy->displayLists[LINK_DL_WAIST];
}

static Gfx **sPlayerHandHoldingSwords = D_801C018C;

static Gfx **sPlayerMasks = D_801C0B20;

void repointHumanModelsToProxy() {
    sPlayerHandHoldingSwords[0] = &sSharedDLs[SHARED_DL_LFIST_SWORD_KOKIRI];
    sPlayerHandHoldingSwords[1] = &sSharedDLs[SHARED_DL_LFIST_SWORD_KOKIRI];
    sPlayerHandHoldingSwords[2] = &sSharedDLs[SHARED_DL_LFIST_SWORD_RAZOR];
    sPlayerHandHoldingSwords[3] = &sSharedDLs[SHARED_DL_LFIST_SWORD_RAZOR];
    sPlayerHandHoldingSwords[4] = &sSharedDLs[SHARED_DL_LFIST_SWORD_GILDED];
    sPlayerHandHoldingSwords[5] = &sSharedDLs[SHARED_DL_LFIST_SWORD_GILDED];

    gPlayerShields[0] = &sSharedDLs[SHARED_DL_SHIELD_HERO_BACK];
    gPlayerShields[1] = &sSharedDLs[SHARED_DL_SHIELD_HERO_BACK];
    gPlayerShields[2] = &sSharedDLs[SHARED_DL_SHIELD_MIRROR_BACK];
    gPlayerShields[3] = &sSharedDLs[SHARED_DL_SHIELD_MIRROR_BACK];

    gPlayerHandHoldingShields[0] = &sSharedDLs[SHARED_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[1] = &sSharedDLs[SHARED_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[2] = &sSharedDLs[SHARED_DL_RFIST_SHIELD_MIRROR];
    gPlayerHandHoldingShields[3] = &sSharedDLs[SHARED_DL_RFIST_SHIELD_MIRROR];

    gPlayerSheathedSwords[0] = &sSharedDLs[SHARED_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[1] = &sSharedDLs[SHARED_DL_SWORD_KOKIRI_SHEATHED];
    gPlayerSheathedSwords[2] = &sSharedDLs[SHARED_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[3] = &sSharedDLs[SHARED_DL_SWORD_RAZOR_SHEATHED];
    gPlayerSheathedSwords[4] = &sSharedDLs[SHARED_DL_SWORD_GILDED_SHEATHED];
    gPlayerSheathedSwords[5] = &sSharedDLs[SHARED_DL_SWORD_GILDED_SHEATHED];

    gPlayerSwordSheaths[0] = &sSharedDLs[SHARED_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[1] = &sSharedDLs[SHARED_DL_SWORD_KOKIRI_SHEATH];
    gPlayerSwordSheaths[2] = &sSharedDLs[SHARED_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[3] = &sSharedDLs[SHARED_DL_SWORD_RAZOR_SHEATH];
    gPlayerSwordSheaths[4] = &sSharedDLs[SHARED_DL_SWORD_GILDED_SHEATH];
    gPlayerSwordSheaths[5] = &sSharedDLs[SHARED_DL_SWORD_GILDED_SHEATH];
}

void refreshSharedDL(Link_FormProxy *current, Link_FormProxy *fallback, Link_DisplayList test, Link_DisplayList target, SharedDisplayList shared) {
    Gfx *dl = getFormProxyDL(current, test) ? &current->displayLists[target] : &fallback->displayLists[target];
    gSPBranchList(&sSharedDLs[shared], dl);
}

void refreshHumanModels() {
    Link_FormProxy *current = &GET_PLAYER_FORM_PROXY;
    Link_FormProxy *human = &gLinkFormProxies[PLAYER_FORM_HUMAN];

    gSPBranchList(&sSharedDLs[SHARED_DL_LFIST_SWORD_KOKIRI], &current->displayLists[LINK_DL_LFIST_SWORD_KOKIRI]);
    gSPBranchList(&sSharedDLs[SHARED_DL_LFIST_SWORD_RAZOR], &current->displayLists[LINK_DL_LFIST_SWORD_RAZOR]);
    gSPBranchList(&sSharedDLs[SHARED_DL_LFIST_SWORD_GILDED], &current->displayLists[LINK_DL_LFIST_SWORD_GILDED]);

    refreshSharedDL(current, human, LINK_DL_SHIELD_HERO, LINK_DL_SHIELD_HERO_BACK, SHARED_DL_SHIELD_HERO_BACK);
    refreshSharedDL(current, human, LINK_DL_SHIELD_MIRROR, LINK_DL_SHIELD_MIRROR_BACK, SHARED_DL_SHIELD_MIRROR_BACK);

    refreshSharedDL(current, human, LINK_DL_SHIELD_HERO, LINK_DL_RFIST_SHIELD_HERO, SHARED_DL_RFIST_SHIELD_HERO);
    refreshSharedDL(current, human, LINK_DL_SHIELD_MIRROR, LINK_DL_RFIST_SHIELD_MIRROR, SHARED_DL_RFIST_SHIELD_MIRROR);

    refreshSharedDL(current, human, LINK_DL_SWORD_KOKIRI_SHEATHED, LINK_DL_SWORD_KOKIRI_SHEATHED, SHARED_DL_SWORD_KOKIRI_SHEATHED);
    refreshSharedDL(current, human, LINK_DL_SWORD_RAZOR_SHEATHED, LINK_DL_SWORD_RAZOR_SHEATHED, SHARED_DL_SWORD_RAZOR_SHEATHED);
    refreshSharedDL(current, human, LINK_DL_SWORD_GILDED_SHEATHED, LINK_DL_SWORD_GILDED_SHEATHED, SHARED_DL_SWORD_GILDED_SHEATHED);

    refreshSharedDL(current, human, LINK_DL_SWORD_KOKIRI_SHEATH, LINK_DL_SWORD_KOKIRI_SHEATH, SHARED_DL_SWORD_KOKIRI_SHEATH);
    refreshSharedDL(current, human, LINK_DL_SWORD_RAZOR_SHEATH, LINK_DL_SWORD_RAZOR_SHEATH, SHARED_DL_SWORD_RAZOR_SHEATH);
    refreshSharedDL(current, human, LINK_DL_SWORD_GILDED_SHEATH, LINK_DL_SWORD_GILDED_SHEATH, SHARED_DL_SWORD_GILDED_SHEATH);
}

void initSharedDLs() {
    for (int i = 0; i < SHARED_DL_MAX; ++i) {
        gSPBranchList(&sSharedDLs[i], gEmptyDL);
    }
}

void addProxyExternalDLs() {
    Link_FormProxy *humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

    ModelReplacer_setReplacerModel(gProxyBottleGlass, &humanProxy->displayLists[LINK_DL_BOTTLE_GLASS]);
    ModelReplacer_setReplacerModel(gProxyBottleContents, &humanProxy->displayLists[LINK_DL_BOTTLE_CONTENTS]);
    ModelReplacer_setReplacerModel(gProxyDekuStick, &humanProxy->displayLists[LINK_DL_DEKU_STICK]);
    ModelReplacer_setReplacerModel(gProxyBowString, &humanProxy->displayLists[LINK_DL_BOW_STRING]);
    ModelReplacer_setReplacerModel(gProxyBowArrow, &humanProxy->displayLists[LINK_DL_BOW_ARROW]);
    ModelReplacer_setReplacerModel(gProxyBowArrowLowPoly, &humanProxy->displayLists[LINK_DL_BOW_ARROW]);
    ModelReplacer_setReplacerModel(gProxyHookshotChain, &humanProxy->displayLists[LINK_DL_HOOKSHOT_CHAIN]);
    ModelReplacer_setReplacerModel(gProxyHookshotHook, &humanProxy->displayLists[LINK_DL_HOOKSHOT_HOOK]);
    ModelReplacer_setReplacerModel(gProxyHookshotReticle, &humanProxy->displayLists[LINK_DL_HOOKSHOT_RETICLE]);
    ModelReplacer_setReplacerModel(gProxyMirrorShieldRay, &humanProxy->displayLists[LINK_DL_SHIELD3_RAY]);
}

void removeProxyExternalDLs() {
    ModelReplacer_setReplacerModel(gProxyBottleGlass, NULL);
    ModelReplacer_setReplacerModel(gProxyBottleContents, NULL);
    ModelReplacer_setReplacerModel(gProxyDekuStick, NULL);
    ModelReplacer_setReplacerModel(gProxyBowString, NULL);
    ModelReplacer_setReplacerModel(gProxyBowArrow, NULL);
    ModelReplacer_setReplacerModel(gProxyBowArrowLowPoly, NULL);
    ModelReplacer_setReplacerModel(gProxyHookshotChain, NULL);
    ModelReplacer_setReplacerModel(gProxyHookshotHook, NULL);
    ModelReplacer_setReplacerModel(gProxyHookshotReticle, NULL);
    ModelReplacer_setReplacerModel(gProxyMirrorShieldRay, NULL);
}

void refreshExternalDLs() {
    if (GET_PLAYER_FORM == PLAYER_FORM_HUMAN) {
        addProxyExternalDLs();
    } else {
        removeProxyExternalDLs();
    }
}

// initialize player models as blank display lists
void initFormProxies() {
    for (u8 i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_FormProxy *formProxy = &gLinkFormProxies[i];

        clearLinkModelInfo(&formProxy->current);

        initFormProxy(formProxy);

        // vanilla forms share these segmented ptrs
        for (u32 j = 0; j < PLAYER_EYES_MAX; ++j) {
            formProxy->vanilla.eyesTextures[j] = sPlayerEyesTextures[j];
        }

        for (u32 j = 0; j < PLAYER_MOUTH_MAX; ++j) {
            formProxy->vanilla.mouthTextures[j] = sPlayerMouthTextures[j];
        }

        // uncomment these when all forms supported
        // requestRefreshFormProxy(i);
        // changeFormPtrsToProxy(i);
    }

    // Only human is supported for now
    setupVanillaHuman();
    requestRefreshFormProxy(PLAYER_FORM_HUMAN);
    changeFormPtrsToProxy(PLAYER_FORM_HUMAN);
    repointHumanModelsToProxy();

    refreshFaceTextures();
}

RECOMP_HOOK_RETURN("Player_Init")
void refreshDLs_on_return_PlayerInit() {
    refreshFaceTextures();

    refreshExternalDLs();

    refreshHumanModels();

    gIsAgePropertyRefreshRequested = true;
}

RECOMP_DECLARE_EVENT(_internal_onReadyFormProxies());

MODEL_REPLACER_CALLBACK_ON_READY
void onModelReplacerReady() {
    initFormProxies();
    _internal_onReadyFormProxies();
}

RECOMP_CALLBACK(".", _internal_onModelApplied)
void refreshSharedModelsOnModelApply(PlayerTransformation form) {
    requestRefreshFormProxy(form);
    gIsAgePropertyRefreshRequested = true;

    if (form == GET_PLAYER_FORM) {
        refreshHumanModels();
        refreshFaceTextures();
    }
}
