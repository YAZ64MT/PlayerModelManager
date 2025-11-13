#include "global.h"
#include "modelentry.h"
#include "recomputils.h"
#include "playermodelmanager_utils.h"
#include "modelentrymanager.h"
#include "equipmentoverrides.h"
#include "model_common.h"
#include "yazmtcorelib_api.h"
#include "modelentrymanager.h"
#include "logger.h"
#include "assets/objects/object_link_child/object_link_child.h"

static U32HashsetHandle sValidModelEntryPtrs;

static bool isValidModelEntry(const ModelEntry *entry) {
    return recomputil_u32_hashset_contains(sValidModelEntryPtrs, (uintptr_t)entry);
}

static void printInvalidPtrWarning(const char *s) {
    Logger_printWarning("PlayerModelManager: %s reveived an invalid pointer", s);
}

typedef struct ModelEntryFunctions {
    bool (*applyToModelInfo)(const struct ModelEntry *entry);
    bool (*removeFromModelInfo)(const struct ModelEntry *entry);
    bool (*setDisplayList)(struct ModelEntry *entry, Link_DisplayList id, Gfx *dl);
    bool (*setMatrix)(struct ModelEntry *entry, Link_EquipmentMatrix id, Mtx *mtx);
} ModelEntryVirtualFunctions;

typedef struct ModelEntry {
    const ModelEntryVirtualFunctions *virtualFuncs;
    PlayerModelManagerModelType type;
    Link_CustomModelCategory category;
    char *displayName;
    char *internalName;
    char *authorName;
    PlayerModelManagerEventHandler *callback;
    void *callbackData;
    PlayerModelManagerHandle handle;
    u64 flags;
    U32ValueHashmapHandle displayListPtrs;
    U32ValueHashmapHandle mtxPtrs;
} ModelEntry;

typedef struct ModelEntryForm {
    ModelEntry modelEntry;
    FlexSkeletonHeader *skel;
    FlexSkeletonHeader *shieldingSkel;
    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];
} ModelEntryForm;

typedef struct ModelEntryEquipment {
    ModelEntry modelEntry;
    Link_EquipmentReplacement equipType;
} ModelEntryEquipment;

typedef struct ModelEntryPack {
    ModelEntry modelEntry;
    YAZMTCore_IterableU32Set *modelEntries;
} ModelEntryPack;

RECOMP_DECLARE_EVENT(_internal_onFormModelApplied(PlayerTransformation form));
RECOMP_DECLARE_EVENT(_internal_onEquipmentModelApplied(Link_EquipmentReplacement eq));

static bool ModelEntry_applyToModelInfo_default(const ModelEntry *entry) {
    Logger_printError("PlayerModelManager: Model Entry with internal name %s is using ModelEntry_applyToModelInfo (should have been overriden!)\n", entry->internalName ? entry->internalName : "[UNKNOWN]");
    tryCrashGame();
    return false;
}

static bool ModelEntry_removeFromModelInfo_default(const ModelEntry *entry) {
    Logger_printError("PlayerModelManager: Model Entry with internal name %s is using ModelEntry_removeFromModelInfo (should have been overriden!)\n", entry->internalName ? entry->internalName : "[UNKNOWN]");
    tryCrashGame();
    return false;
}

static bool ModelEntry_setDisplayList_default(ModelEntry *entry, Link_DisplayList id, Gfx *dl) {
    if (id >= LINK_DL_MAX || id < 0) {
        return false;
    }

    recomputil_u32_value_hashmap_insert(entry->displayListPtrs, id, (uintptr_t)dl);

    return true;
}

static bool ModelEntry_setMatrix_default(ModelEntry *entry, Link_EquipmentMatrix id, Mtx *mtx) {
    if (id >= LINK_EQUIP_MATRIX_MAX || id < 0) {
        return false;
    }

    recomputil_u32_value_hashmap_insert(entry->mtxPtrs, id, (uintptr_t)mtx);

    return true;
}

