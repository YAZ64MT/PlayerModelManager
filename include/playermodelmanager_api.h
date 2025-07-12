#ifndef __PLAYERMODELMANAGER_API__
#define __PLAYERMODELMANAGER_API__

typedef unsigned long PlayerModelManagerHandle;

// Used for keeping compatibility between versions
// DO NOT EDIT
#define PMM_API_VERSION 1UL

#define YAZMT_PMM_MOD_NAME "yazmt_mm_playermodelmanager"

#define PMM_MAX_INTERNAL_NAME_LENGTH 64
#define PMM_MAX_AUTHOR_NAME_LENGTH 64
#define PMM_MAX_DISPLAY_NAME_LENGTH 32

typedef enum {

    // Main skeleton DLs
    PMM_DL_WAIST,
    PMM_DL_RTHIGH,
    PMM_DL_RSHIN,
    PMM_DL_RFOOT,
    PMM_DL_LTHIGH,
    PMM_DL_LSHIN,
    PMM_DL_LFOOT,
    PMM_DL_HEAD,
    PMM_DL_HAT,
    PMM_DL_COLLAR,
    PMM_DL_LSHOULDER,
    PMM_DL_LFOREARM,
    PMM_DL_LHAND,
    PMM_DL_RSHOULDER,
    PMM_DL_RFOREARM,
    PMM_DL_RHAND,
    PMM_DL_SHEATH_NONE,
    PMM_DL_TORSO,

    // Other hand DLs
    PMM_DL_LFIST,
    PMM_DL_LHAND_BOTTLE,
    PMM_DL_LHAND_GUITAR,
    PMM_DL_RFIST,

    // Swords:
    //      1: Kokiri Sword
    //      2: Razor Sword
    //      3: Master Sword / Gilded Sword
    //      4: Giant's Knife / Biggoron Sword / Fierce Deity's Sword
    //      5: Great Fairy Sword

    PMM_DL_SWORD1_SHEATH,
    PMM_DL_SWORD2_SHEATH,
    PMM_DL_SWORD3_SHEATH,
    PMM_DL_SWORD4_SHEATH,
    PMM_DL_SWORD5_SHEATH,
    PMM_DL_SWORD1_HILT,
    PMM_DL_SWORD2_HILT,
    PMM_DL_SWORD3_HILT,
    PMM_DL_SWORD4_HILT,
    PMM_DL_SWORD5_HILT,
    PMM_DL_SWORD1_BLADE,
    PMM_DL_SWORD2_BLADE,
    PMM_DL_SWORD3_BLADE,
    PMM_DL_SWORD4_BLADE,
    PMM_DL_SWORD4_BLADE_BROKEN,
    PMM_DL_SWORD4_BLADE_FRAGMENT,
    PMM_DL_SWORD5_BLADE,

    PMM_DL_SHIELD1,     // Deku Shield
    PMM_DL_SHIELD2,     // Hylian Shield / Hero's Shield
    PMM_DL_SHIELD3,     // Mirror Shield
    PMM_DL_SHIELD3_RAY, // Mirror Shield Projection

    // Items
    PMM_DL_BOTTLE_GLASS,
    PMM_DL_BOTTLE_CONTENTS,
    PMM_DL_OCARINA_FAIRY,
    PMM_DL_OCARINA_TIME,
    PMM_DL_DEKU_STICK,
    PMM_DL_BOW,
    PMM_DL_BOW_STRING,
    PMM_DL_BOW_ARROW,
    PMM_DL_SLINGSHOT,
    PMM_DL_SLINGSHOT_STRING,
    PMM_DL_HOOKSHOT,
    PMM_DL_HOOKSHOT_CHAIN,
    PMM_DL_HOOKSHOT_HOOK,
    PMM_DL_HOOKSHOT_RETICLE,
    PMM_DL_FPS_HOOKSHOT,
    PMM_DL_BOOMERANG,        // Boomerang model while held in hand
    PMM_DL_BOOMERANG_FLYING, // Boomerang model after being thrown
    PMM_DL_HAMMER,

    // First Person
    PMM_DL_FPS_LFOREARM,
    PMM_DL_FPS_LHAND,
    PMM_DL_FPS_RFOREARM,
    PMM_DL_FPS_RHAND,

    // Masks
    PMM_DL_MASK_SKULL,
    PMM_DL_MASK_SPOOKY,
    PMM_DL_MASK_GERUDO,
    PMM_DL_MASK_TRUTH,
    PMM_DL_MASK_KAFEIS_MASK,
    PMM_DL_MASK_ALL_NIGHT,
    PMM_DL_MASK_BUNNY, // May look odd due to animation
    PMM_DL_MASK_KEATON,
    PMM_DL_MASK_GARO,
    PMM_DL_MASK_ROMANI,
    PMM_DL_MASK_CIRCUS_LEADER,
    PMM_DL_MASK_COUPLE,
    PMM_DL_MASK_POSTMAN,
    PMM_DL_MASK_GREAT_FAIRY, // May look odd due to animation
    PMM_DL_MASK_GIBDO,
    PMM_DL_MASK_DON_GERO,
    PMM_DL_MASK_KAMARO,
    PMM_DL_MASK_CAPTAIN,
    PMM_DL_MASK_STONE,
    PMM_DL_MASK_BREMEN,
    PMM_DL_MASK_BLAST,
    PMM_DL_MASK_BLAST_COOLING_DOWN,
    PMM_DL_MASK_SCENTS,
    PMM_DL_MASK_GIANT,
    PMM_DL_MASK_DEKU,
    PMM_DL_MASK_GORON,
    PMM_DL_MASK_ZORA,
    PMM_DL_MASK_FIERCE_DEITY,
    PMM_DL_MASK_DEKU_SCREAM,
    PMM_DL_MASK_GORON_SCREAM,
    PMM_DL_MASK_ZORA_SCREAM,
    PMM_DL_MASK_FIERCE_DEITY_SCREAM,

    // Elegy of Emptiness statues
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_HUMAN, // Only used if model is human
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU,  // Only used if model is Deku
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_GORON, // Only used if model is Goron
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA,  // Only used if model is Zora

    // Strength Upgrades (OoT)
    PMM_DL_BRACELET_LFOREARM,
    PMM_DL_GAUNTLET_LFOREARM,
    PMM_DL_GAUNTLET_LHAND,
    PMM_DL_GAUNTLET_LFIST,
    PMM_DL_GAUNTLET_RFOREARM,
    PMM_DL_GAUNTLET_RHAND,
    PMM_DL_GAUNTLET_RFIST,

    // Boots (OoT)
    PMM_DL_BOOT_LIRON,
    PMM_DL_BOOT_RIRON,
    PMM_DL_BOOT_LHOVER,
    PMM_DL_BOOT_RHOVER,

    // Deku Instrument / Pipes
    PMM_DL_DEKU_GUARD,
    PMM_DL_PIPE_MOUTH,
    PMM_DL_PIPE_RIGHT,
    PMM_DL_PIPE_UP,
    PMM_DL_PIPE_DOWN,
    PMM_DL_PIPE_LEFT,
    PMM_DL_PIPE_A,

    // Deku Propellers
    PMM_DL_STEM_RIGHT,
    PMM_DL_STEM_LEFT,
    PMM_DL_PETAL_PARTICLE,
    PMM_DL_FLOWER_PROPELLER_CLOSED,
    PMM_DL_FLOWER_CENTER_CLOSED,
    PMM_DL_FLOWER_PROPELLER_OPEN,
    PMM_DL_FLOWER_CENTER_OPEN,

    // Deku Pad DLs
    PMM_DL_PAD_WOOD,
    PMM_DL_PAD_GRASS,
    PMM_DL_PAD_OPENING,

    // Goron Instrument / Drums
    PMM_DL_DRUM_STRAP,
    PMM_DL_DRUM_UP,
    PMM_DL_DRUM_LEFT,
    PMM_DL_DRUM_RIGHT,
    PMM_DL_DRUM_DOWN,
    PMM_DL_DRUM_A,

    // Goron shield animation skeleton DLs
    PMM_DL_BODY_SHIELD_HEAD,
    PMM_DL_BODY_SHIELD_BODY,
    PMM_DL_BODY_SHIELD_ARMS_AND_LEGS,

    // Goron Roll / Goron Magic DLs
    PMM_DL_CURLED,
    PMM_DL_SPIKES,
    PMM_DL_FIRE_INIT,
    PMM_DL_FIRE_ROLL,
    PMM_DL_FIRE_PUNCH,

    // Zora Fins
    PMM_DL_LFIN,
    PMM_DL_LFIN_SWIM,
    PMM_DL_LFIN_BOOMERANG,
    PMM_DL_RFIN,
    PMM_DL_RFIN_SWIM,
    PMM_DL_RFIN_BOOMERANG,
    PMM_DL_FIN_SHIELD,

    // Zora Magic
    PMM_DL_MAGIC_BARRIER,

    // Zora Instrument / Guitar
    PMM_DL_GUITAR,

    // Hilt + Blade
    PMM_DL_SHIM_SWORD1,
    PMM_DL_SHIM_SWORD2,
    PMM_DL_SHIM_SWORD3,
    PMM_DL_SHIM_SWORD4,
    PMM_DL_SHIM_SWORD4_BROKEN,
    PMM_DL_SHIM_SWORD5,

    // Hilt + Back Matrix
    PMM_DL_SHIM_SWORD1_HILT_BACK,
    PMM_DL_SHIM_SWORD2_HILT_BACK,
    PMM_DL_SHIM_SWORD3_HILT_BACK,
    PMM_DL_SHIM_SWORD4_HILT_BACK,
    PMM_DL_SHIM_SWORD5_HILT_BACK,

    // Hilt w/ Back Matrix + Sheath
    PMM_DL_SHIM_SWORD1_SHEATHED,
    PMM_DL_SHIM_SWORD2_SHEATHED,
    PMM_DL_SHIM_SWORD3_SHEATHED,
    PMM_DL_SHIM_SWORD4_SHEATHED,
    PMM_DL_SHIM_SWORD5_SHEATHED,

    // Shield w/ Back Matrix
    PMM_DL_SHIM_SHIELD1_BACK,
    PMM_DL_SHIM_SHIELD2_BACK,
    PMM_DL_SHIM_SHIELD3_BACK,

    // Sheath + Shield w/ Back Matrix
    PMM_DL_SHIM_SWORD1_SHIELD1_SHEATH,
    PMM_DL_SHIM_SWORD1_SHIELD2_SHEATH,
    PMM_DL_SHIM_SWORD1_SHIELD3_SHEATH,
    PMM_DL_SHIM_SWORD2_SHIELD1_SHEATH,
    PMM_DL_SHIM_SWORD2_SHIELD2_SHEATH,
    PMM_DL_SHIM_SWORD2_SHIELD3_SHEATH,
    PMM_DL_SHIM_SWORD3_SHIELD1_SHEATH,
    PMM_DL_SHIM_SWORD3_SHIELD2_SHEATH,
    PMM_DL_SHIM_SWORD3_SHIELD3_SHEATH,
    PMM_DL_SHIM_SWORD4_SHIELD1_SHEATH,
    PMM_DL_SHIM_SWORD4_SHIELD2_SHEATH,
    PMM_DL_SHIM_SWORD4_SHIELD3_SHEATH,
    PMM_DL_SHIM_SWORD5_SHIELD1_SHEATH,
    PMM_DL_SHIM_SWORD5_SHIELD2_SHEATH,
    PMM_DL_SHIM_SWORD5_SHIELD3_SHEATH,

    // Sheath + Shield w/ Back Matrix + Hilt w/ Back Matrix
    PMM_DL_SHIM_SWORD1_SHIELD1_SHEATHED,
    PMM_DL_SHIM_SWORD1_SHIELD2_SHEATHED,
    PMM_DL_SHIM_SWORD1_SHIELD3_SHEATHED,
    PMM_DL_SHIM_SWORD2_SHIELD1_SHEATHED,
    PMM_DL_SHIM_SWORD2_SHIELD2_SHEATHED,
    PMM_DL_SHIM_SWORD2_SHIELD3_SHEATHED,
    PMM_DL_SHIM_SWORD3_SHIELD1_SHEATHED,
    PMM_DL_SHIM_SWORD3_SHIELD2_SHEATHED,
    PMM_DL_SHIM_SWORD3_SHIELD3_SHEATHED,
    PMM_DL_SHIM_SWORD4_SHIELD1_SHEATHED,
    PMM_DL_SHIM_SWORD4_SHIELD2_SHEATHED,
    PMM_DL_SHIM_SWORD4_SHIELD3_SHEATHED,
    PMM_DL_SHIM_SWORD5_SHIELD1_SHEATHED,
    PMM_DL_SHIM_SWORD5_SHIELD2_SHEATHED,
    PMM_DL_SHIM_SWORD5_SHIELD3_SHEATHED,

    // Left Fist + Sword w/ Blade
    PMM_DL_SHIM_LFIST_SWORD1,
    PMM_DL_SHIM_LFIST_SWORD2,
    PMM_DL_SHIM_LFIST_SWORD3,
    PMM_DL_SHIM_LFIST_SWORD3_PEDESTAL_GRABBED,
    PMM_DL_SHIM_LFIST_SWORD4,
    PMM_DL_SHIM_LFIST_SWORD4_BROKEN,
    PMM_DL_SHIM_LFIST_SWORD5,

    // Right Fist + Shield
    PMM_DL_SHIM_RFIST_SHIELD1,
    PMM_DL_SHIM_RFIST_SHIELD2,
    PMM_DL_SHIM_RFIST_SHIELD3,

    // Reserved (OoT)
    PMM_DL_SHIM_SHIELD1_ITEM,            // Burning Deku Shield
    PMM_DL_SHIM_SWORD3_PEDESTAL,         // Master Sword rotated to be inside pedestal
    PMM_DL_SHIM_SWORD3_PEDESTAL_GRABBED, // Master Sword rotated for grabbed from pedestal

    // Left First + Items
    PMM_DL_SHIM_LFIST_HAMMER,
    PMM_DL_SHIM_LFIST_BOOMERANG,
    PMM_DL_SHIM_RFIST_BOW,

    // Right Fist + Items
    PMM_DL_SHIM_RFIST_SLINGSHOT,
    PMM_DL_SHIM_RFIST_HOOKSHOT,
    PMM_DL_SHIM_RHAND_OCARINA_FAIRY,
    PMM_DL_SHIM_RHAND_OCARINA_TIME,

    // First Person Right Hand + Items
    PMM_DL_SHIM_FPS_RHAND_BOW,
    PMM_DL_SHIM_FPS_RHAND_SLINGSHOT,
    PMM_DL_SHIM_FPS_RHAND_HOOKSHOT,

    // Deku Link flower propellers
    PMM_DL_SHIM_CENTER_FLOWER_PROPELLER_OPEN,
    PMM_DL_SHIM_CENTER_FLOWER_PROPELLER_CLOSED,
    PMM_DL_MAX
} PlayerModelManagerDisplayListId;

