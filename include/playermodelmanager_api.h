#ifndef __PLAYERMODELMANAGER_API__
#define __PLAYERMODELMANAGER_API__

#include "global.h"
#include "modding.h"

typedef unsigned long PlayerModelManagerFormHandle;

// Used for keeping compatibility between versions
// DO NOT EDIT
#define PMM_API_VERSION 1UL

#define YAZMT_PMM_MOD_NAME "yazmt_mm_playermodelmanager"

typedef enum {
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
    PMM_DL_LFIST,
    PMM_DL_LHAND_BOTTLE,
    PMM_DL_LHAND_GUITAR,
    PMM_DL_RFIST,
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
    PMM_DL_SHIELD1,
    PMM_DL_SHIELD2,
    PMM_DL_SHIELD3,
    PMM_DL_SHIELD3_RAY,
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
    PMM_DL_FPS_LFOREARM,
    PMM_DL_FPS_LHAND,
    PMM_DL_FPS_RFOREARM,
    PMM_DL_FPS_RHAND,
    PMM_DL_MASK_SKULL,
    PMM_DL_MASK_SPOOKY,
    PMM_DL_MASK_GERUDO,
    PMM_DL_MASK_TRUTH,
    PMM_DL_MASK_KAFEIS_MASK,
    PMM_DL_MASK_ALL_NIGHT,
    PMM_DL_MASK_BUNNY,
    PMM_DL_MASK_KEATON,
    PMM_DL_MASK_GARO,
    PMM_DL_MASK_ROMANI,
    PMM_DL_MASK_CIRCUS_LEADER,
    PMM_DL_MASK_COUPLE,
    PMM_DL_MASK_POSTMAN,
    PMM_DL_MASK_GREAT_FAIRY,
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
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_HUMAN,
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU,
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_GORON,
    PMM_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA,
    PMM_DL_BOOMERANG,
    PMM_DL_HAMMER,
    PMM_DL_UPGRADE_LFOREARM,
    PMM_DL_UPGRADE_LHAND,
    PMM_DL_UPGRADE_LFIST,
    PMM_DL_UPGRADE_RFOREARM,
    PMM_DL_UPGRADE_RHAND,
    PMM_DL_UPGRADE_RFIST,
    PMM_DL_BOOT_LIRON,
    PMM_DL_BOOT_RIRON,
    PMM_DL_BOOT_LHOVER,
    PMM_DL_BOOT_RHOVER,
    PMM_DL_DEKU_GUARD,
    PMM_DL_PIPE_MOUTH,
    PMM_DL_PIPE_RIGHT,
    PMM_DL_PIPE_UP,
    PMM_DL_PIPE_DOWN,
    PMM_DL_PIPE_LEFT,
    PMM_DL_PIPE_A,
    PMM_DL_STEM_RIGHT,
    PMM_DL_STEM_LEFT,
    PMM_DL_PETAL_PARTICLE,
    PMM_DL_FLOWER_PROPELLER_CLOSED,
    PMM_DL_FLOWER_CENTER_CLOSED,
    PMM_DL_FLOWER_PROPELLER_OPEN,
    PMM_DL_FLOWER_CENTER_OPEN,
    PMM_DL_PAD_WOOD,
    PMM_DL_PAD_GRASS,
    PMM_DL_PAD_OPENING,
    PMM_DL_FIRE_PUNCH,
    PMM_DL_DRUM_STRAP,
    PMM_DL_DRUM_UP,
    PMM_DL_DRUM_LEFT,
    PMM_DL_DRUM_RIGHT,
    PMM_DL_DRUM_DOWN,
    PMM_DL_DRUM_A,
    PMM_DL_BODY_SHIELD_HEAD,
    PMM_DL_BODY_SHIELD_BODY,
    PMM_DL_BODY_SHIELD_ARMS_AND_LEGS,
    PMM_DL_CURLED,
    PMM_DL_SPIKES,
    PMM_DL_INIT_FIRE,
    PMM_DL_FIRE_ROLL,
    PMM_DL_LFIN,
    PMM_DL_LFIN_SWIM,
    PMM_DL_RFIN,
    PMM_DL_RFIN_SWIM,
    PMM_DL_FIN_SHIELD,
    PMM_DL_MAGIC_BARRIER,
    PMM_DL_GUITAR,
    PMM_DL_PEDESTAL_SWORD,
    PMM_DL_MAX
} PlayerModelManager_DisplayListId;

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
    PMM_MTX_SHIELD1_ODD,
    PMM_MATRIX_MAX,
} PlayerModelManager_MatrixId;

