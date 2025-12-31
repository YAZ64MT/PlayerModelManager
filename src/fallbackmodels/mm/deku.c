#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "modelentrymanager.h"
#include "modelinfo.h"
#include "modelentry.h"
#include "playermodelmanager_api.h"
#include "defaultfacetex.h"

PlayerModelManagerHandle gDekuModelHandle;
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

    gDekuModelHandle = ModelEntryManager_createMemoryHandle(PMM_MODEL_TYPE_DEKU, "__mm_object_link_nuts__");

    ModelEntryForm *entryForm = gDekuModelEntry = (ModelEntryForm *)ModelEntryManager_getEntry(gDekuModelHandle);
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);
    ModelEntryForm_setSkeleton(entryForm, skel);

    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        ModelEntryForm_setEyesTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(deku, gDefaultEyesTextures[i]), i);
    }

    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        ModelEntryForm_setMouthTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(deku, gDefaultMouthTextures[i]), i);
    }

    ModelEntryManager_setEntryHidden(entry, true);
    ModelInfo_setModelEntryForm(&gDekuModelInfo, entryForm);

#define SET_ENTRY_DL(id, dl) ModelEntry_setDisplayList(entry, id, dl)

    // Body
    SET_ENTRY_DL(LINK_DL_WAIST, getDekuDL(gLinkDekuWaistDL));
    SET_ENTRY_DL(LINK_DL_RTHIGH, getDekuDL(gLinkDekuRightThighDL));
    SET_ENTRY_DL(LINK_DL_RSHIN, getDekuDL(gLinkDekuRightShinDL));
    SET_ENTRY_DL(LINK_DL_RFOOT, getDekuDL(gLinkDekuRightFootDL));
    SET_ENTRY_DL(LINK_DL_LTHIGH, getDekuDL(gLinkDekuLeftThighDL));
    SET_ENTRY_DL(LINK_DL_LSHIN, getDekuDL(gLinkDekuLeftShinDL));
    SET_ENTRY_DL(LINK_DL_LFOOT, getDekuDL(gLinkDekuLeftFootDL));
    SET_ENTRY_DL(LINK_DL_HEAD, getDekuDL(gLinkDekuHeadDL));
    SET_ENTRY_DL(LINK_DL_HAT, getDekuDL(gLinkDekuHatDL));
    SET_ENTRY_DL(LINK_DL_COLLAR, getDekuDL(gLinkDekuCollarDL));
    SET_ENTRY_DL(LINK_DL_LSHOULDER, getDekuDL(gLinkDekuLeftShoulderDL));
    SET_ENTRY_DL(LINK_DL_LFOREARM, getDekuDL(gLinkDekuLeftForearmDL));
    SET_ENTRY_DL(LINK_DL_LHAND, getDekuDL(gLinkDekuLeftHandDL));
    SET_ENTRY_DL(LINK_DL_RSHOULDER, getDekuDL(gLinkDekuRightShoulderDL));
    SET_ENTRY_DL(LINK_DL_RFOREARM, getDekuDL(gLinkDekuRightForearmDL));
    SET_ENTRY_DL(LINK_DL_RHAND, getDekuDL(gLinkDekuRightHandDL));
    SET_ENTRY_DL(LINK_DL_SHEATH_NONE, gEmptyDL);
    SET_ENTRY_DL(LINK_DL_TORSO, getDekuDL(gLinkDekuTorsoDL));

    // First Person
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LSHOULDER_SLINGSHOT, gEmptyDL);
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LFOREARM_SLINGSHOT, gEmptyDL);
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LHAND_SLINGSHOT, gEmptyDL);

#undef SET_ENTRY_DL
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaDeku() {
    setupDekuFallbackModel();
}
