#include "global.h"
#include "modding.h"
#include "playermodelmanager.h"
#include "model_common.h"
#include "defines_modelinfo.h"

static bool sPushedMaskMatrix = false;
extern LinkAnimationHeader gPlayerAnim_cl_setmask;
RECOMP_HOOK("Player_PostLimbDrawGameplay")
void useMaskMtx_on_Player_PostLimbDrawGameplay(PlayState *play, s32 limbIndex, Gfx **dList1, Gfx **dList2, Vec3s *rot, Actor *actor) {
    sPushedMaskMatrix = false;

    if (limbIndex == PLAYER_LIMB_HEAD) {
        Player *player = (Player *)actor;

        Mtx *maskMtx = getFormProxyMatrix(&gLinkFormProxies[player->transformation], LINK_EQUIP_MATRIX_MASKS);

        if (maskMtx) {
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
}

RECOMP_HOOK_RETURN("Player_PostLimbDrawGameplay")
void useMaskMtx_on_return_Player_PostLimbDrawGameplay(void) {
    if (sPushedMaskMatrix) {
        Matrix_Pop();
    }
}

#define ADULT_LINK_BREMEN_HEIGHT_MODIFIER 1250.0f
static bool sPushedMatrixBremen = false;

RECOMP_HOOK("Player_Draw")
void fixAdultBremen_on_Player_Draw(Actor *thisx, PlayState *play) {
    sPushedMatrixBremen = false;
    Player *this = (Player *)thisx;

    if (this->transformation == PLAYER_FORM_HUMAN && IS_HUMAN_ADULT_LINK_MODEL) {
        if (this->stateFlags3 & PLAYER_STATE3_20000000) {
            OPEN_DISPS(play->state.gfxCtx);
            Matrix_Push();
            sPushedMatrixBremen = true;
            Matrix_Translate(0.f, ADULT_LINK_BREMEN_HEIGHT_MODIFIER, 0.f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            CLOSE_DISPS(play->state.gfxCtx);
        }
    }
}

RECOMP_HOOK_RETURN("Player_Draw")
void fixAdultBremen_on_return_Player_Draw() {
    if (IS_HUMAN_ADULT_LINK_MODEL) {
        if (sPushedMatrixBremen) {
            Matrix_Pop();
        }
    }
}

RECOMP_HOOK("Player_PostLimbDrawGameplay")
void fixArrowPos_on_Player_PostLimbDrawGameplay(PlayState *play, s32 limbIndex, Gfx **dList1, Gfx **dList2, Vec3s *rot, Actor *actor) {
    Player *player = (Player *)actor;

    Actor *heldActor = player->heldActor;

    Link_FormProxy *fp = &gLinkFormProxies[player->transformation];

    if (heldActor) {
        if (player->actor.scale.y >= 0.0f) {
            if (limbIndex == PLAYER_LIMB_LEFT_HAND && player->rightHandType == PLAYER_MODELTYPE_RH_BOW) {
                if (!Player_IsHoldingHookshot(player)) {
                    if ((player->stateFlags3 & PLAYER_STATE3_40) && (player->transformation != PLAYER_FORM_DEKU)) {
                        Mtx *arrowMtx = getFormProxyMatrix(fp, LINK_EQUIP_MATRIX_ARROW_DRAWN);

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
                Mtx *hookMtx = getFormProxyMatrix(fp, LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK);

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