bool ModelEntry_init(ModelEntry *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {

    Link_CustomModelCategory cat = getCategoryFromModelType(type);

    if (!isValidCategory(cat)) {
        Logger_printError("PlayerModelmanager: Trying to initialize ModelEntry with invalid PlayerModelManagerModelType %d", type);
        tryCrashGame();
        return false;
    }

    if (!internalName) {
        Logger_printError("PlayerModelmanager: Trying to initialize ModelEntry with type %d with NULL internal name!\n", type);
        tryCrashGame();
        return false;
    }

    if (!handle) {
        Logger_printError("PlayerModelmanager: Trying to initialize ModelEntry with invalid PlayerModelManagerHandle %d", handle);
        tryCrashGame();
        return false;
    }

    static ModelEntryVirtualFunctions funcs = {
        .applyToModelInfo = ModelEntry_applyToModelInfo_default,
        .removeFromModelInfo = ModelEntry_removeFromModelInfo_default,
        .setDisplayList = ModelEntry_setDisplayList_default,
        .setMatrix = ModelEntry_setMatrix_default,
    };

    entry->virtualFuncs = &funcs;
    entry->type = type;
    entry->category = cat;
    entry->displayName = NULL;
    entry->internalName = YAZMTCore_Utils_StrDup(internalName);
    entry->authorName = NULL;
    entry->flags = 0;
    entry->callback = NULL;
    entry->callbackData = NULL;
    entry->handle = handle;
    entry->displayListPtrs = recomputil_create_u32_value_hashmap();
    entry->mtxPtrs = recomputil_create_u32_value_hashmap();
    recomputil_u32_hashset_insert(sValidModelEntryPtrs, (uintptr_t)entry);

    return true;
}

bool ModelEntry_applyToModelInfo(const ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return false;
    }

    return entry->virtualFuncs->applyToModelInfo(entry);
}

bool ModelEntry_removeFromModelInfo(const ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return false;
    }

    return entry->virtualFuncs->removeFromModelInfo(entry);
}

Gfx *ModelEntry_getDisplayList(const ModelEntry *entry, Link_DisplayList id) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return false;
    }

    uintptr_t ret = 0;
    recomputil_u32_value_hashmap_get(entry->displayListPtrs, id, &ret);
    return (Gfx *)ret;
}

bool ModelEntry_setDisplayList(ModelEntry *entry, Link_DisplayList id, Gfx *dl) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return false;
    }

    return entry->virtualFuncs->setDisplayList(entry, id, dl);
}

Mtx *ModelEntry_getMatrix(const ModelEntry *entry, Link_EquipmentMatrix id) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return false;
    }

    uintptr_t ret = 0;
    recomputil_u32_value_hashmap_get(entry->mtxPtrs, id, &ret);
    return (Mtx *)ret;
}

bool ModelEntry_setMatrix(ModelEntry *entry, Link_EquipmentMatrix id, Mtx *matrix) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return false;
    }

    return entry->virtualFuncs->setMatrix(entry, id, matrix);
}

bool ModelEntryForm_applyToModelInfo(const ModelEntry *thisx) {
    PlayerTransformation form = getFormFromModelType(thisx->type);

    if (form >= PLAYER_FORM_MAX) {
        return false;
    }

    Link_ModelInfo *modelInfo = &gLinkFormProxies[form].current.modelInfo;

    clearLinkModelInfo(modelInfo);

    ModelEntryForm *this = (ModelEntryForm *)(thisx);

    for (int i = 0; i < LINK_DL_MAX; ++i) {
        modelInfo->models[i] = ModelEntry_getDisplayList(&this->modelEntry, i);
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        modelInfo->equipMtx[i] = ModelEntry_getMatrix(&this->modelEntry, i);
    }

    modelInfo->flags = this->modelEntry.flags;

    modelInfo->skeleton = this->skel;

    modelInfo->shieldingSkeleton = this->shieldingSkel;

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        if (this->eyesTex[i]) {
            modelInfo->mouthTextures[i] = this->mouthTex[i];
        }
    }

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        if (this->eyesTex[i]) {
            modelInfo->eyesTextures[i] = this->eyesTex[i];
        }
    }

    _internal_onFormModelApplied(form);

    return true;
}

