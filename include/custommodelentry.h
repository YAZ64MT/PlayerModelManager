#ifndef __CUSTOMMODELENTRY_H__
#define __CUSTOMMODELENTRY_H__

#include "global.h"
#include "playermodelmanager.h"

typedef enum {
    CUSTOM_MODEL_TYPE_CHILD,
    CUSTOM_MODEL_TYPE_ADULT,
    CUSTOM_MODEL_TYPE_DEKU,
    CUSTOM_MODEL_TYPE_GORON,
    CUSTOM_MODEL_TYPE_ZORA,
    CUSTOM_MODEL_TYPE_FIERCE_DEITY
} CUSTOM_MODEL_TYPE;

typedef struct {
    u64 flags;
    char *displayName;
    char *filePath;
    void (*onModelLoad)(void *userdata);
    void *onModelLoadData;
    void (*onModelUnload)(void *userdata);
    void *onModelUnloadData;
    Gfx *displayListPtrs[LINK_DL_MAX];
} CustomModelEntry;

void CustomModelEntry_init(CustomModelEntry *entry);

#endif
