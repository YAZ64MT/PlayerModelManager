#ifndef __GLOBAL_OBJECTS_API__
#define __GLOBAL_OBJECTS_API__

#include "global.h"
#include "modding.h"

#define YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME "yazmt_mm_global_objects"

// Helper macro for converting segmented pointers to global pointers.
//
// If you are trying to get a pointer to a vanilla object, you should grab a global object pointer
// with ZGlobalObj_getGlobalObject first.
//
// You generally should NOT use this for Gfx pointers since their DL commands need to be handled.
//
// For a Gfx global pointer, use ZGlobalObj_getGlobalGfxPtr instead.
#define SEGMENTED_TO_GLOBAL_PTR(globalObj, segmentedPtr) ((void *)((uintptr_t)globalObj + SEGMENT_OFFSET(segmentedPtr)))

// Converts the segmented pointers of the Gfx commands in a display list to their global pointers.
//
// If a gSPBranchList or gSPDisplayList command is encountered, this function is called recursively
// on the DL pointed to by the respective command.
RECOMP_IMPORT(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, void ZGlobalObj_globalizeDL(void *obj, Gfx *segmentedPtr));

// Converts a LodLimb skeleton's segmented limb pointers and limb display list pointers to global pointers.
//
// Objects that use LodLimb skeletons include Link's forms, Kafei, and so on.
RECOMP_IMPORT(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, void ZGlobalObj_globalizeLodLimbSkeleton(void *obj, FlexSkeletonHeader *skel));

// Converts a StandardLimb skeleton's segmented limb pointers and limb display list pointer to global pointers.
//
// Most NPCs use this skeleton type.
RECOMP_IMPORT(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, void ZGlobalObj_globalizeStandardLimbSkeleton(void *obj, FlexSkeletonHeader *skel));

// Gets a pointer to the start of the globally loaded object with the passed in ID.
//
// Returns NULL if an invalid ID is passed in.
RECOMP_IMPORT(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, void *ZGlobalObj_getGlobalObject(ObjectId id));

// Helper function for grabbing the object ID of the object stored at vromStart.
//
// Returns true if there is an object that starts at the given address, and the object id is written to out.
//
// Returns false if there is no object ID associated with the given vrom address and leaves out unmodified.
RECOMP_IMPORT(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, bool ZGlobalObj_getObjectIdFromVrom(uintptr_t vromStart, unsigned long *out));

// Gets the global object associated with a given vrom address.
RECOMP_IMPORT(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, void *ZGlobalObj_getGlobalObjectFromVrom(uintptr_t vromStart));

// Gets the global Gfx pointer to a display list. Repoints commands that share a segment with the passed in pointer.
RECOMP_IMPORT(YAZMT_Z64_GLOBAL_OBJECTS_MOD_NAME, Gfx *ZGlobalObj_getGlobalGfxPtr(ObjectId id, Gfx *segmentedPtr));

#endif
