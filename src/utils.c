#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "libc/stdarg.h"
#include "playermodelmanager.h"
#include "playermodelmanager_utils.h"
#include "libc/string.h"

void clearLinkModelInfo(Link_ModelInfo* modelInfo) {
    memset(modelInfo, 0, sizeof(Link_ModelInfo));
}

Gfx *createShimDisplayList(u32 displayListCount, ...) {
    Gfx *shimDl = recomp_alloc(sizeof(Gfx) * displayListCount);

    va_list args;
    va_start(args, displayListCount);

    for (u32 i = 0; i < displayListCount - 1; ++i) {
        Gfx *dlToCall = va_arg(args, Gfx *);

        gSPDisplayList(&shimDl[i], dlToCall);
    }

    Gfx *lastDl = va_arg(args, Gfx *);

    gSPBranchList(&shimDl[displayListCount - 1], lastDl);

    va_end(args);

    return shimDl;
}

Gfx *createShimWithMatrix(Mtx* matrix, u32 displayListCount, ...) {
    u32 shimSize = displayListCount + 3;

    Gfx *shimDl = recomp_alloc(sizeof(Gfx) * (shimSize));

    gSPMatrix(shimDl, matrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    va_list args;
    va_start(args, displayListCount);

    Gfx *calledLists = shimDl + 1;
    for (u32 i = 0; i < displayListCount; ++i) {
        Gfx *dlToCall = va_arg(args, Gfx *);

        gSPDisplayList(&calledLists[i], dlToCall);
    }

    va_end(args);

    gSPPopMatrix(&shimDl[shimSize - 2], G_MTX_MODELVIEW);

    gSPEndDisplayList(&shimDl[shimSize - 1]);

    return shimDl;
}

u32 readU32(const u8 array[], u32 offset) {
    return (u32)(array[offset + 0]) << 24 |
           (u32)(array[offset + 1]) << 16 |
           (u32)(array[offset + 2]) << 8 |
           (u32)(array[offset + 3]);
}

void writeU32(u8 array[], u32 offset, u32 value) {
    array[offset + 0] = (value & 0xFF000000) >> 24;
    array[offset + 1] = (value & 0x00FF0000) >> 16;
    array[offset + 2] = (value & 0x0000FF00) >> 8;
    array[offset + 3] = (value & 0x000000FF);
}

// Repoints F3DZEX2 command to be relative to object's location in RAM
void repointF3DCommand(u8 zobj[], u32 commandOffset, u8 targetSegment) {
    u32 opcode = zobj[commandOffset];
    u32 segmentedAddrOffset = commandOffset + 4;
    u8 segment = zobj[segmentedAddrOffset];
    u32 segmentedDataOffset = readU32(zobj, segmentedAddrOffset);
    u32 dataOffset = SEGMENT_OFFSET(segmentedDataOffset);
    u32 repointedOffset;
    switch (opcode) {
    case G_DL:
        if (segment == targetSegment) {
            repointDisplayList(zobj, dataOffset, targetSegment);
        }
    case G_VTX:
    case G_MTX:
    case G_SETTIMG:
        repointedOffset = (u32)(zobj + dataOffset);
        if (segment == targetSegment) {
            writeU32(zobj, segmentedAddrOffset, repointedOffset);
            recomp_printf("Repointing 0x0%x -> 0x%x\n", segmentedDataOffset, repointedOffset);
        }
        break;

    default:
        break;
    }
}

void repointDisplayList(u8 zobj[], u32 displayListStartOffset, u8 targetSegment) {
    u32 offset = displayListStartOffset;
    u8 segment;

    u8 opcode;
    bool isEndDl = false;

    while (!isEndDl) {
        opcode = zobj[offset];

        switch (opcode) {
        case G_ENDDL:
            isEndDl = true;
            break;

        case G_DL:
            if (zobj[offset + 1] == G_DL_NOPUSH) {
                isEndDl = true;
            }
        case G_VTX:
        case G_MTX:
        case G_SETTIMG:
            segment = zobj[offset + 4];
            if (segment == targetSegment) {
                repointF3DCommand(zobj, offset, targetSegment);
            }
            break;

        default:
            break;
        }

        offset += 8;
    }
}

void repointSkeleton(u8 zobj[], u32 skeletonHeaderOffset, u32 targetSegment) {

    // repoint only if segmented
    if (zobj[skeletonHeaderOffset] == targetSegment) {

        u32 firstLimbOffset = SEGMENT_OFFSET(readU32(zobj, skeletonHeaderOffset));
        writeU32(zobj, skeletonHeaderOffset, firstLimbOffset + (u32)zobj);
        FlexSkeletonHeader *flexHeader = (FlexSkeletonHeader *)(&zobj[skeletonHeaderOffset]);

        Gfx *repointedDisplayList;

        LodLimb **limbs = (LodLimb **)(flexHeader->sh.segment);

        recomp_printf("Limb count: %d\n", flexHeader->sh.limbCount);
        recomp_printf("First limb entry location: 0x%x\n", limbs);

        for (u8 i = 0; i < flexHeader->sh.limbCount; i++) {
            limbs[i] = (LodLimb *)&zobj[SEGMENT_OFFSET(limbs[i])];
            if (limbs[i]->dLists[0]) {
                repointedDisplayList = ((Gfx *)(&zobj[SEGMENT_OFFSET(limbs[i]->dLists[0])]));
                limbs[i]->dLists[0] = repointedDisplayList;
                limbs[i]->dLists[1] = repointedDisplayList;
            }
        }
    }
}
