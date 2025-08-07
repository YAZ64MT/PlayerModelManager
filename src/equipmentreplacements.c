#include "global.h"
#include "recomputils.h"
#include "equipmentreplacements.h"
#include "modelmatrixids.h"

void Link_DisplayListOverride_init(Link_DisplayListOverride *override, Link_DisplayList id) {
    override->replacement = NULL;
    override->target = id;
}

void Link_EquipmentMatrixOverride_init(Link_EquipmentMatrixOverride *override, Link_EquipmentMatrix id) {
    override->replacement = NULL;
    override->target = id;
}

void *allocDLOverrides(int num) {
    if (num > 0) {
        return recomp_alloc(sizeof(Link_DisplayListOverride) * num);
    }

    recomp_printf("WARNING: TRIED TO ALLOC %d Link_DisplayListOverride STRUCTS\n", num);
    return NULL;
}

void *allocMtxOverrides(int num) {
    if (num > 0) {
        return recomp_alloc(sizeof(Link_EquipmentMatrixOverride) * num);
    }

    recomp_printf("WARNING: TRIED TO ALLOC %d Link_EquipmentMatrixOverride STRUCTS\n", num);
    return NULL;
}

void createSwordOverride(Link_EquipmentReplacement *equipRepl, Link_DisplayList hilt, Link_DisplayList blade, Link_DisplayList sheath, Link_EquipmentMatrix hiltBackMtx) {
    equipRepl->numDLOverrides = 3;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], hilt);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], blade);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], sheath);

    equipRepl->numMtxOverrides = 1;
    equipRepl->mtxOverrides = allocMtxOverrides(equipRepl->numMtxOverrides);
    Link_EquipmentMatrixOverride_init(&equipRepl->mtxOverrides[0], hiltBackMtx);
}

void createShieldOverride(Link_EquipmentReplacement *equipRepl, Link_DisplayList shield, Link_EquipmentMatrix backMtx) {
    equipRepl->numDLOverrides = 1;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], shield);

    equipRepl->numMtxOverrides = 1;
    equipRepl->mtxOverrides = allocMtxOverrides(equipRepl->numMtxOverrides);
    Link_EquipmentMatrixOverride_init(&equipRepl->mtxOverrides[0], backMtx);
}

void createBowOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->DLOverrides = allocDLOverrides(4);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_BOW);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_FPS_BOW);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_BOW_STRING);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[3], LINK_DL_BOW_ARROW);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void createHookshotOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->DLOverrides = allocDLOverrides(5);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_HOOKSHOT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_FPS_HOOKSHOT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_HOOKSHOT_CHAIN);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[3], LINK_DL_HOOKSHOT_HOOK);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[4], LINK_DL_HOOKSHOT_RETICLE);

    equipRepl->numMtxOverrides = 1;
    equipRepl->mtxOverrides = allocMtxOverrides(equipRepl->numMtxOverrides);
    Link_EquipmentMatrixOverride_init(&equipRepl->mtxOverrides[0], LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK);
}

void Link_EquipmentReplacements_init(Link_EquipmentReplacements *equipRepls) {
#define CREATE_SWORD_OVERRIDE(swordNum) createSwordOverride(&equipRepls[LINK_DL_REPLACE_SWORD##swordNum], LINK_DL_SWORD##swordNum##_HILT, LINK_DL_SWORD##swordNum##_BLADE, LINK_DL_SWORD##swordNum##_SHEATH, LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK);
    CREATE_SWORD_OVERRIDE(1);
    CREATE_SWORD_OVERRIDE(2);
    CREATE_SWORD_OVERRIDE(3);
    CREATE_SWORD_OVERRIDE(4);
    CREATE_SWORD_OVERRIDE(5);
#undef CREATE_SWORD_OVERRIDE

#define CREATE_SHIELD_OVERRIDE(num) createShieldOverride(&equipRepls[LINK_DL_REPLACE_SHIELD##num], LINK_DL_SHIELD##num, LINK_EQUIP_MATRIX_SHIELD##num##_BACK);
    CREATE_SHIELD_OVERRIDE(1);
    CREATE_SHIELD_OVERRIDE(2);
    CREATE_SHIELD_OVERRIDE(3);
}
