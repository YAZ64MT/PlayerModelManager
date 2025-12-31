#include "global.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "playermodelmanager_api.h"
#include "defaultfacetex.h"
#include "apilocal.h"
#include "fallbackmodels.h"

PlayerModelManagerHandle gZoraModelHandle;

Gfx *getZoraDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_ZORA, dl);
}

static void setupZoraFallbackModel() {
    void *zora = GlobalObjects_getGlobalObject(OBJECT_LINK_ZORA);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(zora, &gLinkZoraSkel);

    gZoraModelHandle = PlayerModelManager_registerModel(PMM_API_VERSION, "__mm_object_link_zora__", PMM_MODEL_TYPE_ZORA);

    FallbackModelsCommon_doCommonAssignments(gZoraModelHandle, skel, zora, GlobalObjects_getGlobalObject(GAMEPLAY_KEEP));

    PlayerModelManager_setSkeleton(gZoraModelHandle, skel);

#define SET_ENTRY_DL(id, dl) PlayerModelManager_setDisplayList(gZoraModelHandle, id, dl)

    // hands
    SET_ENTRY_DL(LINK_DL_LHAND, getZoraDL(gLinkZoraLeftHandOpenDL));
    SET_ENTRY_DL(LINK_DL_OPT_LFIST, getZoraDL(gLinkZoraLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_OPT_LHAND_BOTTLE, getZoraDL(gLinkZoraLeftHandHoldBottleDL));
    SET_ENTRY_DL(LINK_DL_OPT_LHAND_GUITAR, getZoraDL(object_link_zora_DL_00E088));

    SET_ENTRY_DL(LINK_DL_RHAND, getZoraDL(gLinkZoraRightHandOpenDL));
    SET_ENTRY_DL(LINK_DL_OPT_RFIST, getZoraDL(gLinkZoraRightHandClosedDL));;

#undef SET_ENTRY_DL
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaZora() {
    setupZoraFallbackModel();
}