#define PMM_DL_SWORD_KOKIRI_HILT PMM_DL_SWORD1_HILT
#define PMM_DL_SWORD_KOKIRI_BLADE PMM_DL_SWORD1_BLADE
#define PMM_DL_SWORD_KOKIRI_SHEATH PMM_DL_SWORD1_SHEATH

#define PMM_DL_SWORD_RAZOR_HILT PMM_DL_SWORD2_HILT
#define PMM_DL_SWORD_RAZOR_BLADE PMM_DL_SWORD2_BLADE
#define PMM_DL_SWORD_RAZOR_SHEATH PMM_DL_SWORD2_SHEATH

#define PMM_DL_SWORD_GILDED_HILT PMM_DL_SWORD3_HILT
#define PMM_DL_SWORD_GILDED_BLADE PMM_DL_SWORD3_BLADE
#define PMM_DL_SWORD_GILDED_SHEATH PMM_DL_SWORD3_SHEATH

#define PMM_DL_SWORD_GREAT_FAIRY_HILT PMM_DL_SWORD5_HILT
#define PMM_DL_SWORD_GREAT_FAIRY_BLADE PMM_DL_SWORD5_BLADE

#define PMM_DL_SWORD_FIERCE_DEITY_HILT PMM_DL_SWORD4_HILT
#define PMM_DL_SWORD_FIERCE_DEITY_BLADE PMM_DL_SWORD4_BLADE

