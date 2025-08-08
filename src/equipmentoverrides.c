#include "global.h"
#include "equipmentoverrides.h"
#include "modelmatrixids.h"

static Link_DisplayList sSword1DLs[] = {LINK_DL_SWORD1_HILT, LINK_DL_SWORD1_BLADE, LINK_DL_SWORD1_SHEATH};
static Link_DisplayList sSword2DLs[] = {LINK_DL_SWORD2_HILT, LINK_DL_SWORD2_BLADE, LINK_DL_SWORD2_SHEATH};
static Link_DisplayList sSword3DLs[] = {LINK_DL_SWORD3_HILT, LINK_DL_SWORD3_BLADE, LINK_DL_SWORD3_SHEATH};
static Link_DisplayList sSword4DLs[] = {LINK_DL_SWORD4_HILT, LINK_DL_SWORD4_BLADE, LINK_DL_SWORD4_SHEATH, LINK_DL_SWORD4_BLADE_BROKEN, LINK_DL_SWORD4_BLADE_FRAGMENT};
static Link_DisplayList sSword5DLs[] = {LINK_DL_SWORD5_HILT, LINK_DL_SWORD5_BLADE, LINK_DL_SWORD5_SHEATH};
static Link_EquipmentMatrix sSword1Mtxs[] = {LINK_EQUIP_MATRIX_SWORD1_BACK};
static Link_EquipmentMatrix sSword2Mtxs[] = {LINK_EQUIP_MATRIX_SWORD2_BACK};
static Link_EquipmentMatrix sSword3Mtxs[] = {LINK_EQUIP_MATRIX_SWORD3_BACK, LINK_EQUIP_MATRIX_SWORD3_PEDESTAL, LINK_EQUIP_MATRIX_SWORD3_PEDESTAL_GRABBED};
static Link_EquipmentMatrix sSword4Mtxs[] = {LINK_EQUIP_MATRIX_SWORD4_BACK};
static Link_EquipmentMatrix sSword5Mtxs[] = {LINK_EQUIP_MATRIX_SWORD5_BACK};

static Link_DisplayList sShield1DLs[] = {LINK_DL_SHIELD1};
static Link_DisplayList sShield2DLs[] = {LINK_DL_SHIELD2};
static Link_DisplayList sShield3DLs[] = {LINK_DL_SHIELD3};
static Link_EquipmentMatrix sShield1Mtxs[] = {LINK_EQUIP_MATRIX_SHIELD1_BACK, LINK_EQUIP_MATRIX_SHIELD1_ITEM};
static Link_EquipmentMatrix sShield2Mtxs[] = {LINK_EQUIP_MATRIX_SHIELD2_BACK};
static Link_EquipmentMatrix sShield3Mtxs[] = {LINK_EQUIP_MATRIX_SHIELD3_BACK};

static Link_DisplayList sHookshotDLs[] = {LINK_DL_HOOKSHOT, LINK_DL_FPS_HOOKSHOT, LINK_DL_HOOKSHOT_CHAIN, LINK_DL_HOOKSHOT_HOOK, LINK_DL_HOOKSHOT_RETICLE};
static Link_EquipmentMatrix sHookshotMtxs[] = {LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK};

static Link_DisplayList sBowDLs[] = {LINK_DL_BOW, LINK_DL_FPS_BOW, LINK_DL_BOW_STRING, LINK_DL_BOW_ARROW};

static Link_DisplayList sSlingshotDLs[] = {LINK_DL_SLINGSHOT, LINK_DL_FPS_SLINGSHOT, LINK_DL_SLINGSHOT_STRING};

static Link_DisplayList sBottleDLs[] = {LINK_DL_BOTTLE_GLASS, LINK_DL_BOTTLE_CONTENTS};

static Link_DisplayList sOcarinaFairyDLs[] = {LINK_DL_OCARINA_FAIRY};

static Link_DisplayList sOcarinaTimeDLs[] = {LINK_DL_OCARINA_TIME};

static Link_DisplayList sBoomerangDLs[] = {LINK_DL_BOOMERANG, LINK_DL_BOOMERANG_FLYING};

static Link_DisplayList sHammerDLs[] = {LINK_DL_HAMMER};

static Link_DisplayList sDekuStickDLs[] = {LINK_DL_DEKU_STICK};

static Link_DisplayList sPipesDLs[] = {LINK_DL_PIPE_MOUTH, LINK_DL_PIPE_A, LINK_DL_PIPE_DOWN, LINK_DL_PIPE_LEFT, LINK_DL_PIPE_RIGHT, LINK_DL_PIPE_UP};

