#include "global.h"
#include "externaldisplaylists.h"
#include "modelreplacer_api.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "assets/objects/object_mir_ray/object_mir_ray.h"

ModelReplacerHandle gProxyBottleGlass;
ModelReplacerHandle gProxyBottleContents;
ModelReplacerHandle gProxyDekuStick;
ModelReplacerHandle gProxyBowString;
ModelReplacerHandle gProxyBowArrow;
ModelReplacerHandle gProxyBowArrowLowPoly;
ModelReplacerHandle gProxyHookshotChain;
ModelReplacerHandle gProxyHookshotHook;
ModelReplacerHandle gProxyHookshotReticle;
ModelReplacerHandle gProxyMirrorShieldRay;
ModelReplacerHandle gProxyZoraLHand;
ModelReplacerHandle gProxyZoraRHand;
ModelReplacerHandle gProxyZoraLHandGuitar;

MODEL_REPLACER_CALLBACK_ON_REGISTER_REPLACERS
void setupExternalReplacements() {
    gProxyBottleGlass = ModelReplacer_registerReplacer(GAMEPLAY_KEEP, gBottleGlassDL, NULL);
    gProxyBottleContents = ModelReplacer_registerReplacer(GAMEPLAY_KEEP, gBottleContentsDL, NULL);
    gProxyDekuStick = ModelReplacer_registerReplacer(GAMEPLAY_KEEP, gDekuStickDL, NULL);
    gProxyBowString = ModelReplacer_registerReplacer(OBJECT_LINK_CHILD, object_link_child_DL_017818, NULL);
    gProxyBowArrow = ModelReplacer_registerReplacer(GAMEPLAY_KEEP, gameplay_keep_DL_013FF0, NULL);
    gProxyBowArrowLowPoly = ModelReplacer_registerReplacer(GAMEPLAY_KEEP, gameplay_keep_DL_014370, NULL);
    gProxyHookshotChain = ModelReplacer_registerReplacer(GAMEPLAY_KEEP, gHookshotChainDL, NULL);
    gProxyHookshotHook = ModelReplacer_registerReplacer(OBJECT_LINK_CHILD, object_link_child_DL_01D960, NULL);
    gProxyHookshotReticle = ModelReplacer_registerReplacer(GAMEPLAY_KEEP, gHookshotReticleDL, NULL);
    gProxyMirrorShieldRay = ModelReplacer_registerReplacer(OBJECT_MIR_RAY, object_mir_ray_DL_0004B0, NULL);
    gProxyZoraLHand = ModelReplacer_registerReplacer(OBJECT_LINK_ZORA, gLinkZoraLeftHandOpenDL, NULL);
    gProxyZoraLHandGuitar = ModelReplacer_registerReplacer(OBJECT_LINK_ZORA, object_link_zora_DL_00E088, NULL);
    gProxyZoraRHand = ModelReplacer_registerReplacer(OBJECT_LINK_ZORA, gLinkZoraRightHandOpenDL, NULL);
}
