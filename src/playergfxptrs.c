#include "global.h"
#include "formproxy.h"
#include "proxyactorext.h"
#include "playerexterns.h"

static void tryReplaceCodeDL(FormProxy *fp, Link_DisplayList dlId, Gfx **dest) {
    Gfx *newDL = FormProxy_getDL(fp, dlId);
    if (newDL) {
        *dest = newDL;
    }
}

static void tryReplaceCodeLodDL(FormProxy *fp, Link_DisplayList dlId, Gfx *dest[]) {
    Gfx *newDL = FormProxy_getDL(fp, dlId);
    if (newDL) {
        dest[0] = newDL;
        dest[1] = newDL;
    }
}

void repointFormPtrsToProxy(Player *player, FormProxy *formProxy) {
    PlayerTransformation playerForm = player->transformation;

    Link_DisplayList rightFistDLId = LINK_DL_OPT_RFIST;
    Link_DisplayList leftFistDLId = LINK_DL_OPT_LFIST;

    if (player->itemAction == PLAYER_IA_DEKU_STICK) {
        rightFistDLId = LINK_DL_OPT_RFIST_DEKU_STICK;
        leftFistDLId = LINK_DL_OPT_LFIST_DEKU_STICK;
    } else if (player->itemAction == PLAYER_IA_SWORD_TWO_HANDED) {
        if (playerForm == PLAYER_FORM_FIERCE_DEITY) {
            rightFistDLId = LINK_DL_OPT_RFIST_SWORD4;
            leftFistDLId = LINK_DL_OPT_LFIST_SWORD4;
        } else {
            rightFistDLId = LINK_DL_OPT_RFIST_SWORD5;
            leftFistDLId = LINK_DL_OPT_LFIST_SWORD5;
        }
    }

    tryReplaceCodeLodDL(formProxy, LINK_DL_RHAND, &gPlayerRightHandOpenDLs[playerForm * 2]);

    tryReplaceCodeLodDL(formProxy, rightFistDLId, &gPlayerRightHandClosedDLs[playerForm * 2]);

    Link_DisplayList rightHandHoldingInstrumentDLId;
    if (playerForm == PLAYER_FORM_HUMAN || playerForm == PLAYER_FORM_FIERCE_DEITY) {
        rightHandHoldingInstrumentDLId = LINK_DL_RHAND_OCARINA_TIME;
    } else {
        rightHandHoldingInstrumentDLId = LINK_DL_OPT_RHAND_OCARINA;
    }

    tryReplaceCodeLodDL(formProxy, rightHandHoldingInstrumentDLId, &gPlayerRightHandInstrumentDLs[playerForm * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_RFIST_HOOKSHOT, &gPlayerRightHandHookshotDLs[playerForm * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_LHAND, &gPlayerLeftHandOpenDLs[playerForm * 2]);

    tryReplaceCodeLodDL(formProxy, leftFistDLId, &gPlayerLeftHandClosedDLs[playerForm * 2]);

    Link_DisplayList leftHandHoldingTwoHandedSwordDLId;
    if (playerForm == PLAYER_FORM_FIERCE_DEITY) {
        leftHandHoldingTwoHandedSwordDLId = LINK_DL_LFIST_SWORD_FIERCE_DEITY;
    } else {
        leftHandHoldingTwoHandedSwordDLId = LINK_DL_LFIST_SWORD_GREAT_FAIRY;
    }

    tryReplaceCodeLodDL(formProxy, leftHandHoldingTwoHandedSwordDLId, &gPlayerLeftHandTwoHandSwordDLs[playerForm * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_LFIST_SWORD_KOKIRI, &gPlayerLeftHandOneHandSwordDLs[playerForm * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_OPT_LHAND_BOTTLE, &gPlayerLeftHandBottleDLs[playerForm * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_RFIST_BOW, &gPlayerRightHandBowDLs[playerForm * 2]);

    Link_DisplayList firstPersonLeftForearmDlId;
    Link_DisplayList firstPersonLeftHandDlId;
    Link_DisplayList firstPersonRightShoulderDlId;
    if (Player_IsHoldingHookshot(player)) {
        firstPersonLeftForearmDlId = LINK_DL_OPT_FPS_LFOREARM_HOOKSHOT;
        firstPersonLeftHandDlId = LINK_DL_OPT_FPS_LHAND_HOOKSHOT;
        firstPersonRightShoulderDlId = LINK_DL_OPT_FPS_RSHOULDER_HOOKSHOT;
    } else {
        firstPersonLeftForearmDlId = LINK_DL_OPT_FPS_LFOREARM_BOW;
        firstPersonLeftHandDlId = LINK_DL_OPT_FPS_LHAND_BOW;
        firstPersonRightShoulderDlId = LINK_DL_OPT_FPS_RSHOULDER_BOW;
    }

    tryReplaceCodeDL(formProxy, firstPersonLeftForearmDlId, &sPlayerFirstPersonLeftForearmDLs[playerForm]);

    tryReplaceCodeDL(formProxy, firstPersonLeftHandDlId, &sPlayerFirstPersonLeftHandDLs[playerForm]);

    tryReplaceCodeDL(formProxy, firstPersonRightShoulderDlId, &sPlayerFirstPersonRightShoulderDLs[playerForm]);

    tryReplaceCodeDL(formProxy, LINK_DL_FPS_RHAND_BOW, &sPlayerFirstPersonRightHandDLs[playerForm]);

    tryReplaceCodeDL(formProxy, LINK_DL_FPS_RHAND_HOOKSHOT, &sPlayerFirstPersonRightHandHookshotDLs[playerForm]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_WAIST, &gPlayerWaistDLs[playerForm * 2]);
}

static Gfx **sPlayerHandHoldingSwords = D_801C018C;

static Gfx **sPlayerMasks = D_801C0B20;

static Gfx **sPlayerFins = D_801C0AB4;

static Gfx **sPlayerSwimFins = D_801C0ABC;

static Gfx **sPlayerPads = D_8085D574;

static Gfx **sPlayerMaskDLs = D_801C0B20;

static void repointSharedModelsToProxy(FormProxy *formProxy) {
    tryReplaceCodeLodDL(formProxy, LINK_DL_LFIST_SWORD_KOKIRI, &sPlayerHandHoldingSwords[PLAYER_SWORD_KOKIRI * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_LFIST_SWORD_RAZOR, &sPlayerHandHoldingSwords[PLAYER_SWORD_RAZOR * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_LFIST_SWORD_GILDED, &sPlayerHandHoldingSwords[PLAYER_SWORD_GILDED * 2]);

    int shieldHeroIndex = PLAYER_SHIELD_HEROS_SHIELD - 1;
    int shieldMirrorIndex = PLAYER_SHIELD_MIRROR_SHIELD - 1;

    tryReplaceCodeLodDL(formProxy, LINK_DL_SHIELD_HERO_BACK, &gPlayerShields[shieldHeroIndex * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_SHIELD_MIRROR_BACK, &gPlayerShields[shieldMirrorIndex * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_RFIST_SHIELD_HERO, &gPlayerHandHoldingShields[shieldHeroIndex * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_RFIST_SHIELD_MIRROR, &gPlayerHandHoldingShields[shieldMirrorIndex * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_SWORD_KOKIRI_SHEATHED, &gPlayerSheathedSwords[PLAYER_SWORD_KOKIRI * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_SWORD_RAZOR_SHEATHED, &gPlayerSheathedSwords[PLAYER_SWORD_RAZOR * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_SWORD_GILDED_SHEATHED, &gPlayerSheathedSwords[PLAYER_SWORD_GILDED * 2]);

    tryReplaceCodeLodDL(formProxy, LINK_DL_SWORD_KOKIRI_SHEATH, &gPlayerSwordSheaths[PLAYER_SWORD_KOKIRI * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_SWORD_RAZOR_SHEATH, &gPlayerSwordSheaths[PLAYER_SWORD_RAZOR * 2]);
    tryReplaceCodeLodDL(formProxy, LINK_DL_SWORD_GILDED_SHEATH, &gPlayerSwordSheaths[PLAYER_SWORD_GILDED * 2]);

    tryReplaceCodeDL(formProxy, LINK_DL_LFIN, &sPlayerFins[0]);
    tryReplaceCodeDL(formProxy, LINK_DL_RFIN, &sPlayerFins[1]);

    tryReplaceCodeDL(formProxy, LINK_DL_LFIN_SWIM, &sPlayerSwimFins[0]);
    tryReplaceCodeDL(formProxy, LINK_DL_RFIN_SWIM, &sPlayerSwimFins[1]);

    tryReplaceCodeDL(formProxy, LINK_DL_PAD_GRASS, &sPlayerPads[0]);
    tryReplaceCodeDL(formProxy, LINK_DL_PAD_WOOD, &sPlayerPads[1]);
    tryReplaceCodeDL(formProxy, LINK_DL_PAD_OPENING, &sPlayerPads[2]);

#define SET_MASK_DL(playerMaskIndex, linkDLIndex) tryReplaceCodeDL(formProxy, linkDLIndex, &sPlayerMaskDLs[playerMaskIndex - 1])

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

void updateAssets_on_Player_Draw(Player *player) {
    FormProxy *fp = ProxyActorExt_getFormProxy(&player->actor);

    if (fp) {
        FormProxy_repointPlayerFaceTexturePtrs(fp);
        repointSharedModelsToProxy(fp);
        repointFormPtrsToProxy(player, fp);
        FlexSkeletonHeader *skel = FormProxy_getSkeleton(fp);
        if (skel) {
            player->skelAnime.dListCount = player->skelAnimeUpper.dListCount = skel->dListCount;
            player->skelAnime.skeleton = player->skelAnimeUpper.skeleton = skel->sh.segment;
        }

        if (player->transformation == PLAYER_FORM_GORON) {
            FlexSkeletonHeader *shieldingSkel = FormProxy_getShieldingSkeleton(fp);

            if (shieldingSkel) {
                player->unk_2C8.dListCount = shieldingSkel->dListCount;
                player->unk_2C8.skeleton = shieldingSkel->sh.segment;
            }
        }
    }
}
