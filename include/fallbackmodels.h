#ifndef FALLBACKMODELS_H
#define FALLBACKMODELS_H

#include "playermodelmanager_api.h"

typedef struct ModelEntryForm ModelEntryForm;

ModelEntryForm *gHumanModelEntry;
ModelEntryForm *gDekuModelEntry;
ModelEntryForm *gGoronModelEntry;
ModelEntryForm *gZoraModelEntry;
ModelEntryForm *gFierceDeityModelEntry;

PlayerModelManagerHandle gHumanModelHandle;
PlayerModelManagerHandle gDekuModelHandle;
PlayerModelManagerHandle gGoronModelHandle;
PlayerModelManagerHandle gZoraModelHandle;
PlayerModelManagerHandle gFierceDeityModelHandle;

#endif
