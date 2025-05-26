#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "model_common.h"
#include "mm_adultfixes.h"
#include "defines_modelinfo.h"

Player *gPlayer;
PlayerTransformation gPlayerTransformation;
PlayState *gPlayState;
s32 gLimbIndex;
Gfx **gFirstPersonDList;

// restore left forearm display list
RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson")
void pre_OverrideLimbDrawFirstPerson(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    gPlayState = play;
    gLimbIndex = limbIndex;
    gFirstPersonDList = dList;
}

RECOMP_HOOK_RETURN("Player_OverrideLimbDrawGameplayFirstPerson")
void post_OverrideLimbDrawFirstPerson() {
    if (gLimbIndex == PLAYER_LIMB_RIGHT_FOREARM) {
        *gFirstPersonDList = &gLinkFormProxies[GET_PLAYER_FORM].proxyDisplayLists[LINK_DL_FPS_RFOREARM];
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

extern PlayerAnimationHeader *D_8085BE84[PLAYER_ANIMGROUP_MAX][PLAYER_ANIMTYPE_MAX];
extern LinkAnimationHeader gPlayerAnim_clink_demo_doorA_link;
extern LinkAnimationHeader gPlayerAnim_clink_demo_doorB_link;

RECOMP_HOOK("Player_Update")
void handleAgeProps_onPlayerUpdate(Actor *thisx, PlayState *play) {
    if (!isAdultAgePropsInitialized()) {
        gVanillaHumanLinkAgeProps = sPlayerAgeProperties[PLAYER_FORM_HUMAN];
        initAdultLinkAgeProperties();
    }

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

        Player *player = (Player *)thisx;

        if (player->transformation == PLAYER_FORM_HUMAN) {
            player->ageProperties = &sPlayerAgeProperties[PLAYER_FORM_HUMAN];
        }
    }
}

RECOMP_HOOK_RETURN("Object_LoadAll")
void refreshAgeProps_on_Object_LoadAll() {
    gIsAgePropertyRefreshRequested = true;
}

RECOMP_HOOK("Player_GetHeight")
void on_Player_GetHeight(Player *player) {
    gPlayer = player;
    gPlayerTransformation = player->transformation;
    if (player->transformation == PLAYER_FORM_HUMAN && IS_HUMAN_ADULT_LINK_MODEL) {
        player->transformation = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("Player_GetHeight")
void return_Player_GetHeight(void) {
    gPlayer->transformation = gPlayerTransformation;
}

#define ADULT_LINK_BREMEN_HEIGHT_MODIFIER 1250.0f
bool gPushedMatrixBremen = false;

RECOMP_HOOK("Player_Draw")
void on_Player_Draw(Actor *thisx, PlayState *play) {
    if (IS_HUMAN_ADULT_LINK_MODEL) {
        Player *this = (Player *)thisx;

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
void return_Player_Draw(Actor *thisx, PlayState *play) {
    if (IS_HUMAN_ADULT_LINK_MODEL) {
        if (gPushedMatrixBremen) {
            Matrix_Pop();
        }
        gPushedMatrixBremen = false;
    }
}

#define ADULT_MASK_SCALE_MODIFIER 1.0f
#define ADULT_MASK_DOWNWARDS_OFFSET -200.f
#define ADULT_MASK_FORWARDS_OFSSEET 0.0f
#define ADULT_MASK_LATERAL_OFFSET 0.0f
bool gPushedMaskMatrix = false;
extern LinkAnimationHeader gPlayerAnim_cl_setmask;
RECOMP_HOOK("Player_PostLimbDrawGameplay")
void on_Player_PostLimbDrawGameplay(PlayState *play, s32 limbIndex, Gfx **dList1, Gfx **dList2, Vec3s *rot, Actor *actor) {
    if (limbIndex == PLAYER_LIMB_HEAD && IS_HUMAN_ADULT_LINK_MODEL) {
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
            gPushedMaskMatrix = true;
            Matrix_Scale(ADULT_MASK_SCALE_MODIFIER, ADULT_MASK_SCALE_MODIFIER, ADULT_MASK_SCALE_MODIFIER, MTXMODE_APPLY);
            Matrix_Translate(ADULT_MASK_FORWARDS_OFSSEET, ADULT_MASK_DOWNWARDS_OFFSET, ADULT_MASK_LATERAL_OFFSET, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            CLOSE_DISPS(play->state.gfxCtx);
        } else {
            gPushedMaskMatrix = false;
        }
    } else {
        gPushedMaskMatrix = false;
    }
}

RECOMP_HOOK_RETURN("Player_PostLimbDrawGameplay")
void return_Player_PostLimbDrawGameplay(void) {
    if (gPushedMaskMatrix) {
        Matrix_Pop();
    }
    gPushedMaskMatrix = false;
}
