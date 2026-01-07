#include "modding.h"
#include "global.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "utils.h"
#include "globalobjects_api.h"
#include "playermodelmanager_api.h"
#include "defaultfacetex.h"
#include "apilocal.h"
#include "fallbackmodels.h"

PlayerModelManagerHandle gDekuModelHandle;

Gfx *getDekuDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_NUTS, dl);
}

void registerDeku() {
    void *deku = GlobalObjects_getGlobalObject(OBJECT_LINK_NUTS);

    FlexSkeletonHeader *skel = SEGMENTED_TO_GLOBAL_PTR(deku, &gLinkDekuSkel);

    gDekuModelHandle = PlayerModelManager_registerModel(PMM_API_VERSION, "__mm_object_link_nuts__", PMM_MODEL_TYPE_DEKU);

    FallbackModelsCommon_doCommonAssignments(gDekuModelHandle, skel, deku, GlobalObjects_getGlobalObject(GAMEPLAY_KEEP));

    FallbackModelsCommon_addEquipmentChildMM(gDekuModelHandle);
}
