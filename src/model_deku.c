#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "playermodelmanager_utils.h"
#include "model_common.h"
#include "globalobjects_api.h"

Gfx *getDekuDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_NUTS, dl);
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaDeku() {
    Link_FormProxy *formProxy = &gLinkFormProxies[PLAYER_FORM_DEKU];

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

    void *zora = GlobalObjects_getGlobalObject(OBJECT_LINK_ZORA);

    GlobalObjects_globalizeLodLimbSkeleton(zora, &gLinkDekuSkel);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(zora, &gLinkDekuSkel);

    formProxy->vanilla.skeleton = skel;

    Gfx **models = formProxy->vanilla.models;

    // limbs
    setSkeletonDLsOnModelInfo(&formProxy->vanilla, skel);

    // hands
    models[LINK_DL_LHAND] = getDekuDL(gLinkDekuLeftHandDL);
    models[LINK_DL_LFIST] = getDekuDL(gLinkDekuLeftHandDL);
    models[LINK_DL_LHAND_BOTTLE] = getDekuDL(gLinkDekuLeftHandDL);
    models[LINK_DL_RHAND] = getDekuDL(gLinkDekuRightHandDL);
    models[LINK_DL_RFIST] = getDekuDL(gLinkDekuRightHandDL);

    // First Person
    models[LINK_DL_FPS_LFOREARM] = getDekuDL(gLinkDekuLeftForearmDL);
    models[LINK_DL_FPS_LHAND] = getDekuDL(gLinkDekuLeftHandDL);
    models[LINK_DL_FPS_RFOREARM] = getDekuDL(gLinkDekuRightForearmDL);
    models[LINK_DL_FPS_RHAND] = getDekuDL(gLinkDekuRightHandDL);
}
