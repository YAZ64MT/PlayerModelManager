#ifndef __PLAYERMODELMANAGER_UTILS_H__
#define __PLAYERMODELMANAGER_UTILS_H__

#include "global.h"
#include "modding.h"
#include "playermodelmanager.h"

void clearLinkModelInfo(Link_ModelInfo *modelInfo);
void clearModelInfoKeepEyes(Link_ModelInfo *info);

Gfx *createShimWithMatrix(Mtx *matrix, int displayListCount, ...);
Gfx *createShimDisplayList(int displayListCount, ...);

u32 readU32(const u8 array[], u32 offset);
void writeU32(u8 array[], u32 offset, u32 value);
bool isSegmentedPtr(void *p);

#endif
