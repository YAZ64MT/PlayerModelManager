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

extern Gfx *gPlayerRightHandBowDLs[2 * PLAYER_FORM_MAX];

extern Gfx *D_801C018C[];

extern Gfx *gPlayerSheathedSwords[];
extern Gfx *gPlayerSwordSheaths[];
extern Gfx *gPlayerShields[];
extern Gfx *gPlayerHandHoldingShields[];

extern TexturePtr sPlayerEyesTextures[];
extern TexturePtr sPlayerMouthTextures[];

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

void repointHumanEquipmentModelsToProxy(PlayerTransformation playerForm) {
    Link_FormProxy *formProxy = &gLinkFormProxies[playerForm];

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

    gPlayerHandHoldingShields[0] = &formProxy->displayLists[LINK_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[1] = &formProxy->displayLists[LINK_DL_RFIST_SHIELD_HERO];
    gPlayerHandHoldingShields[2] = &formProxy->displayLists[LINK_DL_RFIST_SHIELD_MIRROR];
    gPlayerHandHoldingShields[3] = &formProxy->displayLists[LINK_DL_RFIST_SHIELD_MIRROR];

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

void addProxyExternalDLs() {
    Link_FormProxy *humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

    ZModelReplacer_addCustomDL(GAMEPLAY_KEEP, gBottleGlassDL, &humanProxy->displayLists[LINK_DL_BOTTLE_GLASS]);

    ZModelReplacer_addCustomDL(GAMEPLAY_KEEP, gBottleContentsDL, &humanProxy->displayLists[LINK_DL_BOTTLE_FILLING]);

    ZModelReplacer_addCustomDL(GAMEPLAY_KEEP, gDekuStickDL, &humanProxy->displayLists[LINK_DL_DEKU_STICK]);
}

void removeProxyEternalDLs() {
    Link_FormProxy *humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

    ZModelReplacer_removeCustomDL(GAMEPLAY_KEEP, gBottleGlassDL, &humanProxy->displayLists[LINK_DL_BOTTLE_GLASS]);

    ZModelReplacer_removeCustomDL(GAMEPLAY_KEEP, gBottleContentsDL, &humanProxy->displayLists[LINK_DL_BOTTLE_FILLING]);

    ZModelReplacer_removeCustomDL(GAMEPLAY_KEEP, gDekuStickDL, &humanProxy->displayLists[LINK_DL_DEKU_STICK]);
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
        // refreshFormProxy(formProxy);
        // changeFormPtrsToProxy(i);
    }

    // Only human is supported for now
    setupVanillaHuman();
    refreshFormProxy(&gLinkFormProxies[PLAYER_FORM_HUMAN]);
    changeFormPtrsToProxy(PLAYER_FORM_HUMAN);
    repointHumanEquipmentModelsToProxy(PLAYER_FORM_HUMAN);

    matchFaceTexturesToProxy(&GET_PLAYER_FORM_PROXY);
}

RECOMP_HOOK_RETURN("Player_Init")
void postPlayerInit() {
    matchFaceTexturesToProxy(&GET_PLAYER_FORM_PROXY);

    if (GET_PLAYER_FORM == PLAYER_FORM_HUMAN) {
        addProxyExternalDLs();
    }
    else {
        removeProxyEternalDLs();
    }

    gIsAgePropertyRefreshRequested = true;
}

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onReadyFormProxies());

RECOMP_CALLBACK(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, ZModelReplacer_onReady)
void onModelReplacerReady() {
    initFormProxies();
    PlayerModelManager_internal_onReadyFormProxies();
}
