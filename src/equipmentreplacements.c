#include "global.h"
#include "recomputils.h"
#include "equipmentreplacements.h"
#include "modelmatrixids.h"

void createSwordOverride(Link_EquipmentReplacement *equipRepl, Link_DisplayList hilt, Link_DisplayList blade, Link_DisplayList sheath, Link_EquipmentMatrix mtx) {
    equipRepl->DLOverrides = recomp_alloc(sizeof(*(equipRepl->DLOverrides)) * 3);
    equipRepl->DLOverrides[0].replacement = NULL;
    equipRepl->DLOverrides[1].replacement = NULL;
    equipRepl->DLOverrides[2].replacement = NULL;
    equipRepl->DLOverrides[0].target = hilt;
    equipRepl->DLOverrides[1].target = blade;
    equipRepl->DLOverrides[2].target = sheath;
    equipRepl->numDLOverrides = 3;

    equipRepl->mtxOverrides = recomp_alloc(sizeof(*(equipRepl->mtxOverrides)) * 1);
    equipRepl->mtxOverrides[0].replacement = mtx;
    equipRepl->numMtxOverrides = 1;
}

void Link_EquipmentReplacements_init(Link_EquipmentReplacements *equipRepls) {
#define CREATE_SWORD_OVERRIDE(swordNum) createSwordOverride(&equipRepls[LINK_DL_REPLACE_SWORD##swordNum], LINK_DL_SWORD##swordNum##_HILT, LINK_DL_SWORD##swordNum##_BLADE, LINK_DL_SWORD##swordNum##_SHEATH, LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK);
    CREATE_SWORD_OVERRIDE(1);
    CREATE_SWORD_OVERRIDE(2);
    CREATE_SWORD_OVERRIDE(3);
    CREATE_SWORD_OVERRIDE(4);
    CREATE_SWORD_OVERRIDE(5);
}