#define PMM_DL_SHIELD_HERO PMM_DL_SHIELD2

#define PMM_DL_SHIELD_MIRROR PMM_DL_SHIELD3
#define PMM_DL_SHIELD_MIRROR_RAY PMM_DL_SHIELD3_RAY

typedef enum {
    PMM_MTX_SWORD1_BACK,
    PMM_MTX_SWORD2_BACK,
    PMM_MTX_SWORD3_BACK,
    PMM_MTX_SWORD4_BACK,
    PMM_MTX_SWORD5_BACK,
    PMM_MTX_SHIELD1_BACK,
    PMM_MTX_SHIELD2_BACK,
    PMM_MTX_SHIELD3_BACK,
    PMM_MTX_SHIELD1_ITEM,
    PMM_MTX_SWORD3_PEDESTAL,
    PMM_MTX_SWORD3_PEDESTAL_GRABBED,
    PMM_MTX_MASKS,
    PMM_MTX_ARROW_DRAWN,
    PMM_MTX_HOOKSHOT_CHAIN_AND_HOOK,
    PMM_MTX_MAX,
} PlayerModelManagerMatrixId;

#define PMM_MTX_SWORD_KOKIRI_BACK PMM_MTX_SWORD1_BACK
#define PMM_MTX_SWORD_RAZOR_BACK PMM_MTX_SWORD2_BACK
#define PMM_MTX_SWORD_GILDED_BACK PMM_MTX_SWORD3_BACK

