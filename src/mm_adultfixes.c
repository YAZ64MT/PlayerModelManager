#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "model_common.h"
#include "mm_adultfixes.h"
#include "defines_modelinfo.h"
#include "overlays/actors/ovl_En_Rd/z_en_rd.h"
#include "overlays/actors/ovl_En_Railgibud/z_en_railgibud.h"
#include "overlays/actors/ovl_En_Talk_Gibud/z_en_talk_gibud.h"
#include "overlays/actors/ovl_Arms_Hook/z_arms_hook.h"

PlayState *gPlayState;
s32 gLimbIndex;
Gfx **gFirstPersonDList;

RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson")
void addFPSrightForearm_on_OverrideLimbDrawFirstPerson(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    gPlayState = play;
    gLimbIndex = limbIndex;
    gFirstPersonDList = dList;
}

RECOMP_HOOK_RETURN("Player_OverrideLimbDrawGameplayFirstPerson")
void addFPSrightForearm_on_return_OverrideLimbDrawFirstPerson() {
    if (gLimbIndex == PLAYER_LIMB_RIGHT_FOREARM) {
        Link_FormProxy *p = GET_PLAYER_FORM_PROXY;
        *gFirstPersonDList = &p->displayLists[LINK_DL_FPS_RFOREARM];
    }
}

PlayerAgeProperties gAdultLinkAgeProps = {};

PlayerAgeProperties gVanillaHumanLinkAgeProps;

extern PlayerAgeProperties sPlayerAgeProperties[];

void initAdultLinkAgeProperties() {
    PlayerAgeProperties *fdProps = &sPlayerAgeProperties[PLAYER_FORM_FIERCE_DEITY];
    gAdultLinkAgeProps = sPlayerAgeProperties[PLAYER_FORM_ZORA];

    gAdultLinkAgeProps.unk_28 = 44.6f;
    gAdultLinkAgeProps.unk_3C = 15.0f;
    gAdultLinkAgeProps.unk_44 = fdProps->unk_44;
    gAdultLinkAgeProps.unk_4A->x = fdProps->unk_4A->x;
    gAdultLinkAgeProps.unk_4A->y = fdProps->unk_4A->y;
    gAdultLinkAgeProps.unk_4A->z = fdProps->unk_4A->z;
    gAdultLinkAgeProps.unk_62->x = fdProps->unk_62->x;
    gAdultLinkAgeProps.unk_62->y = fdProps->unk_62->y;
    gAdultLinkAgeProps.unk_62->z = fdProps->unk_62->z;
    gAdultLinkAgeProps.unk_7A->x = fdProps->unk_7A->x;
    gAdultLinkAgeProps.unk_7A->y = fdProps->unk_7A->y;
    gAdultLinkAgeProps.unk_7A->z = fdProps->unk_7A->z;
    gAdultLinkAgeProps.voiceSfxIdOffset = SFX_VOICE_BANK_SIZE * 0;
    gAdultLinkAgeProps.surfaceSfxIdOffset = 0x80;
}

bool isAdultAgePropsInitialized() {
    return gAdultLinkAgeProps.surfaceSfxIdOffset == 0x80;
}

RECOMP_HOOK("Player_Init")
void initAgeProps(Actor *thisx, PlayState *play) {
    if (!isAdultAgePropsInitialized()) {
        gVanillaHumanLinkAgeProps = sPlayerAgeProperties[PLAYER_FORM_HUMAN];
        initAdultLinkAgeProperties();
    }
}

extern PlayerAnimationHeader *D_8085BE84[PLAYER_ANIMGROUP_MAX][PLAYER_ANIMTYPE_MAX];
extern LinkAnimationHeader gPlayerAnim_clink_demo_doorA_link;
extern LinkAnimationHeader gPlayerAnim_clink_demo_doorB_link;

