#ifndef __EQUIPMENTREPLACEMENTS_H__
#define __EQUIPMENTREPLACEMENTS_H__

#include "modelmatrixids.h"
#include "ultra64.h"

typedef struct {
    Link_DisplayList target;
    Gfx *replacement;
} Link_DisplayListOverride;

typedef struct {
    Link_EquipmentMatrix target;
    Mtx *replacement;
} Link_EquipmentMatrixtOverride;

typedef struct {
    size_t numDLOverrides;
    Link_DisplayListOverride *DLOverrides;
    size_t *numMtxOverrides;
    Link_EquipmentMatrixtOverride *mtxOverrides;
} Link_EquipmentReplacement;

typedef struct {
    Link_EquipmentReplacement replacements[LINK_DL_REPLACE_MAX];
} Link_EquipmentReplacements;

#endif
