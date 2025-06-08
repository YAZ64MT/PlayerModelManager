#include "global.h"
#include "externaldisplaylists.h"
#include "modelreplacer_api.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_link_child/object_link_child.h"

ZModelReplacerHandle gProxyBottleGlass;
ZModelReplacerHandle gProxyBottleContents;
ZModelReplacerHandle gProxyDekuStick;
ZModelReplacerHandle gProxyBowString;

ZMODELREPLACER_CALLBACK_REGISTER_REPLACERS
void setupExternalReplacements() {
    gProxyBottleGlass = ZModelReplacer_registerReplacer(GAMEPLAY_KEEP, gBottleGlassDL, NULL);
    gProxyBottleContents = ZModelReplacer_registerReplacer(GAMEPLAY_KEEP, gBottleContentsDL, NULL);
    gProxyDekuStick = ZModelReplacer_registerReplacer(GAMEPLAY_KEEP, gDekuStickDL, NULL);
    gProxyBowString = ZModelReplacer_registerReplacer(OBJECT_LINK_CHILD, object_link_child_DL_017818, NULL);
}