bool ModelEntryForm_removeFromModelInfo(const ModelEntry *thisx) {
    PlayerTransformation form = getFormFromModelType(thisx->type);

    Link_FormProxy *proxy = &gLinkFormProxies[form];

    clearLinkModelInfo(&proxy->current.modelInfo);

    _internal_onFormModelApplied(form);

    return true;
}

static bool ModelEntryForm_init(ModelEntryForm *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {
    if (!ModelEntry_init(&entry->modelEntry, handle, type, internalName)) {
        return false;
    }

    if (!isFormCategory(entry->modelEntry.category)) {
        Logger_printError("PlayerModelManager: ModelEntryForm with internal name %s had non-form type %d passed in!\n", internalName, type);
        tryCrashGame();
        return false;
    }

    for (int i = 0; i < LINK_EQUIP_MATRIX_MAX; ++i) {
        ModelEntry_setMatrix_default(&entry->modelEntry, i, NULL);
    }

    static ModelEntryVirtualFunctions funcs = {
        .applyToModelInfo = ModelEntryForm_applyToModelInfo,
        .removeFromModelInfo = ModelEntryForm_removeFromModelInfo,
        .setDisplayList = ModelEntry_setDisplayList_default,
        .setMatrix = ModelEntry_setMatrix_default,
    };

    entry->modelEntry.virtualFuncs = &funcs;
    entry->skel = NULL;
    entry->shieldingSkel = NULL;
    Lib_MemSet(entry->mouthTex, 0, sizeof(entry->mouthTex));
    Lib_MemSet(entry->eyesTex, 0, sizeof(entry->eyesTex));

    return true;
}

const char *ModelEntry_getInternalName(const ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return entry->internalName;
}

const char *ModelEntry_getDisplayName(const ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return entry->displayName;
}

void ModelEntry_setDisplayName(ModelEntry *entry, const char *name) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    recomp_free(entry->displayName);
    
    entry->displayName = name ? YAZMTCore_Utils_StrDup(name) : NULL;
}

const char *ModelEntry_getAuthorName(const ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return entry->authorName;
}

void ModelEntry_setAuthorName(ModelEntry *entry, const char *name) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    recomp_free(entry->authorName);

    entry->authorName = name ? YAZMTCore_Utils_StrDup(name) : NULL;
}

Link_CustomModelCategory ModelEntry_getCategory(const ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return LINK_CMC_MAX;
    }

    return entry->category;
}

PlayerModelManagerModelType ModelEntry_getType(const ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return PMM_MODEL_TYPE_NONE;
    }

    return entry->type;
}

void ModelEntry_doCallback(const ModelEntry *entry, PlayerModelManagerModelEvent eventId) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    if (entry->callback) {
        entry->callback(entry->handle, eventId, entry->callbackData);
    }
}

void ModelEntry_setFlags(ModelEntry *entry, u64 flags) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    entry->flags |= flags;
}

void ModelEntry_unsetFlags(ModelEntry *entry, u64 flags) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    entry->flags &= (~flags);
}

void ModelEntry_unsetAllFlags(ModelEntry *entry) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    entry->flags = 0;
}

bool ModelEntry_isAnyFlagEnabled(ModelEntry *entry, u64 flags) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return false;
    }

    return entry->flags & flags;
}

void ModelEntry_setCallback(ModelEntry *entry, PlayerModelManagerEventHandler *callback, void *data) {
    if (!isValidModelEntry(entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    entry->callback = callback;
    entry->callbackData = data;
}

ModelEntryForm *ModelEntryForm_new(PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {
    ModelEntryForm *entry = recomp_alloc(sizeof(ModelEntryForm));

    if (!entry) {
        Logger_printError("PlayerModelManager: Failed to allocate memory for ModelEntryForm!");
        tryCrashGame();
        return NULL;
    }

    if (!ModelEntryForm_init(entry, handle, type, internalName)) {
        Logger_printError("PlayerModelManager: Failed to initialize ModelEntryForm!");
        tryCrashGame();
        recomp_free(entry);
        return NULL;
    }

    return entry;
}

ModelEntry *ModelEntryForm_getModelEntry(ModelEntryForm *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return &entry->modelEntry;
}

TexturePtr ModelEntryForm_getEyesTexture(ModelEntryForm *entry, PlayerEyeIndex i) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return 0;
    }

    return entry->eyesTex[i];
}

TexturePtr ModelEntryForm_getMouthTexture(ModelEntryForm *entry, PlayerMouthIndex i) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return 0;
    }

    return entry->mouthTex[i];
}