static Link_DisplayList sDrumsDLs[] = {LINK_DL_DRUM_STRAP, LINK_DL_DRUM_A, LINK_DL_DRUM_DOWN, LINK_DL_DRUM_LEFT, LINK_DL_DRUM_RIGHT, LINK_DL_DRUM_UP};

static Link_DisplayList sGuitarDLs[] = {LINK_DL_GUITAR};

static Link_DisplayList sMaskSkullDLs[] = {LINK_DL_MASK_SKULL};

static Link_DisplayList sMaskSpookyDLs[] = {LINK_DL_MASK_SPOOKY};

static Link_DisplayList sMaskGerudoDLs[] = {LINK_DL_MASK_GERUDO};

static Link_DisplayList sMaskTruthDLs[] = {LINK_DL_MASK_TRUTH};

static Link_DisplayList sMaskKafeisMaskDLs[] = {LINK_DL_MASK_KAFEIS_MASK};

static Link_DisplayList sMaskAllNightDLs[] = {LINK_DL_MASK_ALL_NIGHT};

static Link_DisplayList sMaskBunnyDLs[] = {LINK_DL_MASK_BUNNY};

static Link_DisplayList sMaskKeatonDLs[] = {LINK_DL_MASK_KEATON};

static Link_DisplayList sMaskGaroDLs[] = {LINK_DL_MASK_GARO};

static Link_DisplayList sMaskRomaniDLs[] = {LINK_DL_MASK_ROMANI};

static Link_DisplayList sMaskCircusLeaderDLs[] = {LINK_DL_MASK_CIRCUS_LEADER};

static Link_DisplayList sMaskCoupleDLs[] = {LINK_DL_MASK_COUPLE};

static Link_DisplayList sMaskPostmanDLs[] = {LINK_DL_MASK_POSTMAN};

static Link_DisplayList sMaskGreatFairyDLs[] = {LINK_DL_MASK_GREAT_FAIRY};

static Link_DisplayList sMaskGibdoDLs[] = {LINK_DL_MASK_GIBDO};

static Link_DisplayList sMaskDonGeroDLs[] = {LINK_DL_MASK_DON_GERO};

static Link_DisplayList sMaskKamaroDLs[] = {LINK_DL_MASK_KAMARO};

static Link_DisplayList sMaskCaptainDLs[] = {LINK_DL_MASK_CAPTAIN};

static Link_DisplayList sMaskStoneDLs[] = {LINK_DL_MASK_STONE};

static Link_DisplayList sMaskBremenDLs[] = {LINK_DL_MASK_BREMEN};

static Link_DisplayList sMaskBlastDLs[] = {LINK_DL_MASK_BLAST, LINK_DL_MASK_BLAST_COOLING_DOWN};

static Link_DisplayList sMaskScentsDLs[] = {LINK_DL_MASK_SCENTS};

static Link_DisplayList sMaskGiantDLs[] = {LINK_DL_MASK_GIANT};

static Link_DisplayList sMaskDekuDLs[] = {LINK_DL_MASK_DEKU, LINK_DL_MASK_DEKU_SCREAM};

static Link_DisplayList sMaskGoronDLs[] = {LINK_DL_MASK_GORON, LINK_DL_MASK_GORON_SCREAM};

static Link_DisplayList sMaskZoraDLs[] = {LINK_DL_MASK_ZORA, LINK_DL_MASK_ZORA_SCREAM};

static Link_DisplayList sMaskFierceDeityDLs[] = {LINK_DL_MASK_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY_SCREAM};

static Link_EquipmentMatrix sEmptyMtxs[] = {};

// clang-format off
#define DECLARE_OVERRIDE_TABLE_ENTRY(disps, mtxs) {.dl = {.overrides = &disps, .count = ARRAY_COUNT(disps)}, .mtx = {.overrides = &mtxs, .count = ARRAY_COUNT(mtxs)}}
// clang-format on

