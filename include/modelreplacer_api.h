#ifndef __MODEL_REPLACER_API__
#define __MODEL_REPLACER_API__

#include "global.h"
#include "modding.h"

typedef unsigned long ZModelReplacerHandle;

#define YAZMT_Z64_MODEL_REPLACER_MOD_NAME "yazmt_mm_modelreplacer"

// Create an empty Replacer and returns a handle to it
//
// Replacer handles are how your mod will interface with ModelReplacer, so hold onto it.
//
// Each Replacer is specific to a particular object and display list within that object, so
// if you want to replace multiple display lists within an object, you will need a separate
// Replacer for each display list.
//
// Note that this does NOT automatically add the Replacer to the stack, so make sure to call
// ZModelReplacer_pushReplacer at some point after you create it.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, ZModelReplacerHandle ZModelReplacer_createReplacer(ObjectId id, Gfx *vanillaDL));

// Destroys a Replacer
//
// Returns true if Replacer was destroyed, false if nothing happened.
//
// If you know for a fact you will never need a handle again, destroy it to avoid a memory leak.
//
// Though, if you just want to temporarily remove a model and add it back later, consider using
// ZModelReplacer_removeReplacer or setting the display list to NULL with ZModelReplacer_setReplacerModel 
// instead.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_destroyReplacer(ZModelReplacerHandle handle));

// Sets the display list inside the Replacer
//
// Returns true if display list successfully set, false otherwise.
//
// You may call it to switch out the display list at any time, but if your Replacer is not
// at the top of the stack, it will not appear. The change will be reflected if your model
// ever returns to the top of the stack, though.
//
// A NULL custom display list will cause this Replacer to be skipped but will preserve its
// position in the stack if a non-NULL display list is ever set.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_setReplacerModel(ZModelReplacerHandle handle, Gfx *customDL));

// Pushes a Replacer to the top of the stack
//
// Returns true if Replacer successfully pushed, false otherwise.
//
// Replacers are held in a stack, and the Replacer at the top of the stack is the one rendered.
// This puts the Replacer passed in at the top of the stack, making it appear immediately.
//
// If this Replacer was already in the stack, it will lose its old position when it is moved
// to the top.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_pushReplacer(ZModelReplacerHandle handle));

// Removes the Replacer from the Replacer stack
//
// Returns true if removal ocurred, false if nothing happened.
//
// Removes a Replacer from the stack. To get it to render again, you must push it back on
// at some point.
RECOMP_IMPORT(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, bool ZModelReplacer_removeReplacer(ZModelReplacerHandle handle));

#endif
