#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "playermodelmanager_utils.h"
#include "model_common.h"
#include "globalobjects_api.h"

Gfx *getGoronDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_GORON, dl);
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaGoron() {
    Link_FormProxy *formProxy = &gLinkFormProxies[PLAYER_FORM_GORON];

    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        eyesTex[i] = formProxy->vanilla.eyesTextures[i];
    }

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        mouthTex[i] = formProxy->vanilla.mouthTextures[i];
    }

    clearLinkModelInfo(&formProxy->vanilla);

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        formProxy->vanilla.eyesTextures[i] = eyesTex[i];
    }

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        formProxy->vanilla.mouthTextures[i] = mouthTex[i];
    }

    void *goron = GlobalObjects_getGlobalObject(OBJECT_LINK_GORON);

    GlobalObjects_globalizeLodLimbSkeleton(goron, &gLinkGoronSkel);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(goron, &gLinkGoronSkel);

    formProxy->vanilla.skeleton = skel;

    Gfx **models = formProxy->vanilla.models;

    // limbs
    setSkeletonDLsOnModelInfo(&formProxy->vanilla, skel);

    // hands
    models[LINK_DL_LHAND] = getGoronDL(gLinkGoronLeftHandOpenDL);
    models[LINK_DL_LFIST] = getGoronDL(gLinkGoronLeftHandClosedDL);
    models[LINK_DL_LHAND_BOTTLE] = getGoronDL(gLinkGoronLeftHandHoldBottleDL);
    models[LINK_DL_RHAND] = getGoronDL(gLinkGoronRightHandOpenDL);
    models[LINK_DL_RFIST] = getGoronDL(gLinkGoronRightHandClosedDL);

    // First Person
    models[LINK_DL_FPS_LFOREARM] = getGoronDL(gLinkGoronLeftForearmDL);
    models[LINK_DL_FPS_LHAND] = getGoronDL(gLinkGoronLeftHandClosedDL);
    models[LINK_DL_FPS_RFOREARM] = getGoronDL(gLinkGoronRightForearmDL);
    models[LINK_DL_FPS_RHAND] = getGoronDL(gLinkGoronRightHandClosedDL);
}
