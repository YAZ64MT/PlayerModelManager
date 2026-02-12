#include "global.h"
#include "modding.h"
#include "overlays/actors/ovl_En_Test3/z_en_test3.h"

static EnTest3 *sEnTest3;
static PlayState *sPlay;

void updateAssets_on_EnTest3_Draw(EnTest3 *enTest3);

RECOMP_HOOK("EnTest3_Draw") void on_EnTest3_Draw(Actor *thisx, PlayState *play) {
    sEnTest3 = (EnTest3 *)thisx;
    sPlay = play;

    updateAssets_on_EnTest3_Draw(sEnTest3);
}

RECOMP_HOOK_RETURN("EnTest3_Draw") void on_return_EnTest3_Draw(void) {

}
