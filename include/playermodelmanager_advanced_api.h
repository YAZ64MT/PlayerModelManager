#ifndef PLAYERMODELMANAGER_ADVANCED_API_H
#define PLAYERMODELMANAGER_ADVANCED_API_H

// All appearance information, including assigned models, colors, skeletons, and more is held
// in AppearanceData struct.
//
// An AppearanceDataHandle is a reference to one of those structs
//
// Multiple distinct handles with can reference the same AppearanceData
//
// An AppearanceData struct is destroyed sometime after all its references are released
typedef unsigned long AppearanceDataHandle;

#define INVALID_APPEARANCE_DATA_HANDLE ((AppearanceDataHandle)0)

#ifndef YAZMT_PMM_NO_API_IMPORTS

#include "PR/ultratypes.h"
#include "playermodelmanager_api.h"
#include "stdbool.h"
#include "modding.h"
#include "color.h"
#include "z64animation.h"
#include "z64player.h"

// Changes the color of the tunic on all forms on Link and Kafei.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, void PlayerModelManager_requestOverrideTunicColor(u8 r, u8 g, u8 b, u8 a));

// Changes the color of the tunic on a specific form of Link and Kafei.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, void PlayerModelManager_requestOverrideFormTunicColor(PlayerTransformation form, u8 r, u8 g, u8 b, u8 a));

// Returns true if actor is using the model represented by handle h, false otherwise.
//
// Models in form categories take form into account. As a simple example, if the actor
// is Link in Zora form and h represents a model with category PMM_MODEL_TYPE_GORON, then
// this will return false even if h represents the model equipped to his Goron form.
//
// Equipment models do not take form into account.
//
// If h represents a PMM_MODEL_TYPE_MODEL_PACK, this function always returns false.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_isModelApplied(Actor *actor, PlayerModelManagerHandle h));

// Returns a pointer to the display list with the given ID on an actor with PMM appearance data.
//
// Returns NULL if the actor does not have appearance data or if the passed in id is invalid.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, Gfx *PlayerModelManager_Actor_getDisplayList(Actor *actor, PlayerModelManagerDisplayListId dlId));

// Returns a pointer to the matrix with the given ID on an actor with PMM appearance data.
//
// Returns NULL if the actor does not have appearance data or if the passed in id is invalid.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, Mtx *PlayerModelManager_Actor_getMatrix(Actor *actor, PlayerModelManagerMatrixId mtxId));

// Returns true if the actor has PMM appearance data assigned to it, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_hasAppearanceData(Actor *actor));

// Gets the actor's tunic color according to its AppearanceData and form model type. Returns true and writes the color to out if successful, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_getTunicColor(Actor *actor, Color_RGBA8 *out));

// Sets the form model type of the actor. Returns true if successfully set, false otherwise.
//
// Valid types include PMM_MODEL_TYPE_CHILD, PMM_MODEL_TYPE_ADULT, PMM_MODEL_TYPE_DEKU, PMM_MODEL_TYPE_GORON,
// PMM_MODEL_TYPE_ZORA, and PMM_MODEL_TYPE_FIERCE_DEITY
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_setFormModelType(Actor *actor, PlayerModelManagerModelType type));

// Returns the PlayerModelManagerModelType of the actor's assigned AppearanceData, as set by PlayerModelManager_Actor_setFormModelType
//
// Returns PMM_MODEL_TYPE_NONE if the actor has no appearance data
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, PlayerModelManagerModelType PlayerModelManager_Actor_getFormModelType(Actor *actor));

// Writes a FlexSkeletonHeader to dest based on the Actor's AppearanceData and FormModelType
//
// For example, if the actor has PMM_MODEL_TYPE_DEKU as its FormModelType, then drawing the FlexSkeletonHeader
// written by this function will draw the Deku Link model assigned to PMM_MODEL_TYPE_DEKU
//
// This skeleton will have the same number of limbs as Link's normal model (21) and is expected to play Link animations
//
// Returns true if the FlexSkeletonHeader was written, false if it could not be (e.g. if the actor has no AppearanceData)
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_getModelSkeleton(Actor *actor, FlexSkeletonHeader *dest));

// Writes a FlexSkeletonHeader to dest based on the Actor's AppearanceData and FormModelType
//
// For example, if the actor has PMM_MODEL_TYPE_GORON as its FormModelType, then drawing the FlexSkeletonHeader
// written by this function will draw the shielding Goron Link model assigned to PMM_MODEL_TYPE_GORON
//
// This skeleton will have the same number of limbs as Link's shielding model (4) and is expected to play Link's Goron shielding animation
//
// Returns true if the FlexSkeletonHeader was written, false if it could not be (e.g. if the actor has no AppearanceData)
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_getModelShieldingSkeleton(Actor *actor, FlexSkeletonHeader *dest));

// Copies internal name of model into outBuf, provided outBuf is large enough. Returns true if the name was successfully copied, false otherwise
//
// To account for the NULL terminator, a buffer of at least size (PMM_MAX_INTERNAL_NAME_LENGTH + 1) should be used
//
// Returns true if the name was successfully written, false if the actor has no appearance data or no model assigned to that type
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_getModelInternalName(Actor *actor, PlayerModelManagerModelType type, char outBuf[], size_t outBufSize));

