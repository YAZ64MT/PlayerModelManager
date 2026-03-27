#include "global.h"
#include "modding.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "modelmatrixids.h"
#include "playermodelmanager_api.h"
#include "customdls.h"
#include "apilocal.h"
#include "fallbackmodels.h"
#include "logger.h"
#include "mm/vanillawrapperdls.h"
#include "yazmtcorelib_api.h"

PlayerModelManagerHandle gHumanModelHandle;

Gfx sPrimTunicCombinerDL[] = {
    gsDPSetCombineLERP(TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0, 0xFF, 30, 105, 27, 255),
};

Gfx sEnvTunicCombinerDL[] = {gsDPSetCombineLERP(TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED)};

Gfx *getHumanDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, dl);
}

static void makeTunicColorable(Gfx *dl) {
    while (dl->words.w0 >> 24 != G_ENDDL) {
        if (dl->words.w0 >> 24 == G_DL) {
            makeTunicColorable((Gfx *)dl->words.w1);

            if (((dl->words.w0 >> 16) & 0xFF) == G_DL_NOPUSH) {
                break;
            }
        }

        if (YAZMTCore_Utils_MemCmp(sPrimTunicCombinerDL, dl, sizeof(sPrimTunicCombinerDL)) == 0) {
            *dl = sEnvTunicCombinerDL[0];
        }

        dl++;
    }
}

void registerHuman(void) {
    void *human = GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSkel);

    gHumanModelHandle = PlayerModelManager_registerModel(PMM_API_VERSION, "__mm_object_link_child__", PMM_MODEL_TYPE_CHILD);

    FallbackModelsCommon_doCommonAssignments(gHumanModelHandle, skel, human, GlobalObjects_getGlobalObject(GAMEPLAY_KEEP));

    for (u8 i = 0; i < skel->sh.limbCount; ++i) {
        LodLimb *curr = skel->sh.segment[i];

        if (curr->dLists[0]) {
            makeTunicColorable(curr->dLists[0]);
        }
    }

    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_OPT_LFIST, getHumanDL(gLinkHumanLeftHandClosedDL));
    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_OPT_LHAND_BOTTLE, getHumanDL(gLinkHumanLeftHandHoldBottleDL));
    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_OPT_LHAND_GUITAR, getHumanDL(gLinkHumanLeftHandOpenDL));
    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_OPT_RFIST, getHumanDL(gLinkHumanRightHandClosedDL));
    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_OPT_RHAND_OCARINA, gLinkHumanRightHandOcarinaDL);
    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_OPT_FPS_RFOREARM, gEmptyDL);
    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_OPT_FPS_RHAND, gLinkHumanFirstPersonArmDL);
    PlayerModelManager_setDisplayList(gHumanModelHandle, LINK_DL_ELEGY_OF_EMPTINESS_SHELL, gCallHumanElegyShellDL);

    FallbackModelsCommon_addEquipmentChildMM(gHumanModelHandle);
}
