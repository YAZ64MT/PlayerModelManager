#include "global.h"
#include "modding.h"
#include "playermodelmanager.h"
#include "playermodelmanager_mm.h"
#include "model_common.h"
#include "z64recomp_api.h"
#include "overlays/actors/ovl_Mir_Ray3/z_mir_ray3.h"

ActorExtensionId gPlayerExtIdMirrorShieldBackMf;

RECOMP_CALLBACK("*", recomp_on_init)
void initMirrorShieldActorExtensions() {
    gPlayerExtIdMirrorShieldBackMf = z64recomp_extend_actor(ACTOR_PLAYER, sizeof(MtxF));
}

extern Vec3f sSheathLimbModelShieldOnBackPos;
extern Vec3s sSheathLimbModelShieldOnBackZyxRot;

RECOMP_HOOK("Player_PostLimbDrawGameplay")
void setMirrorShieldMtxF_on_Player_PostLimbDrawGameplay(PlayState *play, s32 limbIndex, Gfx **dList1, Gfx **dList2, Vec3s *rot, Actor *actor) {
    if (limbIndex == PLAYER_LIMB_SHEATH) {
        Player *player = (Player *)actor;
        if (player->actor.scale.y >= 0.0f) {
            static MtxF shieldMf;

            Matrix_Push();

            Link_FormProxy *formProxy = &gLinkFormProxies[player->transformation];

            Matrix_MtxToMtxF(getFormProxyMatrix(formProxy, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK), &shieldMf);

            Matrix_Mult(&shieldMf, MTXMODE_APPLY);

            Matrix_Get(z64recomp_get_extended_actor_data(actor, gPlayerExtIdMirrorShieldBackMf));

            Matrix_Pop();
        }
    }
}

static MtxF sRealShieldMf;
static Player *sMirRay3Player;

RECOMP_HOOK("MirRay3_Draw")
void overwriteShieldMtx_on_MirRay3_Draw(Actor *thisx, PlayState *play) {
    MirRay3 *this = (MirRay3 *)thisx;
    Player *player = GET_PLAYER(play);

    sRealShieldMf = player->shieldMf;
    sMirRay3Player = player;

    if (!(this->unk_210 & 1) && Player_HasMirrorShieldEquipped(play) && (player->rightHandType != PLAYER_MODELTYPE_RH_SHIELD) &&
        (player->rightHandType != PLAYER_MODELTYPE_RH_FF)) {
        MtxF *shieldMtx = z64recomp_get_extended_actor_data((Actor *)player, gPlayerExtIdMirrorShieldBackMf);

        player->shieldMf = *shieldMtx;
    }
}

RECOMP_HOOK_RETURN("MirRay3_Draw")
void overwriteShieldMtx_on_return_MirRay3_Draw() {
    sMirRay3Player->shieldMf = sRealShieldMf;
}
