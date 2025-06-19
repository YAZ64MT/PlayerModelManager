#include "global.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "playermodelmanager_utils.h"
#include "model_common.h"
#include "globalobjects_api.h"

Gfx *getZoraDL(Gfx *dl);

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void setupVanillaZora() {
    Link_FormProxy *formProxy = &gLinkFormProxies[PLAYER_FORM_ZORA];

    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];

    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        eyesTex[i] = formProxy->vanilla.eyesTextures[i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        mouthTex[i] = formProxy->vanilla.mouthTextures[i];
    }

    clearLinkModelInfo(&formProxy->vanilla);

    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        formProxy->vanilla.eyesTextures[i] = eyesTex[i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        formProxy->vanilla.mouthTextures[i] = mouthTex[i];
    }

    void *zora = GlobalObjects_getGlobalObject(OBJECT_LINK_ZORA);

    GlobalObjects_globalizeLodLimbSkeleton(zora, &gLinkZoraSkel);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(zora, &gLinkZoraSkel);

    formProxy->vanilla.skeleton = skel;

    Gfx **models = formProxy->vanilla.models;

    // limbs
    setSkeletonDLsOnModelInfo(&formProxy->vanilla, skel);

    // hands
    models[LINK_DL_LHAND] = getZoraDL(gLinkZoraLeftHandOpenDL);
    models[LINK_DL_LFIST] = getZoraDL(gLinkZoraLeftHandClosedDL);
    models[LINK_DL_LHAND_BOTTLE] = getZoraDL(gLinkZoraLeftHandHoldBottleDL);
    models[LINK_DL_LHAND_GUITAR] = getZoraDL(object_link_zora_DL_00E088);
    models[LINK_DL_RHAND] = getZoraDL(gLinkZoraRightHandOpenDL);
    models[LINK_DL_RFIST] = getZoraDL(gLinkZoraRightHandClosedDL);

    // First Person
    models[LINK_DL_FPS_LFOREARM] = getZoraDL(gLinkZoraLeftForearmDL);
    models[LINK_DL_FPS_LHAND] = getZoraDL(gLinkZoraLeftHandClosedDL);
    models[LINK_DL_FPS_RFOREARM] = getZoraDL(gLinkZoraRightForearmDL);
    models[LINK_DL_FPS_RHAND] = getZoraDL(gLinkZoraRightHandClosedDL);
}
