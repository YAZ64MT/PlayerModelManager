#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager.h"

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());

#define DISPLAY_LIST

Gfx DF_COMMAND[] = {
    gsSPEndDisplayList()
};

Gfx CALL_DF_COMMAND[] = {
    gsSPDisplayList(DF_COMMAND),
    gsSPEndDisplayList()
};

static Gfx **sPlayerModels[PLAYER_FORM_MAX] = {NULL, NULL, NULL, NULL, NULL};



const unsigned char *modsPath = NULL;

// initialize player models as blank display lists
void initPlayerModels() {
    for (u8 i = 0; i < PLAYER_FORM_MAX; i++) {
        if (!sPlayerModels[i]) {
            Gfx **modelList = recomp_alloc(sizeof(Gfx *) * LINK_DL_MAX);
            for (u16 j = 0; j < LINK_DL_MAX; j++) {
                modelList[j] = CALL_DF_COMMAND;
            }
            sPlayerModels[i] = modelList;
        }
    }
}

RECOMP_CALLBACK("*", recomp_on_play_main)
void onPlayMain(PlayState *play) {
    if (!modsPath) {
        modsPath = recomp_get_mod_folder_path();
    }
    initPlayerModels();
}
