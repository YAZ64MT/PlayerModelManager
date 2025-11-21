#include "global.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "modelentrymanager.h"
#include "modelinfo.h"
#include "modelentry.h"
#include "playermodelmanager_api.h"
#include "defaultfacetex.h"

ModelEntryForm *gZoraModelEntry;
ModelInfo gZoraModelInfo;

Gfx *getZoraDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_ZORA, dl);
}

static void setupZoraFallbackModel() {
    void *zora = GlobalObjects_getGlobalObject(OBJECT_LINK_ZORA);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(zora, &gLinkZoraSkel);
    GlobalObjects_globalizeLodLimbSkeleton(zora, &gLinkZoraSkel);

    GlobalObjectsSegmentMap segments = {0};
    segments[4] = GlobalObjects_getGlobalObject(GAMEPLAY_KEEP);
    segments[6] = zora;
    Utils_repointLodLimbSkelDLs(skel, segments);

    ModelInfo_init(&gZoraModelInfo);

    ModelEntryForm *entryForm = gZoraModelEntry = (ModelEntryForm *)CMEM_getEntry(CMEM_createMemoryHandle(PMM_MODEL_TYPE_ZORA, "__mm_object_link_zora__"));
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);
    ModelEntryForm_setSkeleton(entryForm, skel);
    ModelEntryForm_setDLsFromSkeletons(entryForm);

    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        ModelEntryForm_setEyesTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(zora, gDefaultEyesTextures[i]), i);
    }

    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        ModelEntryForm_setMouthTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(zora, gDefaultMouthTextures[i]), i);
    }

    CMEM_setEntryHidden(entry, true);
    ModelInfo_setModelEntryForm(&gZoraModelInfo, entryForm);

#define SET_ENTRY_DL(id, dl) ModelEntry_setDisplayList(entry, id, dl)

    // hands
    SET_ENTRY_DL(LINK_DL_LHAND, getZoraDL(gLinkZoraLeftHandOpenDL));
    SET_ENTRY_DL(LINK_DL_LFIST, getZoraDL(gLinkZoraLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_LHAND_BOTTLE, getZoraDL(gLinkZoraLeftHandHoldBottleDL));
    SET_ENTRY_DL(LINK_DL_LHAND_GUITAR, getZoraDL(object_link_zora_DL_00E088));

    SET_ENTRY_DL(LINK_DL_RHAND, getZoraDL(gLinkZoraRightHandOpenDL));
    SET_ENTRY_DL(LINK_DL_RFIST, getZoraDL(gLinkZoraRightHandClosedDL));

    // First Person
    SET_ENTRY_DL(LINK_DL_FPS_LFOREARM, getZoraDL(gLinkZoraLeftForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_LHAND, getZoraDL(gLinkZoraLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_FPS_RFOREARM, getZoraDL(gLinkZoraRightForearmDL));
    SET_ENTRY_DL(LINK_DL_FPS_RHAND, getZoraDL(gLinkZoraRightHandClosedDL));

#undef SET_ENTRY_DL
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaZora() {
    setupZoraFallbackModel();
}
