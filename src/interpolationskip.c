#include "global.h"
#include "modding.h"
#include "modelmatrixids.h"
#include "z64recomp_api.h"

static bool sShouldSkipFormInterpolation[PLAYER_FORM_MAX];
static bool sShouldSkipMirrorShieldInterpolation;
static bool sShouldSkipHookshotInterpolation;

RECOMP_CALLBACK(".", _internal_onFormModelApplied) void refreshSharedModelsOnFormModelApply(PlayerTransformation form) {
    sShouldSkipFormInterpolation[form] = true;
}

RECOMP_CALLBACK(".", _internal_onEquipmentModelApplied) void refreshEquipmentOnEquipmentModelApplied(Link_EquipmentReplacement eq) {
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

void skipInterpolation_on_Play_Draw(PlayState *play) {
    Player *player = GET_PLAYER(play);

    while (player) {
        if (sShouldSkipFormInterpolation[player->transformation] && player->actor.scale.y >= 0.0f) {
            actor_set_interpolation_skipped(&player->actor);
            sShouldSkipMirrorShieldInterpolation = true;
            sShouldSkipHookshotInterpolation = true;
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
