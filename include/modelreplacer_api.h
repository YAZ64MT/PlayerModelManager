#ifndef __MODEL_REPLACER_API__
#define __MODEL_REPLACER_API__

#include "global.h"
#include "modding.h"

#define YAZMT_Z64_MODEL_REPLACER_MOD_NAME "yazmt_mm_modelreplacer"

// Replaces the display list (DL) at the passed in segmented pointer in the given object id.
//
// Custom display lists are added in a stack, where the list at the top of the stack is the one displayed.
//
// If other custom DLs are added after this, the most recently added DL will be drawn instead of this one.
// But if all DLs "above" this one in the stack are removed, this one will be drawn again.
//
// If the passed in custom DL was already in the stack, it will be moved to the top.
//
// Returns true if model added to top of stack, false otherwise.
//
// Usage Example:
// ZModelReplacer_addCustomDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL, myCoolSwordModel)
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_addCustomDL(ObjectId id, Gfx *vanillaDL, Gfx *customDL));

// Removes the custom display list from the custom display list stack.
//
// Returns true if removal ocurred, false if nothing happened.
//
// Usage Example:
// ZModelReplacer_removeCustomDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL, myCoolSwordModel)
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_removeCustomDL(ObjectId id, Gfx *vanillaDL, Gfx *customDL));

// If a model has not been replaced during this play session, the object will need to be reloaded before
// its replacement shows up.
//
// This function is useful if you want an object to have its vanilla appearance at the start but want
// any replacements to be immediately visible.
//
// Returns true on sucessful pre-registration, false otherwise.
//
// Usage Example:
// ZModelReplacer_preregisterDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL)
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_preregisterDL(ObjectId id, Gfx *vanillaDL));

#endif
