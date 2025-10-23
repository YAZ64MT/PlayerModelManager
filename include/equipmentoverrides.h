#ifndef __EQUIPMENTOVERRIDES_H__
#define __EQUIPMENTOVERRIDES_H__

#include "PR/ultratypes.h"
#include "modelmatrixids.h"

typedef struct {
    Link_DisplayList *overrides;
    size_t count;
} EquipmentOverrideDLs;

typedef struct {
    Link_EquipmentMatrix *overrides;
    size_t count;
} EquipmentOverrideMtxs;

typedef struct {
    EquipmentOverrideDLs dl;
    EquipmentOverrideMtxs mtx;
} EquipmentOverride;

extern const EquipmentOverride gEquipmentOverrideTable[LINK_DL_REPLACE_MAX];

#endif
