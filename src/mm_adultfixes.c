#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "model_common.h"

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
