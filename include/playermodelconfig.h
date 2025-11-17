#ifndef __PlAYERMODELCONFIG_H__
#define __PlAYERMODELCONFIG_H__

#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "stdbool.h"

typedef unsigned long PlayerModelConfigDLHandle;
typedef unsigned long PlayerModelConfigMtxHandle;

PlayerModelConfigDLHandle PlayerModelConfig_addSingleDL(Gfx *defaultDL);

PlayerModelConfigMtxHandle PlayerModelConfig_addSingleMtx(Mtx *defaultMtx);

PlayerModelConfigDLHandle PlayerModelConfig_addShimDL(PlayerModelConfigDLHandle dls[], size_t n);

PlayerModelConfigDLHandle PLayerModelConfig_addDLWithMtx(PlayerModelConfigMtxHandle mtx, PlayerModelConfigDLHandle dl[], size_t n);

bool PlayerModelConfig_getEntries(PlayerModelConfigDLHandle **handlesOut, size_t *numOut);

#endif
