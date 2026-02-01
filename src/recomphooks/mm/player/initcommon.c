#include "global.h"
#include "modding.h"

static Player *sPlayer;
static PlayState *sPlay;
static FlexSkeletonHeader *sSkelHeader;

void fixAdultAgeProps_on_Player_InitCommon(Player *player);

RECOMP_HOOK("Player_InitCommon") void on_Player_InitCommon(Player *player, PlayState *play, FlexSkeletonHeader *skelHeader) {
    sPlayer = player;
    sPlay = play;
    sSkelHeader = skelHeader;

    fixAdultAgeProps_on_Player_InitCommon(sPlayer);
}

RECOMP_HOOK_RETURN("Player_InitCommon") void on_return_Player_InitCommon(void) {

}