#define PMM_MTX_SHIELD_HERO_BACK PMM_MTX_SHIELD2_BACK
#define PMM_MTX_SHIELD_MIRROR_BACK PMM_MTX_SHIELD3_BACK

typedef enum {
    PMM_MODEL_TYPE_NONE,
    PMM_MODEL_TYPE_CHILD,
    PMM_MODEL_TYPE_ADULT,
    PMM_MODEL_TYPE_DEKU,
    PMM_MODEL_TYPE_GORON,
    PMM_MODEL_TYPE_ZORA,
    PMM_MODEL_TYPE_FIERCE_DEITY,
    PMM_MODEL_TYPE_MAX
} PlayerModelManagerModelType;

typedef enum {
    PMM_EVENT_MODEL_APPLIED,
    PMM_EVENT_MODEL_REMOVED,
} PlayerModelManagerModelEvent;

typedef void PlayerModelManagerEventHandler(PlayerModelManagerHandle handle, PlayerModelManagerModelEvent event, void *userdata);

#ifndef YAZMT_PMM_NO_API_IMPORTS

#include "global.h"
#include "modding.h"

// Registers a new player model and returns a handle to it.
//
// A handle represents an entry in the in-game model manager menu, so hold onto it.
//
// Choose a unique string identifier. It will not show up in the menu if you set a display name, so it need not be human readable.
// There is a maximum length of 64 characters.
//
// This function can only be used during the PlayerModelManager_onRegisterModels event. Otherwise, an invalid handle will be returned.
#define PLAYERMODELMANAGER_REGISTER_FORM_MODEL(internalName, modelType) PlayerModelManager_registerFormModel(PMM_API_VERSION, internalName, modelType)
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, PlayerModelManagerHandle PlayerModelManager_registerFormModel(unsigned long apiVersion, const char *internalName, PlayerModelManagerModelType modelType));

