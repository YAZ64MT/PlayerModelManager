#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "modelmatrixids.h"
#include "playermodelmanager_api.h"
#include "customdls.h"
#include "apilocal.h"
#include "fallbackmodels.h"
#include "logger.h"

PlayerModelManagerHandle gHumanModelHandle;

Gfx *getHumanDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, dl);
}

void registerHuman(void) {
    void *human = GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSkel);

    gHumanModelHandle = PlayerModelManager_registerModel(PMM_API_VERSION, "__mm_object_link_child__", PMM_MODEL_TYPE_CHILD);

    FallbackModelsCommon_doCommonAssignments(gHumanModelHandle, skel, human, GlobalObjects_getGlobalObject(GAMEPLAY_KEEP));

#define SET_ENTRY_DL(id, dl) PlayerModelManager_setDisplayList(gHumanModelHandle, id, dl)

    // hands
    SET_ENTRY_DL(LINK_DL_OPT_LFIST, getHumanDL(gLinkHumanLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_OPT_LHAND_BOTTLE, getHumanDL(gLinkHumanLeftHandHoldBottleDL));
    SET_ENTRY_DL(LINK_DL_OPT_LHAND_GUITAR, getHumanDL(gLinkHumanLeftHandOpenDL));

    SET_ENTRY_DL(LINK_DL_OPT_RFIST, getHumanDL(gLinkHumanRightHandClosedDL));

    SET_ENTRY_DL(LINK_DL_OPT_RHAND_OCARINA, gLinkHumanRightHandOcarinaDL);

    // First Person
    SET_ENTRY_DL(LINK_DL_OPT_FPS_RFOREARM, gEmptyDL);
    SET_ENTRY_DL(LINK_DL_OPT_FPS_RHAND, gLinkHumanFirstPersonArmDL);

#undef SET_ENTRY_DL

    FallbackModelsCommon_addEquipmentChildMM(gHumanModelHandle);
}
