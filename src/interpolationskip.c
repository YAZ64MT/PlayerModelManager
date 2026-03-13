#include "global.h"
#include "modding.h"
#include "modelmatrixids.h"
#include "z64recomp_api.h"
#include "playermodelmanager_api.h"
#include "overlays/actors/ovl_En_Test3/z_en_test3.h"

static bool sShouldSkipFormInterpolation[PLAYER_FORM_MAX];
static bool sShouldSkipMirrorShieldInterpolation;
static bool sShouldSkipHookshotInterpolation;

RECOMP_CALLBACK(".", _internal_onFormModelApplied) void refreshSharedModelsOnFormModelApply(PlayerTransformation form) {
    sShouldSkipFormInterpolation[form] = true;
}

RECOMP_CALLBACK(".", _internal_onEquipmentModelApplied) void refreshEquipmentOnEquipmentModelApplied(PlayerModelManagerModelType type) {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        sShouldSkipFormInterpolation[i] = true;
    }

    if (type == PMM_MODEL_TYPE_SHIELD3) {
        sShouldSkipMirrorShieldInterpolation = true;
    }

    if (type == PMM_MODEL_TYPE_HOOKSHOT) {
        sShouldSkipHookshotInterpolation = true;
    }
}

void skipInterpolation_on_Play_UpdateMain(PlayState *play) {
    Player *player = GET_PLAYER(play);

    while (player) {
        if (sShouldSkipFormInterpolation[player->transformation]) {
            actor_set_interpolation_skipped(&player->actor);
            sShouldSkipMirrorShieldInterpolation = true;
            sShouldSkipHookshotInterpolation = true;
        }

        player = (Player *)player->actor.next;
    }

    Actor *npc = play->actorCtx.actorLists[ACTORCAT_NPC].first;

    while (npc) {
        if (npc->id == ACTOR_EN_TEST3) {
            EnTest3 *kafei = (EnTest3 *)npc;
            if (sShouldSkipFormInterpolation[kafei->player.transformation]) {
                actor_set_interpolation_skipped(npc);
            }
        }

        npc = npc->next;
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
