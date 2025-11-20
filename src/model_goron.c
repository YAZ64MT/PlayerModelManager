#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "playermodelmanager_utils.h"
#include "globalobjects_api.h"
#include "modelentrymanager.h"
#include "modelinfo.h"
#include "modelentry.h"
#include "playermodelmanager_api.h"

ModelEntryForm *gGoronModelEntry;
ModelInfo gGoronModelInfo;

Gfx *getGoronDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_GORON, dl);
}

static void setupGoronFallbackModel() {
    void *goron = GlobalObjects_getGlobalObject(OBJECT_LINK_GORON);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(goron, &gLinkGoronSkel);
    GlobalObjects_globalizeLodLimbSkeleton(goron, &gLinkGoronSkel);

    FlexSkeletonHeader *shieldingSkel = SEGMENTED_TO_GLOBAL_PTR(goron, &gLinkGoronShieldingSkel);
    GlobalObjects_globalizeStandardLimbSkeleton(goron, &gLinkGoronShieldingSkel);

    ModelInfo_init(&gGoronModelInfo);

    ModelEntryForm *entryForm = gGoronModelEntry = (ModelEntryForm *)CMEM_getEntry(CMEM_createMemoryHandle(PMM_MODEL_TYPE_GORON, "__mm_object_link_goron__"));
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);

    ModelEntryForm_fillDefaultFaceTextures(entryForm);
    ModelEntryForm_setSkeleton(entryForm, skel);
    ModelEntryForm_setShieldingSkeleton(entryForm, shieldingSkel);
    ModelEntryForm_setDLsFromSkeletons(entryForm);

    CMEM_setEntryHidden(entry, true);
    ModelInfo_setModelEntryForm(&gGoronModelInfo, entryForm);

#define SET_ENTRY_DL(id, dl) ModelEntry_setDisplayList(entry, id, dl)

    // hands
    SET_ENTRY_DL(LINK_DL_LHAND, getGoronDL(gLinkGoronLeftHandOpenDL));
    SET_ENTRY_DL(LINK_DL_LFIST, getGoronDL(gLinkGoronLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_LHAND_BOTTLE, getGoronDL(gLinkGoronLeftHandHoldBottleDL));
    SET_ENTRY_DL(LINK_DL_LHAND_GUITAR, getGoronDL(gLinkGoronLeftHandOpenDL));

    SET_ENTRY_DL(LINK_DL_RHAND, getGoronDL(gLinkGoronRightHandOpenDL));
    SET_ENTRY_DL(LINK_DL_RFIST, getGoronDL(gLinkGoronRightHandClosedDL));

    // First Person
    SET_ENTRY_DL(LINK_DL_FPS_LFOREARM, getGoronDL(gLinkGoronLeftForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_LHAND, getGoronDL(gLinkGoronLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_FPS_RFOREARM, getGoronDL(gLinkGoronRightForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_RHAND, getGoronDL(gLinkGoronRightHandClosedDL));

#undef SET_ENTRY_DL
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaGoron() {
    setupGoronFallbackModel();
}