// Returns a TexturePtr to the eyes texture with the passed in index based on the actor's AppearanceData and FormModelType
//
// Returns NULL if the eyes texture can't be found (e.g. the actor has no AppearanceData or an invalid index is passed in)
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, TexturePtr PlayerModelManager_Actor_getEyesTexturePtr(Actor *actor, PlayerEyeIndex index));

// Returns a TexturePtr to the mouth texture with the passed in index based on the actor's AppearanceData and FormModelType
//
// Returns NULL if the texture can't be found (e.g. the actor has no AppearanceData or an invalid index is passed in)
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, TexturePtr PlayerModelManager_Actor_getMouthTexturePtr(Actor *actor, PlayerMouthIndex index));

// Returns a NEW AppearanceDataHandle that points to the same AppearanceData as the actor's handle
//
// As this is a NEW AppearanceDataHandle, you should release it with PlayerModelManager_AppearanceData_releaseHandle
// when you are done with it to avoid leaking memory
//
// Returns INVALID_APPEARANCE_DATA_HANDLE if the actor does not have AppearanceData assigned to it
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, AppearanceDataHandle PlayerModelManager_Actor_getAppearanceDataHandle(Actor *actor));

// Creates a new AppearanceData struct and returns a handle to it
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, AppearanceDataHandle PlayerModelManager_AppearanceData_createData(void));

// Releases a reference to an AppearanceData struct
//
// It is important to do this when you are done using a handle, because AppearanceData can only be destroyed 
// after all its handles are relesed
//
// Returns true if the handle was released, false otherwise
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_releaseHandle(AppearanceDataHandle h));

// Assigns the AppearanceData struct h references to the actor
//
// The actor creates a new AppearanceDataHandle and will release it automatically upon destruction, so you do not need to wait
// to release h if you are done with it
//
// Returns true if the AppearanceData was assigned to the actor, false otherwise
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_assignDataToActor(Actor *actor, AppearanceDataHandle h));

// Gives dest a reference to the same AppearanceData as src and copies the ModelType of src to dest
//
// Returns true if the data was successfully set. Returns false and leaves dest unchanged otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_assignDataFromOtherActor(Actor *dest, Actor *src));

// Assigns a model to an AppearanceData struct and updates its the display lists
//
// If there is a registered model with this internal name, and its model type is compatible with the passed in type,
// then it is assigned to that category, and the AppearanceData's display lists are updated
//
// If an unregistered internal name is passed in, then the custom model assigned to the passed in type is removed
//
// Returns true if a model was successfully assigned
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_assignModel(AppearanceDataHandle h, PlayerModelManagerModelType type, const char *internalName));

// Removes the custom model in the AppearanceData for the passed in model type and updates its the display lists
//
// Returns true if the model was removed, false otherwise
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_removeModel(AppearanceDataHandle h, PlayerModelManagerModelType type));

// Sets the tunic color of the AppearanceData for the passed in model type
//
// Valid types include PMM_MODEL_TYPE_CHILD, PMM_MODEL_TYPE_ADULT, PMM_MODEL_TYPE_DEKU, PMM_MODEL_TYPE_GORON,
// PMM_MODEL_TYPE_ZORA, and PMM_MODEL_TYPE_FIERCE_DEITY
//
// Returns true if the color was set, false otherwise
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_setTunicColor(AppearanceDataHandle h, PlayerModelManagerModelType type, Color_RGBA8 color));

// Writes the tunic color of the AppearanceData for the passed in model type to out
//
// Valid types include PMM_MODEL_TYPE_CHILD, PMM_MODEL_TYPE_ADULT, PMM_MODEL_TYPE_DEKU, PMM_MODEL_TYPE_GORON,
// PMM_MODEL_TYPE_ZORA, and PMM_MODEL_TYPE_FIERCE_DEITY
//
// If the tunic color was not assigned by PlayerModelManager_AppearanceData_setTunicColor, then the tunic color in the mod's config is written
//
// Returns true if out was written to, false otherwise
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_getTunicColor(AppearanceDataHandle h, PlayerModelManagerModelType type, Color_RGBA8 *out));

// Copies internal name of model into outBuf, provided outBuf is large enough. Returns true if the name was successfully copied, false otherwise
//
// To account for the NULL terminator, a buffer of at least size (PMM_MAX_INTERNAL_NAME_LENGTH + 1) should be used
//
// Returns true if the name was successfully written, false if there is no model assigned to that type
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_getModelInternalName(AppearanceDataHandle h, PlayerModelManagerModelType type, char outBuf[], size_t outBufSize));

// Sets a minimum lifetime for the AppearanceData referenced by h
//
// AppearanceData will not be destroyed for at least lifetime playstate updates, even if there are no references remaining
//
// Returns true if the lifetime was successfully set, false otherwise
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_setMinimumLifetime(AppearanceDataHandle h, u32 lifetime));

// Returns a reference to the AppearanceData used by the Player actor by default
//
// The AppearanceDataHandle returned by this function does not need to be freed
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, AppearanceDataHandle PlayerModelManager_AppearanceData_getMainPlayerHandle(void));

// Returns a reference to the AppearanceData used by the Kafei actor by default
//
// The AppearanceDataHandle returned by this function does not need to be freed
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, AppearanceDataHandle PlayerModelManager_AppearanceData_getKafeiHandle(void));

// Hides this model entry from the model menu.
//
// Returns true if the entry was hidden, false otherwise 
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_setHiddenFromModelMenu(PlayerModelManagerHandle h, bool isHidden));

#endif

#endif
