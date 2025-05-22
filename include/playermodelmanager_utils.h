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
void repointF3DCommand(u8 zobj[], u32 commandOffset, u8 targetSegment);
void repointDisplayList(u8 zobj[], u32 displayListStartOffset, u8 targetSegment);
void repointSkeleton(u8 zobj[], u32 headerOffset, u32 targetSegment);

#endif