RECOMP_CALLBACK("*", recomp_on_play_main)
void handleAgePropsOnPlay(PlayState *play) {
    if (gIsAgePropertyRefreshRequested) {
        gIsAgePropertyRefreshRequested = false;
        if (IS_HUMAN_ADULT_LINK_MODEL) {
            sPlayerAgeProperties[PLAYER_FORM_HUMAN] = gAdultLinkAgeProps;

            for (int i = 0; i < PLAYER_ANIMTYPE_MAX; i++) {
                D_8085BE84[PLAYER_ANIMGROUP_doorA][i] = D_8085BE84[PLAYER_ANIMGROUP_doorA_free][i];
                D_8085BE84[PLAYER_ANIMGROUP_doorB][i] = D_8085BE84[PLAYER_ANIMGROUP_doorB_free][i];
            }
        } else {
            sPlayerAgeProperties[PLAYER_FORM_HUMAN] = gVanillaHumanLinkAgeProps;

            for (int i = 0; i < PLAYER_ANIMTYPE_MAX; i++) {
                D_8085BE84[PLAYER_ANIMGROUP_doorA][i] = &gPlayerAnim_clink_demo_doorA_link;
                D_8085BE84[PLAYER_ANIMGROUP_doorB][i] = &gPlayerAnim_clink_demo_doorB_link;
            }
        }

        Player *player = (Player *)GET_PLAYER(play);

        if (player->transformation == PLAYER_FORM_HUMAN) {
            player->ageProperties = &sPlayerAgeProperties[PLAYER_FORM_HUMAN];
        }
    }
}

RECOMP_HOOK_RETURN("Object_LoadAll")
void refreshAgeProps_on_Object_LoadAll() {
    gIsAgePropertyRefreshRequested = true;
}

static Player *sPlayerFromHeight;
static PlayerTransformation sRealPlayerFormHeight;

