#include "global.h"
#include "model_common.h"
#include "externaldisplaylists.h"
#include "assets/objects/object_link_child/object_link_child.h"

ZModelReplacerHandle gProxyBowString;

ZMODELREPLACER_CALLBACK_REGISTER_REPLACERS
void setupZPlayerLibDLReplacements() {
    gProxyBowString = ZModelReplacer_registerReplacer(OBJECT_LINK_CHILD, object_link_child_DL_017818, NULL);
}
