#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "playermodelmanager.h"

extern Gfx **sPlayerModels[PLAYER_FORM_MAX];

void setupHumanModels() {
    Gfx **humanModels = sPlayerModels[PLAYER_FORM_HUMAN];

    // limbs
    humanModels[LINK_DL_WAIST] = gLinkHumanWaistDL;
    humanModels[LINK_DL_RTHIGH] = gLinkHumanRightThighDL;
    humanModels[LINK_DL_RSHIN] = gLinkHumanRightShinDL;
    humanModels[LINK_DL_RFOOT] = gLinkHumanRightFootDL;
    humanModels[LINK_DL_LTHIGH] = gLinkHumanLeftThighDL;
    humanModels[LINK_DL_LSHIN] = gLinkHumanLeftShinDL;
    humanModels[LINK_DL_LFOOT] = gLinkHumanLeftFootDL;
    humanModels[LINK_DL_HEAD] = gLinkHumanHeadDL;
    humanModels[LINK_DL_HAT] = gLinkHumanHatDL;
    humanModels[LINK_DL_COLLAR] = gLinkHumanCollarDL;
    humanModels[LINK_DL_LSHOULDER] = gLinkHumanLeftShoulderDL;
    humanModels[LINK_DL_LFOREARM] = gLinkHumanLeftForearmDL;
    humanModels[LINK_DL_RSHOULDER] = gLinkHumanRightShoulderDL;
    humanModels[LINK_DL_RFOREARM] = gLinkHumanRightForearmDL;
    humanModels[LINK_DL_TORSO] = gLinkHumanTorsoDL;

    // hands
    humanModels[LINK_DL_LHAND] = gLinkHumanLeftHandOpenDL;
    humanModels[LINK_DL_LFIST] = gLinkHumanLeftHandClosedDL;
    humanModels[LINK_DL_LHAND_BOTTLE] = gLinkHumanLeftHandHoldBottleDL;
    humanModels[LINK_DL_RHAND] = gLinkHumanRightHandOpenDL;
    humanModels[LINK_DL_RFIST] = gLinkHumanRightHandClosedDL;

    // sword sheathes
    humanModels[LINK_DL_SWORD_SHEATH_1] = gLinkHumanKokiriSwordSheathDL;
    humanModels[LINK_DL_SWORD_SHEATH_2] = gLinkHumanRazorSwordSheathDL;
    humanModels[LINK_DL_SWORD_SHEATH_3] = gLinkHumanGildedSwordSheathDL;

    // sword hilts
    humanModels[LINK_DL_SWORD_HILT_1] = gKokiriSwordHandleDL;
    humanModels[LINK_DL_SWORD_HILT_2] = gRazorSwordHandleDL;
    humanModels[LINK_DL_SWORD_HILT_3] = gLinkHumanGildedSwordHandleDL;

    // sword blades
    humanModels[LINK_DL_SWORD_BLADE_1] = gKokiriSwordBladeDL;
    humanModels[LINK_DL_SWORD_BLADE_2] = gRazorSwordBladeDL;
    humanModels[LINK_DL_SWORD_BLADE_3] = gLinkHumanGildedSwordBladeDL;

    // shields
    humanModels[LINK_DL_SHIELD_1] = gLinkHumanHerosShieldDL;
    humanModels[LINK_DL_SHIELD_2] = gLinkHumanMirrorShieldDL;
    //humanModels[LINK_DL_SHIELD_3];

    // items
    humanModels[LINK_DL_BOTTLE] = gBottleGlassDL;
    humanModels[LINK_DL_OCARINA_TIME] = (Gfx*)0x06010448;
    humanModels[LINK_DL_DEKU_STICK] = gDekuStickDL;
    humanModels[LINK_DL_BOW] = gLinkHumanBowDL;
    //humanModels[LINK_DL_BOW_STRING];
    humanModels[LINK_DL_HOOKSHOT] = gLinkHumanHookshotDL;
    humanModels[LINK_DL_HOOKSHOT_CHAIN] = gHookshotChainDL;
    humanModels[LINK_DL_HOOKSHOT_RETICLE] = gHookshotReticleDL;

    // First Person
    humanModels[LINK_DL_FPS_LFOREARM] = gLinkHumanLeftForearmDL;
    humanModels[LINK_DL_FPS_LHAND] = gLinkHumanLeftHandClosedDL;
    humanModels[LINK_DL_FPS_RFOREARM];
    humanModels[LINK_DL_FPS_HOOKSHOT] = object_link_child_DL_017B40;

    // masks
    humanModels[LINK_DL_DEKU_MASK] = gDekuMaskDL;
    humanModels[LINK_DL_GORON_MASK] = gGoronMaskDL;
    humanModels[LINK_DL_ZORA_MASK] = gZoraMaskDL;
    humanModels[LINK_DL_DEITY_MASK] = gFierceDeityMaskDL;

    // bottles
    humanModels[LINK_DL_BOTTLE_FILLING] = gBottleContentsDL;
    humanModels[LINK_DL_BOTTLE_EMPTY] = gBottleGlassDL;

    // whole sword
    humanModels[LINK_DL_SWORD4] = gLinkHumanGreatFairysSwordDL;
}