RECOMP_HOOK("Player_GetHeight")
void on_Player_GetHeight(Player *player) {
    sPlayerFromHeight = player;
    sRealPlayerFormHeight = player->transformation;
    if (player->transformation == PLAYER_FORM_HUMAN && IS_HUMAN_ADULT_LINK_MODEL) {
        player->transformation = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("Player_GetHeight")
void return_Player_GetHeight(void) {
    sPlayerFromHeight->transformation = sRealPlayerFormHeight;
}

#define ADULT_LINK_BREMEN_HEIGHT_MODIFIER 1250.0f
bool gPushedMatrixBremen = false;

RECOMP_HOOK("Player_Draw")
void fixAdultBremen_on_Player_Draw(Actor *thisx, PlayState *play) {
    gPushedMatrixBremen = false;
    Player *this = (Player *)thisx;

    if (this->transformation == PLAYER_FORM_HUMAN && IS_HUMAN_ADULT_LINK_MODEL) {
        if (this->stateFlags3 & PLAYER_STATE3_20000000) {
            OPEN_DISPS(play->state.gfxCtx);
            Matrix_Push();
            gPushedMatrixBremen = true;
            Matrix_Translate(0.f, ADULT_LINK_BREMEN_HEIGHT_MODIFIER, 0.f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            CLOSE_DISPS(play->state.gfxCtx);
        }
    }
}

RECOMP_HOOK_RETURN("Player_Draw")
void fixAdultBremen_on_return_Player_Draw() {
    if (IS_HUMAN_ADULT_LINK_MODEL) {
        if (gPushedMatrixBremen) {
            Matrix_Pop();
        }
    }
}

RECOMP_HOOK("Player_PostLimbDrawGameplay")
void fixArrowPos_on_Player_PostLimbDrawGameplay(PlayState *play, s32 limbIndex, Gfx **dList1, Gfx **dList2, Vec3s *rot, Actor *actor) {
    Player *player = (Player *)actor;

    Actor *heldActor = player->heldActor;

    if (heldActor) {
        if (player->actor.scale.y >= 0.0f) {
            if (limbIndex == PLAYER_LIMB_LEFT_HAND && player->rightHandType == PLAYER_MODELTYPE_RH_BOW) {
                if (!Player_IsHoldingHookshot(player)) {
                    if ((player->stateFlags3 & PLAYER_STATE3_40) && (player->transformation != PLAYER_FORM_DEKU)) {
                        Mtx *arrowMtx = getFormProxyMatrix(GET_PLAYER_FORM_PROXY, LINK_EQUIP_MATRIX_ARROW_DRAWN);

                        if (arrowMtx) {
                            OPEN_DISPS(play->state.gfxCtx);
                            MtxF arrowMtxF;
                            Matrix_MtxToMtxF(arrowMtx, &arrowMtxF);
                            Matrix_Mult(&arrowMtxF, MTXMODE_APPLY);
                            CLOSE_DISPS(play->state.gfxCtx);
                        }
                    }
                }
            } else if (limbIndex == PLAYER_LIMB_RIGHT_HAND && Player_IsHoldingHookshot(player)) {
                Mtx *hookMtx = getFormProxyMatrix(GET_PLAYER_FORM_PROXY, LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK);

                if (hookMtx) {
                    OPEN_DISPS(play->state.gfxCtx);
                    MtxF hookMtxF;
                    Matrix_MtxToMtxF(hookMtx, &hookMtxF);
                    Matrix_Mult(&hookMtxF, MTXMODE_APPLY);
                    CLOSE_DISPS(play->state.gfxCtx);
                }
            }
        }
    }
}

// Use adult camera for first person bow/hookshot
static PlayerTransformation sRealPlayerFormFPCamera = PLAYER_FORM_HUMAN;
static Player *sPlayerFPCamera;

RECOMP_HOOK("func_8083868C")
void fixFPCmaera_on_func_8083868C(PlayState *play, Player *this) {
    sRealPlayerFormFPCamera = this->transformation;

    sPlayerFPCamera = this;

    if (this->transformation == PLAYER_FORM_HUMAN && IS_HUMAN_ADULT_LINK_MODEL) {
        this->transformation = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("func_8083868C")
void fixFPCmaera_on_return_func_8083868C() {
    sPlayerFPCamera->transformation = sRealPlayerFormFPCamera;
}

static PlayerTransformation sRealPlayerFormGrab;

RECOMP_HOOK("EnRd_Grab")
void fixEnemyHeight_on_EnRd_Grab(EnRd *this, PlayState *play) {
    Player *p = GET_PLAYER(play);
    sRealPlayerFormGrab = GET_PLAYER_FORM;

    if (IS_HUMAN_ADULT_LINK_MODEL && sRealPlayerFormGrab == PLAYER_FORM_HUMAN) {
        gSaveContext.save.playerForm = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("EnRd_Grab")
void fixEnemyHeight_on_return_EnRd_Grab() {
    gSaveContext.save.playerForm = sRealPlayerFormGrab;
}

// Music Box Gibdo height fix
static PlayerTransformation sRealPlayerFormIdealPosMusicBox;

RECOMP_HOOK("EnRailgibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_EnRailgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    Player *p = GET_PLAYER(play);
    sRealPlayerFormIdealPosMusicBox = GET_PLAYER_FORM;

    if (IS_HUMAN_ADULT_LINK_MODEL && sRealPlayerFormIdealPosMusicBox == PLAYER_FORM_HUMAN) {
        gSaveContext.save.playerForm = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("EnRailgibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_return_EnRailgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    gSaveContext.save.playerForm = sRealPlayerFormIdealPosMusicBox;
}

// Talking Redead/Gibdo height fix
static PlayerTransformation sRealPlayerFormIdealPosTalk;

RECOMP_HOOK("EnTalkGibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_EnTalkgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    Player *p = GET_PLAYER(play);
    sRealPlayerFormIdealPosTalk = GET_PLAYER_FORM;

    if (IS_HUMAN_ADULT_LINK_MODEL && sRealPlayerFormIdealPosTalk == PLAYER_FORM_HUMAN) {
        gSaveContext.save.playerForm = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("EnTalkGibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_return_EnTalkgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    gSaveContext.save.playerForm = sRealPlayerFormIdealPosTalk;
}
