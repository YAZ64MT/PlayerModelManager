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
