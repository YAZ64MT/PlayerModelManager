#include "global.h"
#include "model_common.h"
#include "externaldisplaylists.h"
#include "assets/objects/object_link_child/object_link_child.h"

ZModelReplacerHandle gProxyBowString;

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyFormProxies)
void setupZPlayerLibDLReplacements() {
    gProxyBowString = ZModelReplacer_createReplacer(OBJECT_LINK_CHILD, object_link_child_DL_017818);

    Link_FormProxy *humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

    ZModelReplacer_setReplacerModel(gProxyBowString, &humanProxy->displayLists[LINK_DL_BOW_STRING]);

    ZModelReplacer_pushReplacer(gProxyBowString);
}
