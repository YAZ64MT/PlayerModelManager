#ifndef UTILS_H
#define UTILS_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "z64animation.h"
#include "stdbool.h"
#include "globalobjects_api.h"

Gfx *Utils_createShimWithMatrix(Mtx *matrix, int displayListCount, ...);
Gfx *Utils_createShimDisplayList(int displayListCount, ...);

u32 Utils_readU32(const u8 array[], u32 offset);
void Utils_writeU32(u8 array[], u32 offset, u32 value);
bool Utils_isSegmentedPtr(void *p);

void Utils_setCrashEnabled(bool isEnabled);
void Utils_tryCrashGame();

#endif
