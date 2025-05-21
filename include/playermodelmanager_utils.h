#ifndef __PLAYERMODELMANAGER_UTILS_H__
#define __PLAYERMODELMANAGER_UTILS_H__

#include "global.h"
#include "modding.h"
#include "playermodelmanager.h"

void clearLinkModelInfo(Link_ModelInfo *modelInfo);
Gfx *createShimWithMatrix(Mtx *matrix, u32 displayListCount, ...);
Gfx *createShimDisplayList(u32 displayListCount, ...);

#endif
