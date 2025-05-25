#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "playermodelmanager_utils.h"
#include "defines_ooto.h"
#include "model_common.h"
#include "ml64compat.h"

#define OOTO_CHILD_SKEL_PTR 0x060053A8
#define OOTO_ADULT_SKEL_PTR 0x06005380

void OotoFixHeaderSkelPtr(u8 *zobj) {
    if (readU32(zobj, Z64O_SKELETON_HEADER_POINTER) == 0xFFFFFFFF) {
        switch (zobj[Z64O_FORM_BYTE]) {
        case OOTO_FORM_BYTE_CHILD:
            writeU32(zobj, Z64O_SKELETON_HEADER_POINTER, OOTO_CHILD_SKEL_PTR);
            break;

        case OOTO_FORM_BYTE_ADULT:
            writeU32(zobj, Z64O_SKELETON_HEADER_POINTER, OOTO_ADULT_SKEL_PTR);

        default:
            break;
        }
    }
}

void OotoFixChildLeftShoulder(u8 *zobj) {
    if (zobj[Z64O_FORM_BYTE] == OOTO_FORM_BYTE_CHILD) {
        u32 lShoulder = readU32(zobj, OOTO_CHILD_LUT_DL_LSHOULDER + 4);
        u32 lForearm = readU32(zobj, OOTO_CHILD_LUT_DL_LFOREARM + 4);

        if (lShoulder == lForearm) {
            u32 skelHeaderPtr = SEGMENT_OFFSET(readU32(zobj, Z64O_SKELETON_HEADER_POINTER));

            u32 firstLimbPtr = SEGMENT_OFFSET(readU32(zobj, skelHeaderPtr));

            u32 lShoulderLimbPtr = SEGMENT_OFFSET(readU32(zobj, firstLimbPtr + (PLAYER_LIMB_LEFT_SHOULDER - 1) * sizeof(u32)));

            LodLimb* limb = (LodLimb *)&zobj[lShoulderLimbPtr];

            writeU32(zobj, OOTO_CHILD_LUT_DL_LSHOULDER + 4, (u32)limb->dLists[0]);
        }
    }
}
