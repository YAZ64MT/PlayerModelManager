#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "assets/objects/object_link_child/object_link_child.h"

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());



const unsigned char *modsPath = NULL;

RECOMP_CALLBACK("*", recomp_on_play_main)
void onPlayMain(PlayState *play) {
    if (!modsPath) {
        modsPath = recomp_get_mod_folder_path();
    }
}
