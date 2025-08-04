#ifndef __EQUIPMENTREPLACEMENTS_H__
#define __EQUIPMENTREPLACEMENTS_H__

#include "modelmatrixids.h"
#include "ultra64.h"

typedef struct {
    Link_DisplayList target;
    Gfx *replacement;
} Link_DisplayListOverride;

typedef struct {
    size_t numOverrides;
    Link_DisplayListOverride* overrides;
} Link_EquipmentReplacement;

#endif
