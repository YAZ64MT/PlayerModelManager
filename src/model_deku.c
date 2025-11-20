#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "playermodelmanager_utils.h"
#include "globalobjects_api.h"
#include "modelentrymanager.h"
#include "modelinfo.h"
#include "modelentry.h"
#include "playermodelmanager_api.h"

ModelEntryForm *gDekuModelEntry;
ModelInfo gDekuModelInfo;

Gfx *getDekuDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_NUTS, dl);
}

static void setupDekuFallbackModel() {
    void *deku = GlobalObjects_getGlobalObject(OBJECT_LINK_NUTS);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(deku, &gLinkDekuSkel);
    GlobalObjects_globalizeLodLimbSkeleton(deku, &gLinkDekuSkel);

    ModelInfo_init(&gDekuModelInfo);

    ModelEntryForm *entryForm = gDekuModelEntry = (ModelEntryForm *)CMEM_getEntry(CMEM_createMemoryHandle(PMM_MODEL_TYPE_DEKU, "__mm_object_link_nuts__"));
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    ModelEntryForm_fillDefaultFaceTextures(entryForm);
    ModelEntryForm_setSkeleton(entryForm, skel);
    ModelEntryForm_setDLsFromSkeletons(entryForm);

    CMEM_setEntryHidden(entry, true);
    ModelInfo_setModelEntryForm(&gDekuModelInfo, entryForm);

#define SET_ENTRY_DL(id, dl) ModelEntry_setDisplayList(entry, id, dl)

    // hands
    SET_ENTRY_DL(LINK_DL_LHAND, getDekuDL(gLinkDekuLeftHandDL));
    SET_ENTRY_DL(LINK_DL_LFIST, getDekuDL(gLinkDekuLeftHandDL));
    SET_ENTRY_DL(LINK_DL_LHAND_BOTTLE, getDekuDL(gLinkDekuLeftHandDL));
    SET_ENTRY_DL(LINK_DL_LHAND_GUITAR, getDekuDL(gLinkDekuLeftHandDL));

    SET_ENTRY_DL(LINK_DL_RHAND, getDekuDL(gLinkDekuRightHandDL));
    SET_ENTRY_DL(LINK_DL_RFIST, getDekuDL(gLinkDekuRightHandDL));

    // First Person
    SET_ENTRY_DL(LINK_DL_FPS_LFOREARM, getDekuDL(gLinkDekuLeftForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_LHAND, getDekuDL(gLinkDekuLeftHandDL));
    SET_ENTRY_DL(LINK_DL_FPS_RFOREARM, getDekuDL(gLinkDekuRightForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_RHAND, getDekuDL(gLinkDekuRightHandDL));

#undef SET_ENTRY_DL
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaDeku() {
    setupDekuFallbackModel();
}
