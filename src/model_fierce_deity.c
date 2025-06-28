#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_boy/object_link_boy.h"
#include "playermodelmanager_utils.h"
#include "model_common.h"
#include "globalobjects_api.h"

Gfx *getFierceDeityDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_BOY, dl);
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaFierceDeity() {
    Link_FormProxy *formProxy = &gLinkFormProxies[PLAYER_FORM_FIERCE_DEITY];

    clearModelInfoKeepEyes(&formProxy->vanilla);

    void *fd = GlobalObjects_getGlobalObject(OBJECT_LINK_BOY);

    GlobalObjects_globalizeLodLimbSkeleton(fd, &gLinkFierceDeitySkel);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(fd, &gLinkFierceDeitySkel);

    formProxy->vanilla.skeleton = skel;

    Gfx **models = formProxy->vanilla.models;

    // limbs
    setSkeletonDLsOnModelInfo(&formProxy->vanilla, skel);

    // hands
    models[LINK_DL_LHAND] = getFierceDeityDL(gLinkFierceDeityLeftHandDL);
    models[LINK_DL_LFIST] = getFierceDeityDL(gLinkFierceDeityLeftHandDL);
    models[LINK_DL_LHAND_BOTTLE] = getFierceDeityDL(gLinkFierceDeityLeftHandHoldBottleDL);
    models[LINK_DL_LHAND_GUITAR] = getFierceDeityDL(gLinkFierceDeityLeftHandDL);
    models[LINK_DL_RHAND] = getFierceDeityDL(gLinkFierceDeityRightHandDL);
    models[LINK_DL_RFIST] = getFierceDeityDL(gLinkFierceDeityRightHandDL);

    // First Person
    models[LINK_DL_FPS_LFOREARM] = getFierceDeityDL(gLinkFierceDeityLeftForearmDL);
    models[LINK_DL_FPS_LHAND] = getFierceDeityDL(gLinkFierceDeityLeftHandDL);
    models[LINK_DL_FPS_RFOREARM] = getFierceDeityDL(gLinkFierceDeityRightForearmDL);
    models[LINK_DL_FPS_RHAND] = getFierceDeityDL(gLinkFierceDeityRightHandDL);
}
