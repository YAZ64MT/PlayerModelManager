#include "global.h"
#include "modding.h"

static Player *sPlayer;
static PlayState *sPlay;

void initVanillaProps_on_Player_Init(void);
void setupPlayerFormProxy_on_Player_Init(Player *player);

RECOMP_HOOK("Player_Init") void on_Player_Init(Actor *thisx, PlayState *play) {
    sPlayer = (Player *)thisx;
    sPlay = play;

    initVanillaProps_on_Player_Init();
    setupPlayerFormProxy_on_Player_Init(sPlayer);
}

RECOMP_HOOK_RETURN("Player_Init") void on_return_Player_Init(void) {

}
