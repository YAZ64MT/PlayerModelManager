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

    GlobalObjects_globalizeStandardLimbSkeleton(goron, &gLinkGoronShieldingSkel);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(goron, &gLinkGoronSkel);

    formProxy->vanilla.skeleton = skel;

    formProxy->vanilla.shieldingSkeleton = SEGMENTED_TO_GLOBAL_PTR(goron, &gLinkGoronShieldingSkel);

    Gfx **models = formProxy->vanilla.models;

    // limbs
    setSkeletonDLsOnModelInfo(&formProxy->vanilla, skel);

    // hands
    models[LINK_DL_LHAND] = getGoronDL(gLinkGoronLeftHandOpenDL);
    models[LINK_DL_LFIST] = getGoronDL(gLinkGoronLeftHandClosedDL);
    models[LINK_DL_LHAND_BOTTLE] = getGoronDL(gLinkGoronLeftHandHoldBottleDL);
    models[LINK_DL_LHAND_GUITAR] = getGoronDL(gLinkGoronLeftHandOpenDL);
    models[LINK_DL_RHAND] = getGoronDL(gLinkGoronRightHandOpenDL);
    models[LINK_DL_RFIST] = getGoronDL(gLinkGoronRightHandClosedDL);

    // First Person
    models[LINK_DL_FPS_LFOREARM] = getGoronDL(gLinkGoronLeftForearmDL);
    models[LINK_DL_FPS_LHAND] = getGoronDL(gLinkGoronLeftHandClosedDL);
    models[LINK_DL_FPS_RFOREARM] = getGoronDL(gLinkGoronRightForearmDL);
    models[LINK_DL_FPS_RHAND] = getGoronDL(gLinkGoronRightHandClosedDL);
}

static FlexSkeletonHeader *sShieldingGoronSkel = NULL;

RECOMP_HOOK("SkelAnime_InitFlex")
void patchSecondGoronSkel_on_SkelAnime_InitFlex(PlayState *play, SkelAnime *skelAnime, FlexSkeletonHeader *skeletonHeaderSeg, AnimationHeader *animation, Vec3s *jointTable, Vec3s *morphTable, s32 limbCount) {
    sShieldingGoronSkel = NULL;
    
    if (skeletonHeaderSeg == &gLinkGoronShieldingSkel) {
        FlexSkeletonHeader *skel = Lib_SegmentedToVirtual(skeletonHeaderSeg);
    }
}

RECOMP_HOOK_RETURN("SkelAnime_InitFlex")
void patchSecondGoronSkel_on_return_SkelAnime_InitFlex() {
    if (sShieldingGoronSkel) {
        Link_ShieldingSkeletonProxy *skelProxy = &gLinkFormProxies[PLAYER_FORM_GORON].shieldingSkeleton;

        if (skelProxy) {
            sShieldingGoronSkel->sh.segment = skelProxy->flexSkeleton.sh.segment;
            sShieldingGoronSkel->sh.limbCount = skelProxy->flexSkeleton.sh.limbCount;
            sShieldingGoronSkel->dListCount = skelProxy->flexSkeleton.dListCount;
        }
    }
    
    sShieldingGoronSkel = NULL;
}