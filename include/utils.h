#ifndef UTILS_H
#define UTILS_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "z64animation.h"
#include "stdbool.h"
#include "globalobjects_api.h"

Gfx *createShimWithMatrix(Mtx *matrix, int displayListCount, ...);
Gfx *createShimDisplayList(int displayListCount, ...);

u32 readU32(const u8 array[], u32 offset);
void writeU32(u8 array[], u32 offset, u32 value);
bool isSegmentedPtr(void *p);

void setCrashEnabled(bool isEnabled);
void tryCrashGame();

void repointLodLimbSkelDLs(FlexSkeletonHeader *skel, GlobalObjectsSegmentMap segments);
void repointStandardLimbSkelDLs(FlexSkeletonHeader *skel, GlobalObjectsSegmentMap segments);

#endif
