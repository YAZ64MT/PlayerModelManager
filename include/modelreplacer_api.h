#ifndef __MODEL_REPLACER_API__
#define __MODEL_REPLACER_API__

#include "global.h"
#include "modding.h"

typedef unsigned long ZModelReplacerHandle;

#define YAZMT_Z64_MODEL_REPLACER_MOD_NAME "yazmt_mm_modelreplacer"

// TODO: DOCUMENT
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, ZModelReplacerHandle ZModelReplacer_createReplacer(ObjectId id, Gfx *vanillaDL));

RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_destroyReplacer(ZModelReplacerHandle handle));

RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_setReplacerModel(ZModelReplacerHandle handle, Gfx *customDL));

// TODO: UPDATE
// Replaces the display list (DL) at the passed in segmented pointer in the given object id.
//
// Custom display lists are added in a stack, where the list at the top of the stack is the one displayed.
//
// If other custom DLs are added after this, the most recently added DL will be drawn instead of this one.
// But if all DLs "above" this one in the stack are removed, this one will be drawn again.
//
// If the passed in custom DL was already in the stack, it will be moved to the top.
//
// Returns true if model added to top of stack, false otherwise
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_pushReplacer(ZModelReplacerHandle handle));

// Removes the custom display list from the custom display list stack.
//
// Returns true if removal ocurred, false if nothing happened.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_removeReplacer(ZModelReplacerHandle handle));

#endif
