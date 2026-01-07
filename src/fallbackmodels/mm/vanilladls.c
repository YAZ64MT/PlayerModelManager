#include "global.h"
#include "mm/vanillawrapperdls.h"
#include "globalobjects_api.h"
#include "assets/objects/object_link_boy/object_link_boy.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_mir_ray/object_mir_ray.h"
#include "maskdls.h"
#include "playerexterns.h"

#define DECLARE_MM_DL_CFG(objectId, segmentedDL, wrapperDLName) Gfx wrapperDLName[] = { gsSPEndDisplayList() };
#include "mm/dlconfig.h"
#undef DECLARE_MM_DL_CFG

typedef struct VanillaInitDLInfo {
    ObjectId objectId;
    Gfx *vanillaDL;
    Gfx *callDL;
} VanillaInitDLInfo;

#define DECLARE_MM_DL_CFG(objId, segmentedDL, wrapperDLName) {.callDL = wrapperDLName, .objectId = objId, .vanillaDL = segmentedDL},
static VanillaInitDLInfo sVanillaMMDLInits[] = {
#include "mm/dlconfig.h"
};

static Gfx *getRepointedDL(ObjectId id, Gfx *segmentedDL) {
    return GlobalObjects_getGlobalGfxPtr(id, segmentedDL);
}

void initVanillaMMDLs() {
    for (int i = 0; i < ARRAY_COUNT(sVanillaMMDLInits); ++i) {
        VanillaInitDLInfo *curr = &sVanillaMMDLInits[i];

        
        if (curr->objectId && curr->vanillaDL) {
            Gfx *repointedDL = getRepointedDL(curr->objectId, curr->vanillaDL);

            if (repointedDL) {
                gSPBranchList(curr->callDL, getRepointedDL(curr->objectId, curr->vanillaDL));
            }
        }
    }
}