const EquipmentOverride gEquipmentOverrideTable[LINK_DL_REPLACE_MAX] = {
    DECLARE_OVERRIDE_TABLE_ENTRY(sSword1DLs, sSword1Mtxs),          // LINK_DL_REPLACE_SWORD1
    DECLARE_OVERRIDE_TABLE_ENTRY(sSword2DLs, sSword2Mtxs),          // LINK_DL_REPLACE_SWORD2
    DECLARE_OVERRIDE_TABLE_ENTRY(sSword3DLs, sSword3Mtxs),          // LINK_DL_REPLACE_SWORD3
    DECLARE_OVERRIDE_TABLE_ENTRY(sSword4DLs, sSword4Mtxs),          // LINK_DL_REPLACE_SWORD4
    DECLARE_OVERRIDE_TABLE_ENTRY(sSword5DLs, sSword5Mtxs),          // LINK_DL_REPLACE_SWORD5
    DECLARE_OVERRIDE_TABLE_ENTRY(sShield1DLs, sShield1Mtxs),        // LINK_DL_REPLACE_SHIELD1
    DECLARE_OVERRIDE_TABLE_ENTRY(sShield2DLs, sShield2Mtxs),        // LINK_DL_REPLACE_SHIELD2
    DECLARE_OVERRIDE_TABLE_ENTRY(sShield3DLs, sShield3Mtxs),        // LINK_DL_REPLACE_SHIELD3
    DECLARE_OVERRIDE_TABLE_ENTRY(sHookshotDLs, sHookshotMtxs),      // LINK_DL_REPLACE_HOOKSHOT
    DECLARE_OVERRIDE_TABLE_ENTRY(sBowDLs, sEmptyMtxs),              // LINK_DL_REPLACE_BOW
    DECLARE_OVERRIDE_TABLE_ENTRY(sSlingshotDLs, sEmptyMtxs),        // LINK_DL_REPLACE_SLINGSHOT
    DECLARE_OVERRIDE_TABLE_ENTRY(sBottleDLs, sEmptyMtxs),           // LINK_DL_REPLACE_BOTTLE
    DECLARE_OVERRIDE_TABLE_ENTRY(sOcarinaFairyDLs, sEmptyMtxs),     // LINK_DL_REPLACE_OCARINA_FAIRY
    DECLARE_OVERRIDE_TABLE_ENTRY(sOcarinaTimeDLs, sEmptyMtxs),      // LINK_DL_REPLACE_OCARINA_TIME
    DECLARE_OVERRIDE_TABLE_ENTRY(sBoomerangDLs, sEmptyMtxs),        // LINK_DL_REPLACE_BOOMERANG
    DECLARE_OVERRIDE_TABLE_ENTRY(sHammerDLs, sEmptyMtxs),           // LINK_DL_REPLACE_HAMMER
    DECLARE_OVERRIDE_TABLE_ENTRY(sDekuStickDLs, sEmptyMtxs),        // LINK_DL_REPLACE_DEKU_STICK
    DECLARE_OVERRIDE_TABLE_ENTRY(sPipesDLs, sEmptyMtxs),            // LINK_DL_REPLACE_PIPES
    DECLARE_OVERRIDE_TABLE_ENTRY(sPipesDLs, sEmptyMtxs),            // LINK_DL_REPLACE_DRUMS
    DECLARE_OVERRIDE_TABLE_ENTRY(sGuitarDLs, sEmptyMtxs),           // LINK_DL_REPLACE_GUITAR
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskSkullDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_SKULL
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskSpookyDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_SPOOKY
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskGerudoDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_GERUDO
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskTruthDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_TRUTH
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskKafeisMaskDLs, sEmptyMtxs),   // LINK_DL_REPLACE_MASK_KAFEIS_MASK
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskAllNightDLs, sEmptyMtxs),     // LINK_DL_REPLACE_MASK_ALL_NIGHT
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskBunnyDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_BUNNY
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskKeatonDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_KEATON
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskGaroDLs, sEmptyMtxs),         // LINK_DL_REPLACE_MASK_GARO
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskRomaniDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_ROMANI
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskCircusLeaderDLs, sEmptyMtxs), // LINK_DL_REPLACE_MASK_CIRCUS_LEADER
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskCoupleDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_COUPLE
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskPostmanDLs, sEmptyMtxs),      // LINK_DL_REPLACE_MASK_POSTMAN
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskGreatFairyDLs, sEmptyMtxs),   // LINK_DL_REPLACE_MASK_GREAT_FAIRY
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskGibdoDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_GIBDO
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskDonGeroDLs, sEmptyMtxs),      // LINK_DL_REPLACE_MASK_DON_GERO
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskKamaroDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_KAMARO
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskCaptainDLs, sEmptyMtxs),      // LINK_DL_REPLACE_MASK_CAPTAIN
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskStoneDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_STONE
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskBremenDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_BREMEN
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskBlastDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_BLAST
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskScentsDLs, sEmptyMtxs),       // LINK_DL_REPLACE_MASK_SCENTS
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskGiantDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_GIANT
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskDekuDLs, sEmptyMtxs),         // LINK_DL_REPLACE_MASK_DEKU
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskGoronDLs, sEmptyMtxs),        // LINK_DL_REPLACE_MASK_GORON
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskZoraDLs, sEmptyMtxs),         // LINK_DL_REPLACE_MASK_ZORA
    DECLARE_OVERRIDE_TABLE_ENTRY(sMaskFierceDeityDLs, sEmptyMtxs),  // LINK_DL_REPLACE_MASK_FIERCE_DEITY
};

#undef DECLARE_OVERRIDE_TABLE_ENTRY
