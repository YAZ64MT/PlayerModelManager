#include "zproxy_manager.h"
#include "global.h"
#include "recomputils.h"

extern Gfx gDekuStickDL[];

RECOMP_CALLBACK(".", ZProxyManager_onInit)
void reserveDekuStick() {
    ZProxyManager_reserveVanillaDisplayList(GAMEPLAY_KEEP, &gDekuStickDL);
}