static TexturePtr sDefaultEyesTextures[PLAYER_EYES_MAX] = {
    gLinkHumanEyesOpenTex,        // PLAYER_EYES_OPEN
    gLinkHumanEyesHalfTex,        // PLAYER_EYES_HALF
    gLinkHumanEyesClosedTex,      // PLAYER_EYES_CLOSED
    gLinkHumanEyesRollRightTex,   // PLAYER_EYES_ROLL_RIGHT
    gLinkHumanEyesRollLeftTex,    // PLAYER_EYES_ROLL_LEFT
    gLinkHumanEyesRollUpTex,      // PLAYER_EYES_ROLL_UP
    gLinkHumanEyesRollDownTex,    // PLAYER_EYES_ROLL_DOWN
    object_link_child_Tex_003800, // PLAYER_EYES_7
};

static TexturePtr sDefaultMouthTextures[PLAYER_MOUTH_MAX] = {
    gLinkHumanMouthClosedTex, // PLAYER_MOUTH_CLOSED
    gLinkHumanMouthTeethTex,  // PLAYER_MOUTH_TEETH
    gLinkHumanMouthAngryTex,  // PLAYER_MOUTH_ANGRY
    gLinkHumanMouthHappyTex,  // PLAYER_MOUTH_HAPPY
};

void ModelEntryForm_setEyesTexture(ModelEntryForm *entry, TexturePtr tex, PlayerEyeIndex i) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    if (i < 0 || i >= PLAYER_EYES_MAX) {
        Logger_printWarning("PlayerModelManager: %s received invalid index %d", __PRETTY_FUNCTION__, i);
        return;
    }

    entry->eyesTex[i] = tex;
}

void ModelEntryForm_setMouthTexture(ModelEntryForm *entry, TexturePtr tex, PlayerMouthIndex i) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    if (i < 0 || i >= PLAYER_MOUTH_MAX) {
        Logger_printWarning("PlayerModelManager: %s received invalid index %d", __PRETTY_FUNCTION__, i);
        return;
    }

    entry->mouthTex[i] = tex;
}

void ModelEntryForm_fillDefaultFaceTextures(ModelEntryForm *entry) {
    for (PlayerEyeIndex i = 0; i < PLAYER_EYES_MAX; ++i) {
        ModelEntryForm_setEyesTexture(entry, sDefaultEyesTextures[i], i);
    }

    for (PlayerMouthIndex i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        ModelEntryForm_setMouthTexture(entry, sDefaultMouthTextures[i], i);
    }
}

void ModelEntryForm_setDLsFromSkeletons(ModelEntryForm *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

#define SET_LIMB_DL(pLimb, id)                                                                                                           \
    {                                                                                                                                    \
        if (!ModelEntry_getDisplayList(&entry->modelEntry, id)) {                                                                        \
            ModelEntry_setDisplayList_default(&entry->modelEntry, id, (limbs[pLimb - 1]->dLists[0]) ? (limbs[pLimb - 1]->dLists[0]) : gEmptyDL); \
        }                                                                                                                                \
    }                                                                                                                                    \
    (void)0

    if (entry->skel) {
        LodLimb **limbs = (LodLimb **)entry->skel->sh.segment;
        SET_LIMB_DL(PLAYER_LIMB_WAIST, LINK_DL_WAIST);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_THIGH, LINK_DL_RTHIGH);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_SHIN, LINK_DL_RSHIN);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_FOOT, LINK_DL_RFOOT);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_THIGH, LINK_DL_LTHIGH);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_SHIN, LINK_DL_LSHIN);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_FOOT, LINK_DL_LFOOT);
        SET_LIMB_DL(PLAYER_LIMB_HEAD, LINK_DL_HEAD);
        SET_LIMB_DL(PLAYER_LIMB_HAT, LINK_DL_HAT);
        SET_LIMB_DL(PLAYER_LIMB_COLLAR, LINK_DL_COLLAR);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_SHOULDER, LINK_DL_LSHOULDER);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_FOREARM, LINK_DL_LFOREARM);
        SET_LIMB_DL(PLAYER_LIMB_LEFT_HAND, LINK_DL_LHAND);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_SHOULDER, LINK_DL_RSHOULDER);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_FOREARM, LINK_DL_RFOREARM);
        SET_LIMB_DL(PLAYER_LIMB_RIGHT_HAND, LINK_DL_RHAND);
        SET_LIMB_DL(PLAYER_LIMB_SHEATH, LINK_DL_SHEATH_NONE);
        SET_LIMB_DL(PLAYER_LIMB_TORSO, LINK_DL_TORSO);
    }

