#ifndef EQUIPMENTOVERRIDES_H
#define EQUIPMENTOVERRIDES_H

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

const EquipmentOverride *EquipmentOverrides_getEquipmentOverride(Link_EquipmentReplacement id);

#endif
