#ifndef __PLAYERMODELMANAGER_UTILS_H__
#define __PLAYERMODELMANAGER_UTILS_H__

#include "global.h"
#include "modding.h"
#include "playermodelmanager.h"

void clearLinkModelInfo(Link_ModelInfo *modelInfo);
Gfx *createShimWithMatrix(Mtx *matrix, u32 displayListCount, ...);
Gfx *createShimDisplayList(u32 displayListCount, ...);

u32 readU32(const u8 array[], u32 offset);
void writeU32(u8 array[], u32 offset, u32 value);
bool isSegmentedPtr(void *p);

#endif
