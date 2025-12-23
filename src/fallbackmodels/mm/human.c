#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "modelinfo.h"
#include "modelentry.h"
#include "modelentrymanager.h"
#include "modelmatrixids.h"
#include "playermodelmanager_api.h"
#include "defaultfacetex.h"
#include "customdls.h"

ModelEntryForm *gHumanModelEntry;
ModelInfo gHumanModelInfo;

Gfx *getHumanDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, dl);
}

static void setupHumanFallbackModel() {
    void *human = GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSkel);
    GlobalObjects_globalizeLodLimbSkeleton(human, &gLinkHumanSkel);

    ModelInfo_init(&gHumanModelInfo);

    ModelEntryForm *entryForm = gHumanModelEntry = (ModelEntryForm *)CMEM_getEntry(CMEM_createMemoryHandle(PMM_MODEL_TYPE_CHILD, "__mm_object_link_child__"));
    ModelEntry *entry = ModelEntryForm_getModelEntry(entryForm);
    ModelEntryForm_setSkeleton(entryForm, skel);

    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        ModelEntryForm_setEyesTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(human, gDefaultEyesTextures[i]), i);
    }

    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        ModelEntryForm_setMouthTexture(entryForm, (TexturePtr)SEGMENTED_TO_GLOBAL_PTR(human, gDefaultMouthTextures[i]), i);
    }

    CMEM_setEntryHidden(entry, true);
    ModelInfo_setModelEntryForm(&gHumanModelInfo, entryForm);

#define SET_ENTRY_DL(id, dl) ModelEntry_setDisplayList(entry, id, dl)

    // Body
    SET_ENTRY_DL(LINK_DL_WAIST, getHumanDL(gLinkHumanWaistDL));
    SET_ENTRY_DL(LINK_DL_RTHIGH, getHumanDL(gLinkHumanRightThighDL));
    SET_ENTRY_DL(LINK_DL_RSHIN, getHumanDL(gLinkHumanRightShinDL));
    SET_ENTRY_DL(LINK_DL_RFOOT, getHumanDL(gLinkHumanRightFootDL));
    SET_ENTRY_DL(LINK_DL_LTHIGH, getHumanDL(gLinkHumanLeftThighDL));
    SET_ENTRY_DL(LINK_DL_LSHIN, getHumanDL(gLinkHumanLeftShinDL));
    SET_ENTRY_DL(LINK_DL_LFOOT, getHumanDL(gLinkHumanLeftFootDL));
    SET_ENTRY_DL(LINK_DL_HEAD, getHumanDL(gLinkHumanHeadDL));
    SET_ENTRY_DL(LINK_DL_HAT, getHumanDL(gLinkHumanHatDL));
    SET_ENTRY_DL(LINK_DL_COLLAR, getHumanDL(gLinkHumanCollarDL));
    SET_ENTRY_DL(LINK_DL_LSHOULDER, getHumanDL(gLinkHumanLeftShoulderDL));
    SET_ENTRY_DL(LINK_DL_LFOREARM, getHumanDL(gLinkHumanLeftForearmDL));
    SET_ENTRY_DL(LINK_DL_LHAND, getHumanDL(gLinkHumanLeftHandOpenDL));
    SET_ENTRY_DL(LINK_DL_RSHOULDER, getHumanDL(gLinkHumanRightShoulderDL));
    SET_ENTRY_DL(LINK_DL_RFOREARM, getHumanDL(gLinkHumanRightForearmDL));
    SET_ENTRY_DL(LINK_DL_RHAND, getHumanDL(gLinkHumanRightHandOpenDL));
    SET_ENTRY_DL(LINK_DL_SHEATH_NONE, getHumanDL(gLinkHumanSheathedKokiriSwordDL));
    SET_ENTRY_DL(LINK_DL_TORSO, getHumanDL(gLinkHumanTorsoDL));

    // hands
    SET_ENTRY_DL(LINK_DL_LFIST, getHumanDL(gLinkHumanLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_LHAND_BOTTLE, getHumanDL(gLinkHumanLeftHandHoldBottleDL));
    SET_ENTRY_DL(LINK_DL_LHAND_GUITAR, getHumanDL(gLinkHumanLeftHandOpenDL));

    SET_ENTRY_DL(LINK_DL_RFIST, getHumanDL(gLinkHumanRightHandClosedDL));

    SET_ENTRY_DL(LINK_DL_OPT_RHAND_OCARINA, gLinkHumanRightHandOcarinaDL);

    // First Person
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LFOREARM, getHumanDL(gLinkHumanLeftForearmDL));
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LHAND, getHumanDL(gLinkHumanLeftHandClosedDL));
    SET_ENTRY_DL(LINK_DL_OPT_FPS_RFOREARM, gEmptyDL);
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LSHOULDER_SLINGSHOT, gEmptyDL);
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LFOREARM_SLINGSHOT, gEmptyDL);
    SET_ENTRY_DL(LINK_DL_OPT_FPS_LHAND_SLINGSHOT, gEmptyDL);

    SET_ENTRY_DL(LINK_DL_OPT_FPS_RHAND, gLinkHumanFirstPersonArmDL);

    // items
    SET_ENTRY_DL(LINK_DL_OCARINA_TIME, gLinkHumanOcarinaDL);

#undef SET_ENTRY_DL
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaHuman() {
    setupHumanFallbackModel();
}