#define PMM_MTX_SWORD_KOKIRI_BACK PMM_MTX_SWORD1_BACK
#define PMM_MTX_SWORD_RAZOR_BACK PMM_MTX_SWORD2_BACK
#define PMM_MTX_SWORD_GILDED_BACK PMM_MTX_SWORD3_BACK

#define PMM_MTX_SHIELD_HERO_BACK PMM_MTX_SHIELD2_BACK
#define PMM_MTX_SHIELD_MIRROR_BACK PMM_MTX_SHIELD3_BACK

typedef enum {
    PMM_FORM_MODEL_TYPE_NONE,
    PMM_FORM_MODEL_TYPE_CHILD,
    PMM_FORM_MODEL_TYPE_ADULT,
    PMM_FORM_MODEL_TYPE_DEKU,
    PMM_FORM_MODEL_TYPE_GORON,
    PMM_FORM_MODEL_TYPE_ZORA,
    PMM_FORM_MODEL_TYPE_FIERCE_DEITY,
    PMM_FORM_MODEL_TYPE_MAX
} PlayerModelManager_FormModelType;

typedef enum {
    PMM_EVENT_MODEL_APPLIED,
    PMM_EVENT_MODEL_REMOVED,
} PlayerModelManager_ModelEvent;

typedef void PlayerModelManagerEventHandler(PlayerModelManagerFormHandle handle, PlayerModelManager_ModelEvent event, void *userdata);

#ifndef YAZMT_PMM_NO_API_IMPORTS

// Registers a new player model and returns a handle to it.
//
// A handle represents an entry in the in-game model manager menu, so hold onto it.
//
// Choose a unique string identifier. It will not show up in the menu if you set a display name, so it need not be human readable.
// There is a maximum length of 255 characters.
//
// Choose a model type from the PlayerModelManager_FormModelType enum. For now, only PMM_FORM_MODEL_TYPE_CHILD and PMM_FORM_MODEL_TYPE_ADULT are supported.
//
// This function can only be used during the PlayerModelManager_onRegisterModels event. Otherwise, an invalid handle will be returned.
#define PLAYERMODELMANAGER_REGISTER_FORM_MODEL(internalName, modelType) PlayerModelManager_registerFormModel(PMM_API_VERSION, id, modelType)
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, PlayerModelManagerFormHandle PlayerModelManager_registerFormModel(unsigned long apiVersion, char *id, PlayerModelManager_FormModelType modelType));

// Sets the name that will appear in the menu for the passed in model handle.
//
// Limited to 64 characters.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setDisplayName(PlayerModelManagerFormHandle h, char *displayName));

// Sets the name that will appear in the author field of the menu.
//
// Limited to 128 characters.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setAuthor(PlayerModelManagerFormHandle h, char *author));

// Set a display list on the model. The ID can be obtained from PlayerModelManager_DisplayListId
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setDL(PlayerModelManagerFormHandle h, PlayerModelManager_DisplayListId dlId, Gfx *dl));

RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setMtx(PlayerModelManagerFormHandle h, PlayerModelManager_MatrixId mtxId, Mtx *matrix));

RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setCallback(PlayerModelManagerFormHandle h, PlayerModelManagerEventHandler *callback, void *userdata));

RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setSkeleton(PlayerModelManagerFormHandle h, FlexSkeletonHeader *skel));

RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setEyesTextures(PlayerModelManagerFormHandle h, TexturePtr eyesTextures[PLAYER_EYES_MAX]));

RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setMouthTextures(PlayerModelManagerFormHandle h, TexturePtr mouthTextures[PLAYER_MOUTH_MAX]));

RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, Gfx *PlayerModelManager_getDL(unsigned long apiVersion, PlayerTransformation form, PlayerModelManager_DisplayListId dl));

RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_isApplied(PlayerModelManagerFormHandle h));

#define PLAYERMODELMANAGER_GET_FORM_DISPLAY_LIST(form, displayListId) PlayerModelManager_getDL(PMM_API_VERSION, form, displayListId)

#define PLAYERMODELMANAGER_CALLBACK_REGISTER_MODELS RECOMP_CALLBACK(YAZMT_PMM_MOD_NAME, onRegisterModels)

#endif

#endif
