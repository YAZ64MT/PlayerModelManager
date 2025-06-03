#include "global.h"
#include "custommodelentry.h"

void CustomModelEntry_init(CustomModelEntry *entry) {
    entry->displayName = NULL;
    entry->filePath = NULL;
    entry->flags = 0;
    entry->onModelLoad = NULL;
    entry->onModelLoadData = NULL;
    entry->onModelUnload = NULL;
    entry->onModelUnloadData = NULL;
    for (unsigned i = 0; i < LINK_DL_MAX; ++i) {
        entry->displayListPtrs[i] = NULL;
    }
}
