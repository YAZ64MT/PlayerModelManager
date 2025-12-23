#include "global.h"
#include "modding.h"

static PlayState *sPlay;

void processFormProxyRefreshRequests_on_Play_Draw();
void skipInterpolation_on_Play_Draw(PlayState *play);

RECOMP_HOOK("Play_Draw") void on_Play_Draw(PlayState *this) {
    sPlay = this;

    processFormProxyRefreshRequests_on_Play_Draw();
    skipInterpolation_on_Play_Draw(sPlay);
}

RECOMP_HOOK_RETURN("Play_Draw") void on_return_Play_Draw() {

}
