#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_boy/object_link_boy.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "modelentrymanager.h"
#include "modelinfo.h"
#include "modelentry.h"
#include "playermodelmanager_api.h"
#include "defaultfacetex.h"

ModelEntryForm *gFierceDeityModelEntry;
ModelInfo gFierceDeityModelInfo;

Gfx *getFierceDeityDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_BOY, dl);
}

static void setupFierceDeityFallbackModel() {
    void *fd = GlobalObjects_getGlobalObject(OBJECT_LINK_BOY);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(fd, &gLinkFierceDeitySkel);
    GlobalObjects_globalizeLodLimbSkeleton(fd, &gLinkFierceDeitySkel);

    GlobalObjectsSegmentMap segments = {0};
    segments[4] = GlobalObjects_getGlobalObject(GAMEPLAY_KEEP);
    segments[6] = fd;
    Utils_repointLodLimbSkelDLs(skel, segments);

    ModelInfo_init(&gFierceDeityModelInfo);

    ModelEntryForm *entryForm = gFierceDeityModelEntry = (ModelEntryForm *)CMEM_getEntry(CMEM_createMemoryHandle(PMM_MODEL_TYPE_FIERCE_DEITY, "__mm_object_link_boy__"));
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);
    ModelEntryForm_setSkeleton(entryForm, skel);
    ModelEntryForm_setDLsFromSkeletons(entryForm);

    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        ModelEntryForm_setEyesTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(fd, gDefaultEyesTextures[i]), i);
    }

    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        ModelEntryForm_setMouthTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(fd, gDefaultMouthTextures[i]), i);
    }

    CMEM_setEntryHidden(entry, true);
    ModelInfo_setModelEntryForm(&gFierceDeityModelInfo, entryForm);

#define SET_ENTRY_DL(id, dl) ModelEntry_setDisplayList(entry, id, dl)

    // hands
    SET_ENTRY_DL(LINK_DL_LHAND, getFierceDeityDL(gLinkFierceDeityLeftHandDL));
    SET_ENTRY_DL(LINK_DL_LFIST, getFierceDeityDL(gLinkFierceDeityLeftHandDL));
    SET_ENTRY_DL(LINK_DL_LHAND_BOTTLE, getFierceDeityDL(gLinkFierceDeityLeftHandDL));
    SET_ENTRY_DL(LINK_DL_LHAND_GUITAR, getFierceDeityDL(gLinkFierceDeityLeftHandDL));

    SET_ENTRY_DL(LINK_DL_RHAND, getFierceDeityDL(gLinkFierceDeityRightHandDL));
    SET_ENTRY_DL(LINK_DL_RFIST, getFierceDeityDL(gLinkFierceDeityRightHandDL));

    // First Person
    SET_ENTRY_DL(LINK_DL_FPS_LFOREARM, getFierceDeityDL(gLinkFierceDeityLeftForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_LHAND, getFierceDeityDL(gLinkFierceDeityLeftHandDL));
    SET_ENTRY_DL(LINK_DL_FPS_RFOREARM, getFierceDeityDL(gLinkFierceDeityRightForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_RHAND, getFierceDeityDL(gLinkFierceDeityRightHandDL));

#undef SET_ENTRY_DL
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaFierceDeity() {
    setupFierceDeityFallbackModel();
}
