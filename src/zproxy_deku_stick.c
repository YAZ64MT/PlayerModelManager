#include "global.h"
#include "recomputils.h"
#include "zproxy_manager.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

RECOMP_CALLBACK(".", ZProxyManager_onInit)
void reserveDekuStick() {
    ZProxyManager_reserveVanillaDisplayList(GAMEPLAY_KEEP, &gDekuStickDL);
}
