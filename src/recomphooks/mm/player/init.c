#include "global.h"
#include "modding.h"

static Player *sPlayer;
static PlayState *sPlay;

RECOMP_HOOK("Player_Init") void on_Player_Init(Actor *thisx, PlayState *play) {
    sPlayer = (Player *)thisx;
    sPlay = play;
}

RECOMP_HOOK_RETURN("Player_Init") void on_return_Player_Init(void) {

}
