#include "global.h"
#include "externaldisplaylists.h"
#include "modelreplacer_api.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "model_common.h"

ZModelReplacerHandle gProxyBottleGlass;
ZModelReplacerHandle gProxyBottleContents;
ZModelReplacerHandle gProxyDekuStick;

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyFormProxies)
void setupGameplayKeepDLReplacements() {
    gProxyBottleGlass = ZModelReplacer_createReplacer(GAMEPLAY_KEEP, gBottleGlassDL);
    gProxyBottleContents = ZModelReplacer_createReplacer(GAMEPLAY_KEEP, gBottleContentsDL);
    gProxyDekuStick = ZModelReplacer_createReplacer(GAMEPLAY_KEEP, gDekuStickDL);

    Link_FormProxy *humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

    ZModelReplacer_setReplacerModel(gProxyBottleGlass, &humanProxy->displayLists[LINK_DL_BOTTLE_GLASS]);
    ZModelReplacer_setReplacerModel(gProxyBottleContents, &humanProxy->displayLists[LINK_DL_BOTTLE_CONTENTS]);
    ZModelReplacer_setReplacerModel(gProxyDekuStick, &humanProxy->displayLists[LINK_DL_DEKU_STICK]);

    ZModelReplacer_pushReplacer(gProxyBottleGlass);
    ZModelReplacer_pushReplacer(gProxyBottleContents);
    ZModelReplacer_pushReplacer(gProxyDekuStick);
}
