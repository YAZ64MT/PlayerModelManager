#include "zproxy_manager.h"
#include "global.h"
#include "recomputils.h"

extern Gfx gBottleContentsDL[];
extern Gfx gBottleGlassDL[];

RECOMP_CALLBACK(".", ZProxyManager_onInit)
void reserveGlassBottle() {
    //recomp_printf("Reserving glass bottle slots...\n");
    ZProxyManager_reserveVanillaDisplayList(GAMEPLAY_KEEP, &gBottleGlassDL);
    ZProxyManager_reserveVanillaDisplayList(GAMEPLAY_KEEP, &gBottleContentsDL);
}
