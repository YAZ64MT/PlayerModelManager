#include "global.h"
#include "externaldisplaylists.h"
#include "modelreplacer_api.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "model_common.h"

ZModelReplacerHandle gProxyBottleGlass;
ZModelReplacerHandle gProxyBottleContents;
ZModelReplacerHandle gProxyDekuStick;

ZMODELREPLACER_CALLBACK_REGISTER_REPLACERS
void setupGameplayKeepDLReplacements() {
    gProxyBottleGlass = ZModelReplacer_registerReplacer(GAMEPLAY_KEEP, gBottleGlassDL, NULL);
    gProxyBottleContents = ZModelReplacer_registerReplacer(GAMEPLAY_KEEP, gBottleContentsDL, NULL);
    gProxyDekuStick = ZModelReplacer_registerReplacer(GAMEPLAY_KEEP, gDekuStickDL, NULL);
}
