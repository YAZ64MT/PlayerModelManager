#include "global.h"
#include "playermodelmanager_api.h"
#include "globalobjects_api.h"
#include "apilocal.h"
#include "modelentrymanager.h"
#include "defaultfacetex.h"
#include "logger.h"

void FallbackModelsCommon_doCommonAssignments(PlayerModelManagerHandle h, FlexSkeletonHeader *skel, void *seg06, void *seg04) {
    ModelEntry *entry = ModelEntryManager_getEntry(h);

    if (!entry) {
        Logger_printWarning("Invalid PlayerModelManagerHandle passed in!");
        return;
    }

    GlobalObjectsSegmentMap segments = {0};
    segments[0x04] = seg04;
    segments[0x06] = seg06;

    GlobalObjects_globalizeLodLimbSkeleton(skel, seg06);

    for (int i = 0; i < skel->sh.limbCount; ++i) {
        LodLimb *curr = skel->sh.segment[i];

        if (curr->dLists[0]) {
            GlobalObjects_rebaseDL(curr->dLists[0], segments);
        }

        if (curr->dLists[1]) {
            GlobalObjects_rebaseDL(curr->dLists[1], segments);
        }
    }

    TexturePtr eyesTex[PLAYER_EYES_MAX];
    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        eyesTex[i] = SEGMENTED_TO_GLOBAL_PTR(seg06, gDefaultEyesTextures[i]);
    }
    PlayerModelManager_setEyesTextures(h, eyesTex);

    TexturePtr mouthTex[PLAYER_MOUTH_MAX];
    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        mouthTex[i] = SEGMENTED_TO_GLOBAL_PTR(seg06, gDefaultMouthTextures[i]);
    }
    PlayerModelManager_setMouthTextures(h, mouthTex);

    ModelEntryManager_setEntryHidden(entry, true);

    // First Person
    PlayerModelManager_setDisplayList(h, LINK_DL_OPT_FPS_LSHOULDER_SLINGSHOT, gEmptyDL);
    PlayerModelManager_setDisplayList(h, LINK_DL_OPT_FPS_LFOREARM_SLINGSHOT, gEmptyDL);
    PlayerModelManager_setDisplayList(h, LINK_DL_OPT_FPS_LHAND_SLINGSHOT, gEmptyDL);
}
