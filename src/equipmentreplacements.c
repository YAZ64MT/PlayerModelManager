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

void createBgsOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->numDLOverrides = 5;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_SWORD4_HILT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_SWORD4_BLADE);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_SWORD4_SHEATH);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_SWORD4_BLADE_BROKEN);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_SWORD4_BLADE_FRAGMENT);

    equipRepl->numMtxOverrides = 1;
    equipRepl->mtxOverrides = allocMtxOverrides(equipRepl->numMtxOverrides);
    Link_EquipmentMatrixOverride_init(&equipRepl->mtxOverrides[0], LINK_EQUIP_MATRIX_SWORD4_BACK);
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
    equipRepl->numDLOverrides = 4;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_BOW);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_FPS_BOW);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_BOW_STRING);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[3], LINK_DL_BOW_ARROW);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void createSlingshotOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->numDLOverrides = 2;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_SLINGSHOT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_SLINGSHOT_STRING);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void createHookshotOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->numDLOverrides = 5;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_HOOKSHOT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_FPS_HOOKSHOT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_HOOKSHOT_CHAIN);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[3], LINK_DL_HOOKSHOT_HOOK);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[4], LINK_DL_HOOKSHOT_RETICLE);

    equipRepl->numMtxOverrides = 1;
    equipRepl->mtxOverrides = allocMtxOverrides(equipRepl->numMtxOverrides);
    Link_EquipmentMatrixOverride_init(&equipRepl->mtxOverrides[0], LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK);
}

void createBottleOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->numDLOverrides = 2;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_BOTTLE_GLASS);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_BOTTLE_CONTENTS);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void createPipesOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->numDLOverrides = 6;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_PIPE_A);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_PIPE_LEFT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_PIPE_RIGHT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[3], LINK_DL_PIPE_DOWN);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[4], LINK_DL_PIPE_UP);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[5], LINK_DL_PIPE_MOUTH);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void createDrumsOverride(Link_EquipmentReplacement *equipRepl) {
    equipRepl->numDLOverrides = 6;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], LINK_DL_DRUM_A);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], LINK_DL_DRUM_LEFT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[2], LINK_DL_DRUM_RIGHT);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[3], LINK_DL_DRUM_DOWN);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[4], LINK_DL_DRUM_UP);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[5], LINK_DL_DRUM_STRAP);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void createSingleDLOverride(Link_EquipmentReplacement *equipRepl, Link_DisplayList id) {
    equipRepl->numDLOverrides = 1;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], id);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void createDoubleDLOverride(Link_EquipmentReplacement *equipRepl, Link_DisplayList id1, Link_DisplayList id2) {
    equipRepl->numDLOverrides = 2;
    equipRepl->DLOverrides = allocDLOverrides(equipRepl->numDLOverrides);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[0], id1);
    Link_DisplayListOverride_init(&equipRepl->DLOverrides[1], id2);

    equipRepl->numMtxOverrides = 0;
    equipRepl->mtxOverrides = NULL;
}

void Link_EquipmentReplacements_init(Link_EquipmentReplacements *equipRepls) {
#define CREATE_SWORD_OVERRIDE(swordNum) createSwordOverride(&equipRepls->replacements[LINK_DL_REPLACE_SWORD##swordNum], LINK_DL_SWORD##swordNum##_HILT, LINK_DL_SWORD##swordNum##_BLADE, LINK_DL_SWORD##swordNum##_SHEATH, LINK_EQUIP_MATRIX_SWORD##swordNum##_BACK);
    CREATE_SWORD_OVERRIDE(1);
    CREATE_SWORD_OVERRIDE(2);
    CREATE_SWORD_OVERRIDE(3);
    createBgsOverride(&equipRepls->replacements[LINK_DL_REPLACE_SWORD4]);
    CREATE_SWORD_OVERRIDE(5);
#undef CREATE_SWORD_OVERRIDE

#define CREATE_SHIELD_OVERRIDE(num) createShieldOverride(&equipRepls->replacements[LINK_DL_REPLACE_SHIELD##num], LINK_DL_SHIELD##num, LINK_EQUIP_MATRIX_SHIELD##num##_BACK);
    CREATE_SHIELD_OVERRIDE(1);
    CREATE_SHIELD_OVERRIDE(2);
    CREATE_SHIELD_OVERRIDE(3);
#undef CREATE_SHIELD_OVERRIDE

    createBowOverride(&equipRepls->replacements[LINK_DL_REPLACE_BOW]);
    createHookshotOverride(&equipRepls->replacements[LINK_DL_REPLACE_HOOKSHOT]);
    createSlingshotOverride(&equipRepls->replacements[LINK_DL_REPLACE_SLINGSHOT]);
    createBottleOverride(&equipRepls->replacements[LINK_DL_REPLACE_BOTTLE]);
    createPipesOverride(&equipRepls->replacements[LINK_DL_REPLACE_PIPES]);
    createDrumsOverride(&equipRepls->replacements[LINK_DL_REPLACE_DRUMS]);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_DEKU_STICK], LINK_DL_DEKU_STICK);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_OCARINA_FAIRY], LINK_DL_OCARINA_FAIRY);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_OCARINA_TIME], LINK_DL_OCARINA_TIME);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_HAMMER], LINK_DL_HAMMER);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_GUITAR], LINK_DL_GUITAR);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_SKULL], LINK_DL_MASK_SKULL);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_SPOOKY], LINK_DL_MASK_SPOOKY);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_GERUDO], LINK_DL_MASK_GERUDO);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_TRUTH], LINK_DL_MASK_TRUTH);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_KAFEIS_MASK], LINK_DL_MASK_KAFEIS_MASK);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_ALL_NIGHT], LINK_DL_MASK_ALL_NIGHT);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_BUNNY], LINK_DL_MASK_BUNNY);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_KEATON], LINK_DL_MASK_KEATON);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_GARO], LINK_DL_MASK_GARO);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_ROMANI], LINK_DL_MASK_ROMANI);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_CIRCUS_LEADER], LINK_DL_MASK_CIRCUS_LEADER);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_COUPLE], LINK_DL_MASK_COUPLE);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_POSTMAN], LINK_DL_MASK_POSTMAN);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_GREAT_FAIRY], LINK_DL_MASK_GREAT_FAIRY);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_GIBDO], LINK_DL_MASK_GIBDO);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_DON_GERO], LINK_DL_MASK_DON_GERO);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_KAMARO], LINK_DL_MASK_KAMARO);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_CAPTAIN], LINK_DL_MASK_CAPTAIN);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_STONE], LINK_DL_MASK_STONE);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_BREMEN], LINK_DL_MASK_BREMEN);
    createDoubleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_BLAST], LINK_DL_MASK_BLAST, LINK_DL_MASK_BLAST_COOLING_DOWN);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_SCENTS], LINK_DL_MASK_SCENTS);
    createSingleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_GIANT], LINK_DL_MASK_GIANT);
    createDoubleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_DEKU], LINK_DL_MASK_DEKU, LINK_DL_MASK_DEKU_SCREAM);
    createDoubleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_GORON], LINK_DL_MASK_GORON, LINK_DL_MASK_GORON_SCREAM);
    createDoubleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_ZORA], LINK_DL_MASK_ZORA, LINK_DL_MASK_ZORA_SCREAM);
    createDoubleDLOverride(&equipRepls->replacements[LINK_DL_REPLACE_MASK_FIERCE_DEITY], LINK_DL_MASK_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY_SCREAM);
}