#undef SET_LIMB_DL

#define SET_SHIELDING_LIMB_DL(pLimb, id)                                                                                         \
    {                                                                                                                            \
        if (!ModelEntry_getDisplayList(&entry->modelEntry, id)) {                                                                \
            ModelEntry_setDisplayList_default(&entry->modelEntry, id, (limbs[pLimb - 1]->dList) ? (limbs[pLimb - 1]->dList) : gEmptyDL); \
        }                                                                                                                        \
    }                                                                                                                            \
    (void)0

    if (entry->shieldingSkel) {
        StandardLimb **limbs = (StandardLimb **)entry->shieldingSkel->sh.segment;
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_BODY, LINK_DL_BODY_SHIELD_BODY);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_HEAD, LINK_DL_BODY_SHIELD_HEAD);
        SET_SHIELDING_LIMB_DL(LINK_BODY_SHIELD_LIMB_ARMS_AND_LEGS, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
    }

#undef SET_SHIELDING_LIMB_DL
}

void ModelEntryForm_setSkeleton(ModelEntryForm *entry, FlexSkeletonHeader *skel) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    entry->skel = skel;
}

FlexSkeletonHeader *ModelEntryForm_getSkeleton(ModelEntryForm *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return entry->skel;
}

void ModelEntryForm_setShieldingSkeleton(ModelEntryForm *entry, FlexSkeletonHeader *skel) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return;
    }

    entry->shieldingSkel = skel;
}

FlexSkeletonHeader *ModelEntryForm_getShieldingSkeleton(ModelEntryForm *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return entry->shieldingSkel;
}

bool ModelEntryEquipment_setDisplayList(ModelEntry *thisx, Link_DisplayList id, Gfx *dl) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)((void *)thisx);

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    // Count generally is very small (count < 10), so a simple linear search will do
    for (size_t i = 0; i < override->dl.count; ++i) {
        if (id == override->dl.overrides[i]) {
            return ModelEntry_setDisplayList_default(thisx, id, dl);
        }
    }

    return false;
}

bool ModelEntryEquipment_setMatrix(ModelEntry *thisx, Link_EquipmentMatrix id, Mtx *mtx) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)((void *)thisx);

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    // Count generally is very small (count < 10), so a simple linear search will do
    for (size_t i = 0; i < override->mtx.count; ++i) {
        if (id == override->mtx.overrides[i]) {
            return ModelEntry_setMatrix_default(thisx, id, mtx);
        }
    }

    return false;
}

bool ModelEntryEquipment_applyToModelInfo(const ModelEntry *thisx) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)((void *)thisx);

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_ModelInfoCustom *modelInfoCustom = &gLinkFormProxies[i].current;

        for (size_t i = 0; i < override->dl.count; ++i) {
            Link_DisplayList id = override->dl.overrides[i];
            uintptr_t dl = 0;
            recomputil_u32_value_hashmap_get(this->modelEntry.displayListPtrs, id, &dl);
            recomputil_u32_value_hashmap_insert(modelInfoCustom->gfxOverrides, id, dl);
        }

        for (size_t i = 0; i < override->mtx.count; ++i) {
            Link_EquipmentMatrix id = override->mtx.overrides[i];
            uintptr_t mtx = 0;
            recomputil_u32_value_hashmap_get(this->modelEntry.mtxPtrs, id, &mtx);
            recomputil_u32_value_hashmap_insert(modelInfoCustom->mtxOverrides, id, mtx);
        }
    }

    _internal_onEquipmentModelApplied(this->equipType);

    return true;
}

