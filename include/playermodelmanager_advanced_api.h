#ifndef PLAYERMODELMANAGER_ADVANCED_API_H
#define PLAYERMODELMANAGER_ADVANCED_API_H

#include "PR/ultratypes.h"
#include "playermodelmanager_api.h"
#include "stdbool.h"
#include "modding.h"
#include "color.h"

typedef unsigned long AppearanceDataHandle;

#ifndef YAZMT_PMM_NO_API_IMPORTS

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

// Sets the form model type of the actor. Returns true if successfully set, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_setFormModelType(Actor *actor, PlayerModelManagerModelType type));

// Gets the actor's tunic color according to its AppearanceData and form model type. Returns true and writes the color to out if successful, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_Actor_getTunicColor(Actor *actor, Color_RGBA8 *out));

// Copies internal name of model into outBuf, provided outBuf is large enough. Returns true if the name was successfully copied, false otherwise.
RECOMP_IMPORT(YAZMT_PMM_MOD_NAME, bool PlayerModelManager_AppearanceData_getModelInternalName(AppearanceDataHandle h, PlayerModelManagerModelType type, char outBuf[], size_t outBufSize));

#endif

#endif
