#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "recompdata.h"
#include "playermodelmanager_utils.h"
#include "defines_ooto.h"
#include "model_common.h"
#include "ml64compat.h"
#include "globalobjects_api.h"

#define OOTO_CHILD_SKEL_PTR 0x060053A8
#define OOTO_ADULT_SKEL_PTR 0x06005380

void OotoFixHeaderSkelPtr(void *obj) {
    u8 *zobj = obj;

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

void OotoFixChildLeftShoulder(void *obj) {
    u8 *zobj = obj;

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

U32ValueHashmapHandle gSegmentPointersRemap;

// remap specific pointers
// useful for gameplay keep textures
void remapPtrs(void *obj, Gfx *dlPtr, U32ValueHashmapHandle oldPtrs2New) {
    if (isSegmentedPtr(obj)) {
        recomp_printf("remapPtrs: Incorrectly passed in non-global pointer 0x%X as zobj\n", obj);
        return;
    }

    if (!isSegmentedPtr(dlPtr)) {
        recomp_printf("remapPtrs: Incorrectly passed in global pointer 0x%X as dlPtr\n", obj);
        return;
    }

    unsigned targetSegment = SEGMENT_NUMBER(dlPtr);

    Gfx *globalPtr = SEGMENTED_TO_GLOBAL_PTR(obj, dlPtr);

    unsigned opcode;

    unsigned currentSegment;

    bool isEndDl = false;

    while (!isEndDl) {
        opcode = globalPtr->words.w0 >> 24;

        currentSegment = SEGMENT_NUMBER(globalPtr->words.w1);

        switch (opcode) {
            case G_ENDDL:
                isEndDl = true;
                break;

            case G_DL:
                if (currentSegment == targetSegment) {
                    remapPtrs(obj, (Gfx *)globalPtr->words.w1, oldPtrs2New);
                }

                if ((globalPtr->words.w0 >> 16 & 0xFF) == G_DL_NOPUSH) {
                    isEndDl = true;
                }
                // FALL THROUGH
            case G_VTX:
            case G_MTX:
            case G_SETTIMG:
            case G_MOVEMEM:
                recomputil_u32_value_hashmap_get(oldPtrs2New, globalPtr->words.w1, &globalPtr->words.w1);
                break;

            default:
                break;
        }

        globalPtr = (Gfx *)((uintptr_t)globalPtr + sizeof(Gfx));
    }
}

void applySegmentPtrRemaps(void *obj, Gfx *dl) {
    remapPtrs(obj, dl, gSegmentPointersRemap);
}

void setSegmentPtrRemap(u32 key, u32 value) {
    recomputil_u32_value_hashmap_insert(gSegmentPointersRemap, key, value);
}

void initSegmentPtrsRemap() {
    gSegmentPointersRemap = recomputil_create_u32_value_hashmap();
}

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onReadyML64CompatBase());

RECOMP_CALLBACK(".", PlayerModelManager_internal_onReadyFormProxies)
void initML64CompatBase_onReadyFormProxies() {
    initSegmentPtrsRemap();
    PlayerModelManager_internal_onReadyML64CompatBase();
}