bool ModelEntryEquipment_removeFromModelInfo(const ModelEntry *thisx) {
    ModelEntryEquipment *this = (ModelEntryEquipment *)thisx;

    const EquipmentOverride *override = &gEquipmentOverrideTable[this->equipType];

    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        Link_FormProxy *fp = &gLinkFormProxies[i];

        for (size_t j = 0; j < override->dl.count; ++j) {
            recomputil_u32_value_hashmap_erase(fp->current.gfxOverrides, override->dl.overrides[j]);
        }

        for (size_t j = 0; j < override->mtx.count; ++j) {
            recomputil_u32_value_hashmap_erase(fp->current.mtxOverrides, override->mtx.overrides[j]);
        }
    }

    _internal_onEquipmentModelApplied(this->equipType);

    return true;
}

static bool ModelEntryEquipment_init(ModelEntryEquipment *entry, PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {
    if (!ModelEntry_init(&entry->modelEntry, handle, type, internalName)) {
        return false;
    }

    if (!isEquipmentCategory(entry->modelEntry.category)) {
        Logger_printError("PlayerModelManager: ModelEntryEquipment with internal name %s had non-equipment type %d passed in!\n", internalName, type);
        tryCrashGame();
        return false;
    }

    static ModelEntryVirtualFunctions funcs = {
        .applyToModelInfo = ModelEntryEquipment_applyToModelInfo,
        .removeFromModelInfo = ModelEntryEquipment_removeFromModelInfo,
        .setDisplayList = ModelEntryEquipment_setDisplayList,
        .setMatrix = ModelEntryEquipment_setMatrix,
    };

    entry->modelEntry.virtualFuncs = &funcs;
    entry->equipType = getEquipmentReplacementFromCategory(entry->modelEntry.category);

    return true;
}

ModelEntryEquipment *ModelEntryEquipment_new(PlayerModelManagerHandle handle, PlayerModelManagerModelType type, char *internalName) {
    ModelEntryEquipment *entry = recomp_alloc(sizeof(ModelEntryEquipment));

    if (!entry) {
        Logger_printError("PlayerModelManager: Failed to allocate memory for ModelEntryEquipment!");
        tryCrashGame();
        return NULL;
    }

    if (!ModelEntryEquipment_init(entry, handle, type, internalName)) {
        Logger_printError("PlayerModelManager: Failed to initialize ModelEntryEquipment!");
        tryCrashGame();
        recomp_free(entry);
        return NULL;
    }

    return entry;
}

ModelEntry *ModelEntryEquipment_getModelEntry(ModelEntryEquipment *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return &entry->modelEntry;
}

bool ModelEntryPack_setDisplayList(ModelEntry *thisx, Link_DisplayList id, Gfx *dl) {
    return false;
}

bool ModelEntryPack_setMatrix(ModelEntry *thisx, Link_EquipmentMatrix id, Mtx *mtx) {
    return false;
}

bool ModelEntryPack_applyToModelInfo(const ModelEntry *thisx) {
    const ModelEntryPack *this = (const ModelEntryPack *)thisx;

    size_t numEntries = ModelEntryPack_getModelEntriesCount(this);
    ModelEntry const *const *modelEntries = ModelEntryPack_getModelEntries(this);

    for (size_t i = 0; i < numEntries; ++i) {
        CMEM_tryApplyEntry(modelEntries[i]->category, modelEntries[i]);
    }

    return true;
}

bool ModelEntryPack_removeFromModelInfo(const ModelEntry *this) {
    return true;
}

bool ModelEntryPack_init(ModelEntryPack *entry, PlayerModelManagerHandle handle, char *internalName) {
    if (!ModelEntry_init(&entry->modelEntry, handle, PMM_MODEL_TYPE_MODEL_PACK, internalName)) {
        return false;
    }

    static ModelEntryVirtualFunctions funcs = {
        .applyToModelInfo = ModelEntryPack_applyToModelInfo,
        .removeFromModelInfo = ModelEntryPack_removeFromModelInfo,
        .setDisplayList = ModelEntryPack_setDisplayList,
        .setMatrix = ModelEntryPack_setMatrix,
    };
    entry->modelEntry.virtualFuncs = &funcs;
    entry->modelEntries = YAZMTCore_IterableU32Set_new();

    return true;
}

ModelEntryPack *ModelEntryPack_new(PlayerModelManagerHandle handle, char *internalName) {
    ModelEntryPack *entry = recomp_alloc(sizeof(ModelEntryPack));

    if (!entry) {
        Logger_printError("PlayerModelManager: Failed to allocate memory for ModelEntryPack!");
        tryCrashGame();
        return NULL;
    }

    if (!ModelEntryPack_init(entry, handle, internalName)) {
        Logger_printError("PlayerModelManager: Failed to initialize ModelEntryPack!");
        tryCrashGame();
        recomp_free(entry);
        return NULL;
    }

    return entry;
}

ModelEntry *ModelEntryPack_getModelEntry(ModelEntryPack *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return &entry->modelEntry;
}

ModelEntry const *const *ModelEntryPack_getModelEntries(const ModelEntryPack *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    return (ModelEntry const *const *)YAZMTCore_IterableU32Set_values(entry->modelEntries);
}

size_t ModelEntryPack_getModelEntriesCount(const ModelEntryPack *entry) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return 0;
    }

    return YAZMTCore_IterableU32Set_size(entry->modelEntries);
}

