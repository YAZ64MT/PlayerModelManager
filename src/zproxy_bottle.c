#include "zproxy_manager.h"
#include "global.h"
#include "recomputils.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

RECOMP_CALLBACK(".", ZProxyManager_onInit)
void reserveGlassBottle() {
    //recomp_printf("Reserving glass bottle slots...\n");
    ZProxyManager_reserveVanillaDisplayList(GAMEPLAY_KEEP, &gBottleGlassDL);
    ZProxyManager_reserveVanillaDisplayList(GAMEPLAY_KEEP, &gBottleContentsDL);
}
