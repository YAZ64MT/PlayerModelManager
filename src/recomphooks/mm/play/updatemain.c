#include "global.h"
#include "modding.h"

static PlayState *sPlay;

void updateAdultProperties_on_Play_UpdateMain(PlayState *play);
void checkToOpenModelMenu_on_Play_UpdateMain(PlayState *play);
void processPlayerProxyRefreshRequests_on_Play_UpdateMain(void);
void processFormProxyRefreshRequests_on_Play_UpdateMain(void);
void skipInterpolation_on_Play_UpdateMain(PlayState *play);

RECOMP_HOOK("Play_UpdateMain") void on_Play_UpdateMain(PlayState *this) {
    sPlay = this;

    updateAdultProperties_on_Play_UpdateMain(sPlay);
    checkToOpenModelMenu_on_Play_UpdateMain(sPlay);
    processPlayerProxyRefreshRequests_on_Play_UpdateMain();
    processFormProxyRefreshRequests_on_Play_UpdateMain();
    skipInterpolation_on_Play_UpdateMain(sPlay);
}

RECOMP_HOOK_RETURN("Play_UpdateMain") void on_return_Play_UpdateMain(void) {

}