static YAZMTCore_DynamicU32Array *sQueuedPacks;

static bool isEntryInPack(const ModelEntryPack *pack, const ModelEntry *entry) {
    YAZMTCore_DynamicU32Array_clear(sQueuedPacks);

    YAZMTCore_DynamicU32Array_push(sQueuedPacks, (uintptr_t)pack);

    do {
        size_t packsCount = YAZMTCore_DynamicU32Array_size(sQueuedPacks);

        ModelEntryPack *currPack = (ModelEntryPack *)YAZMTCore_DynamicU32Array_data(sQueuedPacks)[packsCount - 1];

        if (YAZMTCore_IterableU32Set_contains(pack->modelEntries, (uintptr_t)entry)) {
            return true;
        }

        YAZMTCore_DynamicU32Array_pop(sQueuedPacks);

        const ModelEntry *const *packEntries = ModelEntryPack_getModelEntries(currPack);
        size_t entryCount = ModelEntryPack_getModelEntriesCount(currPack);

        for (size_t i = 0; i < entryCount; ++i) {
            const ModelEntry *currEntry = packEntries[i];

            if (isPackCategory(ModelEntry_getCategory(currEntry))) {
                YAZMTCore_DynamicU32Array_push(sQueuedPacks, (uintptr_t)currEntry);
            }
        }

    } while (YAZMTCore_DynamicU32Array_size(sQueuedPacks) > 0);
    
    return false;
}

bool ModelEntryPack_addEntryToPack(ModelEntryPack *entry, ModelEntry *entryToAdd) {
    if (!isValidModelEntry((ModelEntry *)entry)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    if (!isValidModelEntry((ModelEntry *)entryToAdd)) {
        printInvalidPtrWarning(__PRETTY_FUNCTION__);
        return NULL;
    }

    if (!isPackCategory(ModelEntry_getCategory(entryToAdd)) || !isEntryInPack((ModelEntryPack *)entryToAdd, &entry->modelEntry)) {
        YAZMTCore_IterableU32Set_insert(entry->modelEntries, (uintptr_t)entryToAdd);
        return true;
    }

    return false;
}

RECOMP_CALLBACK(".", _internal_initHashObjects)
void initModelEntryObjects() {
    sQueuedPacks = YAZMTCore_DynamicU32Array_new();

    sValidModelEntryPtrs = recomputil_create_u32_hashset();
}
