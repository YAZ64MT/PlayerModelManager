#ifndef __ZOBJUTILS_H__
#define __ZOBJUTILS_H__

#include "global.h"
#include "modding.h"

// Repoints single F3DZEX2 command to be relative to newBase.
//
// assumes zobj points to the start of the binary and that the target segmented addresses are relative to it.
RECOMP_IMPORT("mm_recomp_zobjutils", void ZobjUtils_repointGfxCommand(u8 zobj[], u32 commandOffset, u8 targetSegment, const void *newBase));

// Repoints all F3DZEX2 to be relative to newBase.
//
// assumes zobj points to the start of the binary and that the target segmented addresses are relative to it.
RECOMP_IMPORT("mm_recomp_zobjutils", void ZobjUtils_repointDisplayList(u8 zobj[], u32 displayListStartOffset, u8 targetSegment, const void *newBase));

// Repoints FlexSkeleton to be relative to be relative to newBase.
//
// assumes zobj points to the start of the binary and that segmented addresses are relative to it.
RECOMP_IMPORT("mm_recomp_zobjutils", void ZobjUtils_repointFlexSkeleton(u8 zobj[], u32 skeletonHeaderOffset, u8 targetSegment, const void *newBase));

// Finds index of FlexSkeletonHeader in binary.
//
// Returns -1 if no skeleton header can be found
RECOMP_IMPORT("mm_recomp_zobjutils", s32 ZobjUtils_getFlexSkeletonHeaderOffset(const u8 zobj[], u32 zobjSize));

#endif
