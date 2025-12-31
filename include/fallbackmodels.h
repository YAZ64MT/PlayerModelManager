#ifndef FALLBACKMODELS_H
#define FALLBACKMODELS_H

#include "playermodelmanager_api.h"
#include "z64animation.h"

typedef struct ModelEntryForm ModelEntryForm;

PlayerModelManagerHandle gHumanModelHandle;
PlayerModelManagerHandle gDekuModelHandle;
PlayerModelManagerHandle gGoronModelHandle;
PlayerModelManagerHandle gZoraModelHandle;
PlayerModelManagerHandle gFierceDeityModelHandle;

// FlexSkeletonHeader should contain a pointer to an array of LodLimb limbs
void FallbackModelsCommon_doCommonAssignments(PlayerModelManagerHandle h, FlexSkeletonHeader *skel, void *seg06, void *seg04);

#endif
