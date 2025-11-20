#ifndef PLAYERMODELMANAGER_UTILS_H
#define PLAYERMODELMANAGER_UTILS_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "modding.h"
#include "stdbool.h"

Gfx *createShimWithMatrix(Mtx *matrix, int displayListCount, ...);
Gfx *createShimDisplayList(int displayListCount, ...);

u32 readU32(const u8 array[], u32 offset);
void writeU32(u8 array[], u32 offset, u32 value);
bool isSegmentedPtr(void *p);

void setCrashEnabled(bool isEnabled);
void tryCrashGame();

#endif