// Sets the name that will appear in the menu for the passed in model handle.
//
// Limited to 32 characters.
//
// This function can only be used during the PlayerModelManager_onRegisterModels event.
//
// Returns true if successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setDisplayName(PlayerModelManagerHandle h, const char *displayName));

// Sets the name that will appear in the author field of the menu.
//
// Limited to 64 characters.
//
// This function can only be used during the PlayerModelManager_onRegisterModels event.
//
// Returns true if successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setAuthor(PlayerModelManagerHandle h, const char *author));

// Sets the skeleton and any display lists attached to it on a custom model.
//
// Returns true if successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel));

// Sets the skeleton and any display lists attached to it on the shielding skeleton of a custom model.
//
// This is used for Goron Link's guard animation, so make sure to pass in a skeleton with the appropriaate amount of limbs.
//
// Returns true if successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setShieldingSkeleton(PlayerModelManagerHandle h, FlexSkeletonHeader *skel));

// Set a display list on the model. The ID can be obtained from PlayerModelManagerDisplayListId.
//
// Display lists prefixed with SHIM are automatically generated, and while you can replace them, you are encouraged not to
// to increase compatibility with equipment replacement mods.
//
// Returns true upon successful display list set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setDisplayList(PlayerModelManagerHandle h, PlayerModelManagerDisplayListId dlId, Gfx *dl));

