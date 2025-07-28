#include "modding.h"
#include "recomputils.h"
#include "global.h"
#include "recompdata.h"
#include "modelreplacer_compat.h"
#include "playermodelmanager.h"
#include "globalobjects_api.h"
#include "modelreplacer_api.h"
#include "model_shared.h"
#include "model_common.h"
#include "modelreplacements.h"

static U32ValueHashmapHandle sObjectIdToVanillaDLToListenerMapMap;
static U32ValueHashmapHandle sLinkDLEntryToListeners[PLAYER_FORM_MAX];
static U32HashsetHandle sExcludedDisplayLists;

MRC_ListenerInfo *getListenerInfo(ObjectId id, Gfx *vanillaDL) {
    if (!recomputil_u32_value_hashmap_contains(sObjectIdToVanillaDLToListenerMapMap, id)) {
        return NULL;
    }

    U32MemoryHashmapHandle vanillaDLMap;
    recomputil_u32_value_hashmap_get(sObjectIdToVanillaDLToListenerMapMap, id, &vanillaDLMap);

    return recomputil_u32_memory_hashmap_get(vanillaDLMap, (uintptr_t)vanillaDL);
}

MRC_ListenerInfo *MRC_getListenerFromFormAndDL(PlayerTransformation form, Link_DisplayList entryDLId) {
    unsigned long infoPtr;

    if (recomputil_u32_value_hashmap_get(sLinkDLEntryToListeners[form], entryDLId, &infoPtr)) {
        return (MRC_ListenerInfo *)infoPtr;
    }

    return NULL;
}

void createListenerInfo(ObjectId id, Gfx *vanillaDL, PlayerTransformation form, Link_DisplayList linkDLId) {
    if (!recomputil_u32_value_hashmap_contains(sObjectIdToVanillaDLToListenerMapMap, id)) {
        recomputil_u32_value_hashmap_insert(sObjectIdToVanillaDLToListenerMapMap, id, recomputil_create_u32_memory_hashmap(sizeof(MRC_ListenerInfo)));
    }

    U32MemoryHashmapHandle vanillaDLMap;
    recomputil_u32_value_hashmap_get(sObjectIdToVanillaDLToListenerMapMap, id, &vanillaDLMap);

    MRC_ListenerInfo *listener = recomputil_u32_memory_hashmap_get(vanillaDLMap, (uintptr_t)vanillaDL);

    if (!listener) {
        recomputil_u32_memory_hashmap_create(vanillaDLMap, (uintptr_t)vanillaDL);
        listener = recomputil_u32_memory_hashmap_get(vanillaDLMap, (uintptr_t)vanillaDL);
        listener->form = form;
        listener->linkDLId = linkDLId;
        listener->objId = id;
        listener->vanillaDL = vanillaDL;

        if (form == MRC_PLAYER_FORM_EVERY) {
            for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
                recomputil_u32_value_hashmap_insert(sLinkDLEntryToListeners[i], linkDLId, (uintptr_t)listener);
            }
        } else {
            recomputil_u32_value_hashmap_insert(sLinkDLEntryToListeners[form], linkDLId, (uintptr_t)listener);
        }
    }
}

void MRC_setupListenerDL(ObjectId id, Gfx *vanillaDL, PlayerTransformation form, Link_DisplayList linkDLId) {
    createListenerInfo(id, vanillaDL, form, linkDLId);

    Gfx **models = NULL;

    if (form == MRC_PLAYER_FORM_EVERY) {
        models = gSharedDisplayLists;
    } else {
        models = gLinkFormProxies[form].vanilla.models;
    }

    models[linkDLId] = GlobalObjects_getGlobalGfxPtr(id, vanillaDL);
}

RECOMP_CALLBACK(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, onModelChange)
void updateListenerDLs_on_onModelChange(ObjectId id, Gfx *vanillaDL, Gfx *newDL) {
    if (recomputil_u32_hashset_contains(sExcludedDisplayLists, (uintptr_t)newDL)) {
        return;
    }

    MRC_ListenerInfo *listener = getListenerInfo(id, vanillaDL);

    if (listener) {
        Link_DisplayList linkDLId = listener->linkDLId;
        PlayerTransformation form = listener->form;

        if (listener->form == MRC_PLAYER_FORM_EVERY) {
            if (gSharedDisplayLists[linkDLId] != newDL) {
                gSharedDisplayLists[linkDLId] = newDL;

                for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
                    requestRefreshFormProxyDL(&gLinkFormProxies[i], linkDLId);
                }
            }
        } else {
            if (gLinkFormProxies[form].vanilla.models[linkDLId] != newDL) {
                gLinkFormProxies[form].vanilla.models[linkDLId] = newDL;
                requestRefreshFormProxyDL(&gLinkFormProxies[form], linkDLId);
            }
        }
    }
}

RECOMP_CALLBACK(".", _internal_initHashObjects)
void initModelReplacerHashObjects() {
    sObjectIdToVanillaDLToListenerMapMap = recomputil_create_u32_value_hashmap();

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        sLinkDLEntryToListeners[i] = recomputil_create_u32_value_hashmap();
    }

    sExcludedDisplayLists = recomputil_create_u32_hashset();

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        for (int j = 0; j < PLAYER_FORM_MAX; ++j) {
            recomputil_u32_hashset_insert(sExcludedDisplayLists, (uintptr_t)&gLinkFormProxies[j].displayLists[i]);
        }
    }
}

RECOMP_DECLARE_EVENT(_internal_onReadyModelReplacerCompat())

RECOMP_CALLBACK(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, onBeforeRegisterReplacers)
void initReplacerCompat_on_event() {
    _internal_onReadyModelReplacerCompat();
}