// Sets a matrix on the custom model.
//
// Returns true if matrix was successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setMatrix(PlayerModelManagerHandle h, PlayerModelManagerMatrixId mtxId, Mtx *matrix));

// Set a callback function for this model. The types of events that can be responded to are in the PlayerModelManagerModelEvent enum.
//
// Returns true if callback successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setCallback(PlayerModelManagerHandle h, PlayerModelManagerEventHandler *callback, void *userdata));

// Set eye flipbook textures for this model. Passed in array is expected to be at least 8 elements large.
//
// Returns true if successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setEyesTextures(PlayerModelManagerHandle h, TexturePtr eyesTextures[]));

// Set mouth flipbook textures for this model. Passed in array is expected to be at least 4 elements large.
//
// Returns true if successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setMouthTextures(PlayerModelManagerHandle h, TexturePtr mouthTextures[]));

// Get a static pointer to a DL in a particular form. The visual will be automatically updated if the model for that form changes.
//
// If an invalid display list ID is passed in, NULL will be returned.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, Gfx *PlayerModelManager_getFormDisplayList(unsigned long apiVersion, PlayerTransformation form, PlayerModelManagerDisplayListId dlId));

// Helper define for PlayerModelManager_getDL. See PlayerModelManager_getDL description for functionality.
#define PLAYERMODELMANAGER_GET_FORM_DISPLAY_LIST(form, displayListId) PlayerModelManager_getDL(PMM_API_VERSION, form, displayListId)

// Returns true if the model attached to the passed in handle is currently equipped, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_isApplied(PlayerModelManagerHandle h));

#define PLAYERMODELMANAGER_CALLBACK_REGISTER_MODELS RECOMP_CALLBACK(YAZMT_PMM_MOD_NAME, onRegisterModels)

#endif

#endif
