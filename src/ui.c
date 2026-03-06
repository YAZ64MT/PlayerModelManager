#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "recompui.h"
#include "modelentrymanager.h"
#include "yazmtcorelib_api.h"
#include "logger.h"
#include "recompdata.h"
#include "modelentry.h"

static bool sIsFileListRefreshRequested;

static void requestRefreshFileList(void);
static void onUpOneLevelButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata);

typedef struct CategoryInfo {
    int index;
    const char *displayName;
    bool isVisible;
    bool isUsedByCurrentGame;
    const ModelEntry *realEntry;
    PlayerModelManagerModelType category;
    bool isNeedsDiskSave;
} CategoryInfo;

static bool sIsForceAllCategoriesVisible = false;

#define DECLARE_DEFAULT_CAT_INFO(name, isUsed, modelType, isVisibleByDefault) {.displayName = name, .isVisible = isVisibleByDefault, .isUsedByCurrentGame = isUsed, .realEntry = NULL, .category = modelType, .isNeedsDiskSave = false}
#define DECLARE_CAT_INFO(name, isUsed, modelType) DECLARE_DEFAULT_CAT_INFO(name, isUsed, modelType, false)

#define CAT_USED_MM true

// Always opposite of MM
#define CAT_USED_OOT (!CAT_USED_MM)

// used by both games
#define CAT_USED_Z64 true

static CategoryInfo sCategoryInfos[] = {
    // At least one category must be visible or the category selector goes into an infinite loop
    DECLARE_CAT_INFO("Model Packs", CAT_USED_Z64, PMM_MODEL_TYPE_MODEL_PACK),
    DECLARE_DEFAULT_CAT_INFO("Young Link", CAT_USED_OOT, PMM_MODEL_TYPE_CHILD, CAT_USED_OOT),
    DECLARE_DEFAULT_CAT_INFO("Adult Link", CAT_USED_OOT, PMM_MODEL_TYPE_ADULT, CAT_USED_OOT),
    DECLARE_DEFAULT_CAT_INFO("Human", CAT_USED_MM, PMM_MODEL_TYPE_CHILD, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Deku", CAT_USED_MM, PMM_MODEL_TYPE_DEKU, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Goron", CAT_USED_MM, PMM_MODEL_TYPE_GORON, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Zora", CAT_USED_MM, PMM_MODEL_TYPE_ZORA, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Fierce Deity", CAT_USED_MM, PMM_MODEL_TYPE_FIERCE_DEITY, CAT_USED_MM),
    DECLARE_CAT_INFO("Kokiri Sword", CAT_USED_Z64, PMM_MODEL_TYPE_SWORD1),
    DECLARE_CAT_INFO("Razor Sword", CAT_USED_MM, PMM_MODEL_TYPE_SWORD2),
    DECLARE_CAT_INFO("Gilded Sword", CAT_USED_Z64, PMM_MODEL_TYPE_SWORD3),
    DECLARE_CAT_INFO("Fierce Deity's Sword", CAT_USED_Z64, PMM_MODEL_TYPE_SWORD4),
    DECLARE_CAT_INFO("Great Fairy's Sword", CAT_USED_MM, PMM_MODEL_TYPE_SWORD5),
    DECLARE_CAT_INFO("Deku Shield", CAT_USED_OOT, PMM_MODEL_TYPE_SHIELD1),
    DECLARE_CAT_INFO("Hero's Shield", CAT_USED_Z64, PMM_MODEL_TYPE_SHIELD2),
    DECLARE_CAT_INFO("Mirror Shield", CAT_USED_Z64, PMM_MODEL_TYPE_SHIELD3),
    DECLARE_CAT_INFO("Hookshot", CAT_USED_Z64, PMM_MODEL_TYPE_HOOKSHOT),
    DECLARE_CAT_INFO("Bow", CAT_USED_Z64, PMM_MODEL_TYPE_BOW),
    DECLARE_CAT_INFO("Slingshot", CAT_USED_OOT, PMM_MODEL_TYPE_SLINGSHOT),
    DECLARE_CAT_INFO("Bottle", CAT_USED_Z64, PMM_MODEL_TYPE_BOTTLE),
    DECLARE_CAT_INFO("Boomerang", CAT_USED_OOT, PMM_MODEL_TYPE_BOOMERANG),
    DECLARE_CAT_INFO("Hammer", CAT_USED_OOT, PMM_MODEL_TYPE_HAMMER),
    DECLARE_CAT_INFO("Deku Stick", CAT_USED_Z64, PMM_MODEL_TYPE_DEKU_STICK),
    DECLARE_CAT_INFO("Bomb", CAT_USED_Z64, PMM_MODEL_TYPE_BOMB),
    DECLARE_CAT_INFO("Bombchu", CAT_USED_Z64, PMM_MODEL_TYPE_BOMBCHU),
    DECLARE_CAT_INFO("Fairy Ocarina", CAT_USED_OOT, PMM_MODEL_TYPE_OCARINA_FAIRY),
    DECLARE_CAT_INFO("Ocarina of Time", CAT_USED_Z64, PMM_MODEL_TYPE_OCARINA_TIME),
    DECLARE_CAT_INFO("Deku Pipes", CAT_USED_MM, PMM_MODEL_TYPE_PIPES),
    DECLARE_CAT_INFO("Goron Drums", CAT_USED_MM, PMM_MODEL_TYPE_DRUMS),
    DECLARE_CAT_INFO("Zora Guitar", CAT_USED_MM, PMM_MODEL_TYPE_GUITAR),
    DECLARE_CAT_INFO("Skull Mask", CAT_USED_OOT, PMM_MODEL_TYPE_MASK_SKULL),
    DECLARE_CAT_INFO("Spooky Mask", CAT_USED_OOT, PMM_MODEL_TYPE_MASK_SPOOKY),
    DECLARE_CAT_INFO("Gerudo Mask", CAT_USED_OOT, PMM_MODEL_TYPE_MASK_GERUDO),
    DECLARE_CAT_INFO("Mask of Truth", CAT_USED_Z64, PMM_MODEL_TYPE_MASK_TRUTH),
    DECLARE_CAT_INFO("Kafei's Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_KAFEIS_MASK),
    DECLARE_CAT_INFO("All Night Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_ALL_NIGHT),
    DECLARE_CAT_INFO("Bunny Hood", CAT_USED_Z64, PMM_MODEL_TYPE_MASK_BUNNY),
    DECLARE_CAT_INFO("Keaton Mask", CAT_USED_Z64, PMM_MODEL_TYPE_MASK_KEATON),
    DECLARE_CAT_INFO("Garo Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_GARO),
    DECLARE_CAT_INFO("Romani Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_ROMANI),
    DECLARE_CAT_INFO("Circus Leader's Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_CIRCUS_LEADER),
    DECLARE_CAT_INFO("Couple's Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_COUPLE),
    DECLARE_CAT_INFO("Postman's Hat", CAT_USED_MM, PMM_MODEL_TYPE_MASK_POSTMAN),
    DECLARE_CAT_INFO("Great Fairy Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_GREAT_FAIRY),
    DECLARE_CAT_INFO("Gibdo Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_GIBDO),
    DECLARE_CAT_INFO("Don Gero's Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_DON_GERO),
    DECLARE_CAT_INFO("Kamaro Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_KAMARO),
    DECLARE_CAT_INFO("Captain's Hat", CAT_USED_MM, PMM_MODEL_TYPE_MASK_CAPTAIN),
    DECLARE_CAT_INFO("Stone Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_STONE),
    DECLARE_CAT_INFO("Bremen Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_BREMEN),
    DECLARE_CAT_INFO("Blast Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_BLAST),
    DECLARE_CAT_INFO("Mask of Scents", CAT_USED_MM, PMM_MODEL_TYPE_MASK_SCENTS),
    DECLARE_CAT_INFO("Giant's Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_GIANT),
    DECLARE_CAT_INFO("Deku Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_DEKU),
    DECLARE_CAT_INFO("Goron Mask", CAT_USED_Z64, PMM_MODEL_TYPE_MASK_GORON),
    DECLARE_CAT_INFO("Zora Mask", CAT_USED_Z64, PMM_MODEL_TYPE_MASK_ZORA),
    DECLARE_CAT_INFO("Fierce Deity Mask", CAT_USED_MM, PMM_MODEL_TYPE_MASK_FIERCE_DEITY),
};

#define SELECTING_CATEGORY -99

static int sCurrentCategoryInfo = 0;

static bool isValidCategoryInfoIndex(int i) {
    return i >= 0 && i < ARRAY_COUNT(sCategoryInfos);
}

static bool isSelectingCategory(void) {
    return false;
}

static bool isSelectingModel(void) {
    return isValidCategoryInfoIndex(sCurrentCategoryInfo);
}

static CategoryInfo *getCurrentCategoryInfo(void) {
    if (isSelectingModel()) {
        return &sCategoryInfos[sCurrentCategoryInfo];
    } else {
        Logger_printWarning("getCurrentCategoryInfo found invalid sCurrentCategoryInfo value %d\n", sCurrentCategoryInfo);
    }

    return NULL;
}

typedef struct {
    RecompuiColor borderColor;
    RecompuiColor bgColor;
} ButtonColor;

static const ButtonColor sPrimaryButtonColor = {
    .borderColor = {
        .r = 185,
        .g = 125,
        .b = 242,
        .a = 204,
    },
    .bgColor = {
        .r = 185,
        .g = 125,
        .b = 242,
        .a = 13,
    },
};

static const ButtonColor sSecondaryButtonColor = {
    .borderColor = {
        .r = 23,
        .g = 214,
        .b = 232,
        .a = 204,
    },
    .bgColor = {
        .r = 23,
        .g = 214,
        .b = 232,
        .a = 13,
    },
};

static const ButtonColor sModelSelectedButtonColor = {
    .borderColor = {
        .r = 227,
        .g = 151,
        .b = 75,
        .a = 204,
    },
    .bgColor = {
        .r = 227,
        .g = 151,
        .b = 75,
        .a = 13,
    },
};

static const ButtonColor sModelRemovedButtonColor = {
    .borderColor = {
        .r = 224,
        .g = 74,
        .b = 89,
        .a = 204,
    },
    .bgColor = {
        .r = 224,
        .g = 74,
        .b = 89,
        .a = 13,
    },
};

static void setAllNavDirsToAuto(RecompuiResource id) {
    recompui_set_nav_auto(id, NAVDIRECTION_DOWN);
    recompui_set_nav_auto(id, NAVDIRECTION_UP);
    recompui_set_nav_auto(id, NAVDIRECTION_LEFT);
    recompui_set_nav_auto(id, NAVDIRECTION_RIGHT);
}

static RecompuiResource createListBoxButton(RecompuiContext context, RecompuiResource parent, const char *text, RecompuiButtonStyle style) {
    RecompuiResource button = recompui_create_button(context, parent, text, style);
    recompui_set_flex_shrink(button, 0);
    recompui_set_width(button, 100.0f, UNIT_PERCENT);
    recompui_set_text_align(button, TEXT_ALIGN_CENTER);

    return button;
}

// Connect a list of buttons vertically with the first entry at the top
//
// Left and right inputs will navigate to the top and bottom of the list,
// respectively
static void connectListBoxButtons(const RecompuiResource buttons[], size_t n) {
    if (n < 2) {
        return;
    }

    RecompuiResource first = buttons[0];
    RecompuiResource last = buttons[n - 1];

    for (size_t i = 1; i < n - 1; ++i) {
        RecompuiResource curr = buttons[i];
        RecompuiResource next = buttons[i + 1];

        recompui_set_nav(buttons[i], NAVDIRECTION_DOWN, next);
        recompui_set_nav(buttons[i], NAVDIRECTION_LEFT, first);
        recompui_set_nav(buttons[i], NAVDIRECTION_RIGHT, last);

        recompui_set_nav(next, NAVDIRECTION_UP, curr);
    }

    recompui_set_nav(first, NAVDIRECTION_DOWN, buttons[1]);
    recompui_set_nav(first, NAVDIRECTION_RIGHT, last);

    recompui_set_nav(last, NAVDIRECTION_UP, buttons[n - 2]);
    recompui_set_nav(last, NAVDIRECTION_LEFT, first);
}

YAZMTCore_IterableU32Set *sModelListButtons;

static const RecompuiResource *getCurrentModelButtonArray(void) {
    return YAZMTCore_IterableU32Set_values(sModelListButtons);
}

static size_t getCurrentModelButtonArraySize(void) {
    return YAZMTCore_IterableU32Set_size(sModelListButtons);
}

YAZMTCore_IterableU32Set *sCategoryListButtons;

static const RecompuiResource *getCategoryButtonArray(void) {
    return YAZMTCore_IterableU32Set_values(sCategoryListButtons);
}

static size_t getCategoryButtonArraySize(void) {
    return YAZMTCore_IterableU32Set_size(sCategoryListButtons);
}

static void pushButtonToList(YAZMTCore_IterableU32Set *buttonList, RecompuiResource button) {
    YAZMTCore_IterableU32Set_insert(buttonList, button);
}

RecompuiResource createAndPushButtonToList(RecompuiContext context, RecompuiResource parent, const char *text, RecompuiButtonStyle style, YAZMTCore_IterableU32Set *buttonList) {
    RecompuiResource button = createListBoxButton(context, parent, text, style);
    pushButtonToList(buttonList, button);
    return button;
}

U32ValueHashmapHandle sModelButtonsToData;
U32ValueHashmapHandle sCategoryButtonsToData;

static const void *getListButtonData(U32ValueHashmapHandle dataMap, RecompuiResource button) {
    u32 out = (uintptr_t)NULL;
    recomputil_u32_value_hashmap_get(dataMap, button, &out);
    return (void *)out;
}

static void setListButtonData(U32ValueHashmapHandle dataMap, RecompuiResource button, const void *data) {
    recomputil_u32_value_hashmap_insert(dataMap, button, (uintptr_t)data);
}

static bool getListButtonValue(U32ValueHashmapHandle dataMap, RecompuiResource button, u32 *out) {
    return recomputil_u32_value_hashmap_get(dataMap, button, out);
}

static void setListButtonValue(U32ValueHashmapHandle dataMap, RecompuiResource button, u32 val) {
    recomputil_u32_value_hashmap_insert(dataMap, button, val);
}

static void setButtonColor(RecompuiResource button, const ButtonColor *color) {
    recompui_set_background_color(button, &color->bgColor);
    recompui_set_border_color(button, &color->borderColor);
}

static void refreshModelButtonEntryColors(void) {
    CategoryInfo *catInf = getCurrentCategoryInfo();

    if (catInf) {
        void *entry = (void *)ModelEntryManager_getCurrentEntry(catInf->category);
        size_t count = getCurrentModelButtonArraySize();
        const RecompuiResource *buttons = getCurrentModelButtonArray();

        for (size_t i = 0; i < count; ++i) {
            RecompuiResource button = buttons[i];
            const void *buttonData = getListButtonData(sModelButtonsToData, button);

            if (entry && entry == buttonData) {
                setButtonColor(button, &sModelSelectedButtonColor);
            } else if (buttonData) {
                setButtonColor(button, &sPrimaryButtonColor);
            }
        }
    }
}

static void refreshCategoryButtonEntryColors(void) {
    CategoryInfo *catInf = getCurrentCategoryInfo();

    if (catInf) {
        size_t count = getCategoryButtonArraySize();
        const RecompuiResource *buttons = getCategoryButtonArray();

        for (size_t i = 0; i < count; ++i) {
            RecompuiResource button = buttons[i];
            const void *buttonData = getListButtonData(sCategoryButtonsToData, button);

            if (buttonData && buttonData == catInf) {
                setButtonColor(button, &sModelSelectedButtonColor);
            } else if (buttonData) {
                setButtonColor(button, &sPrimaryButtonColor);
            }
        }
    }
}

static RecompuiContext sUIContext;
static RecompuiResource sUIRoot;
static RecompuiResource sContainerMain;
static RecompuiResource sRowTop;
static RecompuiResource sRowAuthor;
static RecompuiResource sRowCategory;
static RecompuiResource sButtonCategoryNext;
static RecompuiResource sButtonCategoryPrev;
static RecompuiResource sLabelCategory;
static RecompuiResource sButtonClose;
static RecompuiResource sButtonCategoriesShow;

static RecompuiResource sContainerCategories;
static bool sIsContainerCategoriesVisible;
static RecompuiResource sCategoriesListElement;
static RecompuiResource sCategoriesTopRow;
static RecompuiResource sButtonCategoriesHide;
static RecompuiResource sButtonRemoveAllModels;

static bool sIsUIContextShown = false;

RecompuiResource sModelListElement;
RecompuiResource sLabelAuthorPrefix;
RecompuiResource sLabelAuthor;

static bool sIsLivePreviewEnabled = false;

static bool shouldLivePreview(void) {
    return sIsLivePreviewEnabled;
}

static void destroyAuthor(void) {
    if (sLabelAuthor) {
        recompui_destroy_element(sRowAuthor, sLabelAuthor);
        recompui_destroy_element(sRowAuthor, sLabelAuthorPrefix);
    }
    sLabelAuthor = 0;
    sLabelAuthorPrefix = 0;
}

static void setAuthor(const char *author) {
    // TODO: Restore author preview
    return;

    if (!author) {
        author = "N/A";
    }

    destroyAuthor();
    sLabelAuthorPrefix = recompui_create_label(sUIContext, sRowAuthor, "Author(s): ", LABELSTYLE_NORMAL);
    sLabelAuthor = recompui_create_label(sUIContext, sRowAuthor, author, LABELSTYLE_NORMAL);
}

static void applyRealEntry(int entryIndex) {
    if (entryIndex >= 0 && entryIndex < ARRAY_COUNT(sCategoryInfos)) {
        ModelEntryManager_tryApplyEntry(sCategoryInfos[entryIndex].category, sCategoryInfos[entryIndex].realEntry);
    } else {
        Logger_printWarning("applyRealEntry received invalid entryIndex %d\n", entryIndex);
    }
}

static void applyRealEntries(void) {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        applyRealEntry(i);
    }
}

static void clearRealEntries(void) {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        sCategoryInfos[i].realEntry = NULL;
    }
}

static void fillRealEntries(void) {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        sCategoryInfos[i].realEntry = ModelEntryManager_getCurrentEntry(sCategoryInfos[i].category);
    }
}

static void removeAllModels(void) {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        CategoryInfo *catInf = &sCategoryInfos[i];
        ModelEntryManager_tryApplyEntry(catInf->category, NULL);
    }
}

static void removeEquipmentModels(void) {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        CategoryInfo *catInf = &sCategoryInfos[i];
        if (isEquipmentModelType(catInf->category)) {
            ModelEntryManager_tryApplyEntry(catInf->category, NULL);
        }
    }
}

static void saveAllModels(void) {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        CategoryInfo *catInf = &sCategoryInfos[i];
        catInf->isNeedsDiskSave = true;
        catInf->realEntry = ModelEntryManager_getCurrentEntry(catInf->category);
    }
}

static void removeAllModelsButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        if (data->type == UI_EVENT_CLICK) {
            Audio_PlaySfx(NA_SE_SY_DECIDE);
            removeAllModels();
            saveAllModels();
        }
    }
}

static void removeEquipmentModelsButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        if (data->type == UI_EVENT_CLICK) {
            if (isSelectingModel()) {
                CategoryInfo *catInf = getCurrentCategoryInfo();
                if (catInf) {
                    Audio_PlaySfx(NA_SE_SY_DECIDE);
                    applyRealEntries();
                    removeEquipmentModels();
                    saveAllModels();
                } else {
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                }
            }
        } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
            destroyAuthor();

            if (shouldLivePreview()) {
                applyRealEntries();
                removeEquipmentModels();
            }
        }
    }
}

static void removePackButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        if (data->type == UI_EVENT_CLICK) {
            if (isSelectingModel()) {
                CategoryInfo *catInf = getCurrentCategoryInfo();
                if (catInf) {
                    Audio_PlaySfx(NA_SE_SY_DECIDE);
                    removeAllModels();
                    saveAllModels();
                } else {
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                }
            }
        } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
            destroyAuthor();

            if (shouldLivePreview()) {
                removeAllModels();
            }
        }
    }
}

static bool sShouldClearAllButtonsNextFrame;

static void closeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        if (data->type == UI_EVENT_CLICK) {
            applyRealEntries();
            recompui_hide_context(sUIContext);
            sIsUIContextShown = false;
            bool wasModelChanged = false;

            for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
                CategoryInfo *catInf = &sCategoryInfos[i];

                if (catInf->isNeedsDiskSave) {
                    catInf->isNeedsDiskSave = false;
                    wasModelChanged = true;
                    ModelEntryManager_saveCurrentEntry(catInf->category);
                }
            }

            if (wasModelChanged) {
                Audio_PlaySfx(NA_SE_SY_PIECE_OF_HEART);
            } else {
                Audio_PlaySfx(NA_SE_SY_DECIDE);
            }

            clearRealEntries();

            sShouldClearAllButtonsNextFrame = true;
        } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
            destroyAuthor();

            if (shouldLivePreview()) {
                applyRealEntries();
            }
        }
    }
}

static void destroyModelButtons();

void refreshCategoryName(void) {
    if (sLabelCategory) {
        recompui_destroy_element(sRowCategory, sLabelCategory);
        sLabelCategory = 0;
    }

    if (isSelectingCategory()) {
        sLabelCategory = recompui_create_label(sUIContext, sRowCategory, "Categories", LABELSTYLE_LARGE);

    } else if (isSelectingModel()) {
        CategoryInfo *catInf = getCurrentCategoryInfo();

        if (catInf) {
            sLabelCategory = recompui_create_label(sUIContext, sRowCategory, catInf->displayName, LABELSTYLE_LARGE);
        }
    }
}

static void incrementCurrentCategory(void) {
    CategoryInfo *currInf;

    do {
        sCurrentCategoryInfo = (sCurrentCategoryInfo + 1) % ARRAY_COUNT(sCategoryInfos);

        currInf = &sCategoryInfos[sCurrentCategoryInfo];
    } while (!(currInf->isVisible || sIsForceAllCategoriesVisible));
}

static void decrementCurrentCategory(void) {
    CategoryInfo *currInf;

    do {
        sCurrentCategoryInfo = (sCurrentCategoryInfo - 1 + ARRAY_COUNT(sCategoryInfos)) % ARRAY_COUNT(sCategoryInfos);

        currInf = &sCategoryInfos[sCurrentCategoryInfo];
    } while (!(currInf->isVisible || sIsForceAllCategoriesVisible));
}

static void changeCategoryButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        if (data->type == UI_EVENT_CLICK) {
            Audio_PlaySfx(NA_SE_SY_DECIDE);

            applyRealEntries();

            bool isNextButton = !!userdata;

            if (isNextButton) {
                incrementCurrentCategory();
            } else {
                decrementCurrentCategory();
            }

            requestRefreshFileList();
        } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
            destroyAuthor();

            if (shouldLivePreview()) {
                applyRealEntries();
            }
        }
    }
}

static void createNextPrevCategoryButtons(void) {
    sButtonCategoryPrev = recompui_create_button(sUIContext, sRowCategory, "◀", BUTTONSTYLE_SECONDARY);
    sButtonCategoryNext = recompui_create_button(sUIContext, sRowCategory, "▶", BUTTONSTYLE_SECONDARY);

    recompui_register_callback(sButtonCategoryPrev, changeCategoryButtonPressed, NULL);
    recompui_register_callback(sButtonCategoryNext, changeCategoryButtonPressed, (void *)1);
}

static void setupContainer(RecompuiResource container, float borderWidth, float borderRadius, RecompuiColor *borderColor, RecompuiColor *modalColor) {
    recompui_set_height(container, 100.f, UNIT_PERCENT);
    recompui_set_flex_grow(container, 1.0f);
    recompui_set_max_width(container, 33.f, UNIT_PERCENT);

    recompui_set_display(container, DISPLAY_FLEX);
    recompui_set_justify_content(container, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_flex_direction(container, FLEX_DIRECTION_COLUMN);
    recompui_set_padding(container, 16.0f, UNIT_DP);
    recompui_set_gap(container, 16.0f, UNIT_DP);
    recompui_set_align_items(container, ALIGN_ITEMS_FLEX_START);

    recompui_set_border_width(container, borderWidth, UNIT_DP);
    recompui_set_border_radius(container, borderRadius, UNIT_DP);
    recompui_set_border_color(container, borderColor);
    recompui_set_background_color(container, modalColor);
}

static void setupScrollingList(RecompuiResource list) {
    recompui_set_flex_basis(list, 100.0f, UNIT_DP);
    recompui_set_flex_grow(list, 1.0f);
    recompui_set_flex_shrink(list, 0.0f);
    recompui_set_display(list, DISPLAY_FLEX);
    recompui_set_flex_direction(list, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(list, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(list, ALIGN_ITEMS_FLEX_START);
    recompui_set_gap(list, 16.0f, UNIT_DP);
    recompui_set_overflow_y(list, OVERFLOW_SCROLL);
    recompui_set_overflow_x(list, OVERFLOW_HIDDEN);
}

static void setupRow(RecompuiResource row) {
    recompui_set_flex_basis(row, 100.0f, UNIT_DP);
    recompui_set_flex_grow(row, 0);
    recompui_set_flex_shrink(row, 0);
    recompui_set_display(row, DISPLAY_FLEX);
    recompui_set_flex_direction(row, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(row, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(row, ALIGN_ITEMS_FLEX_END);
    recompui_set_gap(row, 16.0f, UNIT_DP);
}

static void setupAuthorRow(void) {
    sRowAuthor = recompui_create_element(sUIContext, sContainerMain);
    recompui_set_flex_basis(sRowAuthor, 100.0f, UNIT_DP);
    recompui_set_flex_grow(sRowAuthor, 0.0f);
    recompui_set_flex_shrink(sRowAuthor, 0.0f);
    recompui_set_display(sRowAuthor, DISPLAY_FLEX);
    recompui_set_flex_direction(sRowAuthor, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(sRowAuthor, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(sRowAuthor, ALIGN_ITEMS_FLEX_END);
    recompui_set_gap(sRowAuthor, 0.0f, UNIT_DP);
}

RECOMP_CALLBACK(".", _internal_preInitHashObjects) void initUIOnRecompInit(void) {
    RecompuiColor backgroundColor;
    backgroundColor.r = 255;
    backgroundColor.g = 255;
    backgroundColor.b = 255;
    backgroundColor.a = 0.1f * 255;

    RecompuiColor borderColor;
    borderColor.r = 255;
    borderColor.g = 255;
    borderColor.b = 255;
    borderColor.a = 0.2f * 255;

    RecompuiColor modalColor;
    modalColor.r = 8;
    modalColor.g = 7;
    modalColor.b = 13;
    modalColor.a = 0.9f * 255;

    const float bodyPadding = 64.0f;
    const float modalHeight = RECOMPUI_TOTAL_HEIGHT - (2 * bodyPadding);
    const float modalMaxWidth = modalHeight * (16.0f / 9.0f);
    const float modalBorderWidth = 1.1f;
    const float modalBorderRadius = 16.0f;

    sUIContext = recompui_create_context();
    recompui_open_context(sUIContext);

    sUIRoot = recompui_context_root(sUIContext);
    // Set up the sUIRoot element so it takes up the full screen.
    recompui_set_position(sUIRoot, POSITION_ABSOLUTE);
    recompui_set_top(sUIRoot, 0, UNIT_DP);
    recompui_set_right(sUIRoot, 0, UNIT_DP);
    recompui_set_bottom(sUIRoot, 0, UNIT_DP);
    recompui_set_left(sUIRoot, 0, UNIT_DP);
    recompui_set_width_auto(sUIRoot);
    recompui_set_height_auto(sUIRoot);

    // Set up the sUIRoot element's padding so the modal contents don't touch the screen edges.
    recompui_set_padding(sUIRoot, bodyPadding, UNIT_DP);

    // Set up the flexbox properties of the sUIRoot element.
    recompui_set_flex_direction(sUIRoot, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(sUIRoot, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(sUIRoot, ALIGN_ITEMS_FLEX_START);

    sContainerMain = recompui_create_element(sUIContext, sUIRoot);

    setupContainer(sContainerMain, modalBorderWidth, modalBorderRadius, &borderColor, &modalColor);

    sRowTop = recompui_create_element(sUIContext, sContainerMain);
    setupRow(sRowTop);

    sRowCategory = recompui_create_element(sUIContext, sContainerMain);
    setupRow(sRowCategory);
    createNextPrevCategoryButtons();

    sButtonClose = recompui_create_button(sUIContext, sRowTop, "Apply", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(sButtonClose, TEXT_ALIGN_CENTER);

    sButtonCategoriesShow = recompui_create_button(sUIContext, sRowTop, "Show Categories", BUTTONSTYLE_SECONDARY);
    recompui_register_callback(sButtonCategoriesShow, onUpOneLevelButtonPressed, NULL);

    recompui_register_callback(sButtonClose, closeButtonPressed, NULL);

    sModelListElement = recompui_create_element(sUIContext, sContainerMain);
    setupScrollingList(sModelListElement);

    // TODO: Restore author preview after UI crashes are mitigated in Recomp
    //setupAuthorRow();

    // Setup category container
    sContainerCategories = recompui_create_element(sUIContext, sUIRoot);

    setupContainer(sContainerCategories, modalBorderWidth, modalBorderRadius, &borderColor, &modalColor);

    sCategoriesTopRow = recompui_create_element(sUIContext, sContainerCategories);
    setupRow(sCategoriesTopRow);

    sButtonCategoriesHide = recompui_create_button(sUIContext, sCategoriesTopRow, "Hide Cat.", BUTTONSTYLE_SECONDARY);
    recompui_register_callback(sButtonCategoriesHide, onUpOneLevelButtonPressed, NULL);

    sButtonRemoveAllModels = recompui_create_button(sUIContext, sCategoriesTopRow, "[Remove All]", BUTTONSTYLE_SECONDARY);
    recompui_register_callback(sButtonRemoveAllModels, removeAllModelsButtonPressed, NULL);
    setButtonColor(sButtonRemoveAllModels, &sModelRemovedButtonColor);

    sCategoriesListElement = recompui_create_element(sUIContext, sContainerCategories);
    setupScrollingList(sCategoriesListElement);

    recompui_set_visibility(sContainerCategories, VISIBILITY_HIDDEN);
    sIsContainerCategoriesVisible = false;

    recompui_set_nav(sButtonCategoriesShow, NAVDIRECTION_RIGHT, sButtonCategoryPrev);
    recompui_set_nav(sButtonCategoriesShow, NAVDIRECTION_DOWN, sButtonCategoryNext);

    recompui_set_nav(sButtonCategoryNext, NAVDIRECTION_UP, sButtonCategoriesShow);

    recompui_set_nav(sButtonCategoryPrev, NAVDIRECTION_LEFT, sButtonCategoriesShow);
    recompui_set_nav(sButtonCategoryPrev, NAVDIRECTION_UP, sButtonClose);

    recompui_set_nav(sButtonClose, NAVDIRECTION_DOWN, sButtonCategoryPrev);
    recompui_set_nav(sButtonClose, NAVDIRECTION_RIGHT, sButtonCategoriesShow);
    recompui_set_nav(sButtonClose, NAVDIRECTION_DOWN, sButtonCategoryPrev);

    recompui_close_context(sUIContext);

    sIsUIContextShown = false;
}

static void onModelButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        ModelEntry *entryOrNull = userdata;

        CategoryInfo *catInf = getCurrentCategoryInfo();

        if (catInf) {
            PlayerModelManagerModelType modelType = catInf->category;

            if (data->type == UI_EVENT_CLICK) {
                Audio_PlaySfx(NA_SE_SY_DECIDE);

                ModelEntryManager_tryApplyEntry(modelType, entryOrNull);

                catInf->realEntry = entryOrNull;

                refreshModelButtonEntryColors();

                catInf->isNeedsDiskSave = true;
            } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
                if (entryOrNull) {
                    setAuthor(ModelEntry_getAuthorName(entryOrNull));
                } else {
                    destroyAuthor();
                }

                if (shouldLivePreview()) {
                    applyRealEntries();
                    ModelEntryManager_tryApplyEntry(modelType, entryOrNull);
                }
            }
        }
    }
}

static void onPackButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        ModelEntry *entryOrNull = userdata;

        CategoryInfo *catInf = getCurrentCategoryInfo();

        if (catInf) {
            PlayerModelManagerModelType modelType = catInf->category;

            if (data->type == UI_EVENT_CLICK) {
                Audio_PlaySfx(NA_SE_SY_DECIDE);
                ModelEntryManager_tryApplyEntry(modelType, entryOrNull);
                saveAllModels();
            } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
                if (entryOrNull) {
                    setAuthor(ModelEntry_getAuthorName(entryOrNull));
                } else {
                    destroyAuthor();
                }

                if (shouldLivePreview()) {
                    applyRealEntries();
                    ModelEntryManager_tryApplyEntry(modelType, entryOrNull);
                }
            }
        }
    }
}

static void onCategoryButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        if (data->type == UI_EVENT_CLICK) {
            CategoryInfo *catInf = userdata;

            if (catInf && isValidCategoryInfoIndex(catInf->index)) {
                Audio_PlaySfx(NA_SE_SY_DECIDE);
                sCurrentCategoryInfo = catInf->index;
                requestRefreshFileList();
            } else {
                Audio_PlaySfx(NA_SE_SY_ERROR);
            }
        }
    }
}

static void onUpOneLevelButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (sIsUIContextShown) {
        if (data->type == UI_EVENT_CLICK) {
            Audio_PlaySfx(NA_SE_SY_DECIDE);

            sIsContainerCategoriesVisible = !sIsContainerCategoriesVisible;

            if (sIsContainerCategoriesVisible) {
                recompui_set_visibility(sContainerCategories, VISIBILITY_VISIBLE);
                recompui_set_visibility(sButtonCategoriesShow, VISIBILITY_HIDDEN);
                recompui_set_nav(sButtonClose, NAVDIRECTION_RIGHT, sButtonCategoriesHide);
                recompui_set_nav(sButtonCategoryNext, NAVDIRECTION_UP, sButtonClose);
            } else {
                recompui_set_visibility(sContainerCategories, VISIBILITY_HIDDEN);
                recompui_set_visibility(sButtonCategoriesShow, VISIBILITY_VISIBLE);
                recompui_set_nav(sButtonClose, NAVDIRECTION_RIGHT, sButtonCategoriesShow);
                recompui_set_nav(sButtonCategoryNext, NAVDIRECTION_UP, sButtonCategoriesShow);
            }
        } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
            destroyAuthor();

            if (shouldLivePreview()) {
                applyRealEntries();
            }
        }
    }
}

static void destroyModelButtons(void) {
    size_t count = getCurrentModelButtonArraySize();
    const RecompuiResource *buttons = getCurrentModelButtonArray();

    for (size_t i = 0; i < count; ++i) {
        recomputil_u32_value_hashmap_erase(sModelButtonsToData, buttons[i]);
        recompui_destroy_element(sModelListElement, buttons[i]);
    }

    YAZMTCore_IterableU32Set_clear(sModelListButtons);
}

static void createModelListButtons(void) {
    CategoryInfo *catInf = getCurrentCategoryInfo();
    RecompuiEventHandler *pressedCallback = onModelButtonPressed;
    RecompuiEventHandler *removedCallback = onModelButtonPressed;
    const char *removeText = "[None]";

    bool isPack = isPackModelType(catInf->category);
    if (isPack) {
        pressedCallback = onPackButtonPressed;
        removedCallback = removeEquipmentModelsButtonPressed;
        removeText = "[Remove Equipment Models]";
    }

    RecompuiResource removeModelButton = createAndPushButtonToList(sUIContext, sModelListElement, removeText, BUTTONSTYLE_PRIMARY, sModelListButtons);
    setButtonColor(removeModelButton, &sModelRemovedButtonColor);
    recompui_register_callback(removeModelButton, removedCallback, NULL);

    size_t count = 0;
    const ModelEntry **modelEntries = ModelEntryManager_getCategoryEntryData(catInf->category, &count);

    for (size_t i = 0; i < count; ++i) {
        if (!ModelEntryManager_isEntryHidden(modelEntries[i])) {
            const char *name = NULL;

            name = ModelEntry_getDisplayName(modelEntries[i]);
            if (!name) {
                name = ModelEntry_getInternalName(modelEntries[i]);

                if (!name) {
                    name = "ERROR READING MODEL ENTRY NAME";
                }
            }

            RecompuiResource modelButton = createAndPushButtonToList(sUIContext, sModelListElement, name, BUTTONSTYLE_PRIMARY, sModelListButtons);

            // Need to cast away constness due to API signature
            // function in pressedCallback should not modify the ModelEntry, though
            recompui_register_callback(modelButton, pressedCallback, (ModelEntry *)modelEntries[i]);

            setListButtonData(sModelButtonsToData, modelButton, modelEntries[i]);
        }
    }

    connectListBoxButtons(getCurrentModelButtonArray(), getCurrentModelButtonArraySize());
}

static void requestRefreshFileList(void) {
    sIsFileListRefreshRequested = true;
}

static void refreshFileList(void) {
    // MUST CALL INSIDE UI CONTEXT

    destroyModelButtons();
    
    if (isSelectingModel()) {
        createModelListButtons();

        refreshModelButtonEntryColors();
        refreshCategoryButtonEntryColors();

        size_t listCount = getCurrentModelButtonArraySize();

        if (listCount > 0) {
            const RecompuiResource *buttons = getCurrentModelButtonArray();
            RecompuiResource first = buttons[0];
            RecompuiResource last = buttons[listCount - 1];

            recompui_set_nav(first, NAVDIRECTION_UP, sButtonCategoryPrev);
            recompui_set_nav(first, NAVDIRECTION_LEFT, sButtonCategoryNext);

            recompui_set_nav(sButtonCategoryNext, NAVDIRECTION_DOWN, first);
            recompui_set_nav(sButtonCategoryNext, NAVDIRECTION_RIGHT, first);

            recompui_set_nav(sButtonCategoryPrev, NAVDIRECTION_DOWN, first);

            recompui_set_nav(sButtonClose, NAVDIRECTION_UP, last);

            recompui_set_nav(last, NAVDIRECTION_DOWN, sButtonClose);

            recompui_set_nav(sButtonCategoriesShow, NAVDIRECTION_UP, last);
        } else {
            recompui_set_nav(sButtonCategoryNext, NAVDIRECTION_DOWN, sButtonCategoriesShow);

            recompui_set_nav(sButtonCategoryPrev, NAVDIRECTION_DOWN, sButtonClose);

            recompui_set_nav(sButtonClose, NAVDIRECTION_UP, sButtonCategoryPrev);

            recompui_set_nav(sButtonCategoriesShow, NAVDIRECTION_UP, sButtonCategoryNext);
        }
    }
    refreshCategoryName();
    applyRealEntries();
}

typedef enum {
    MODCFG_BUTTON_COMBO_NONE,
    MODCFG_BUTTON_COMBO_LR,
    MODCFG_BUTTON_COMBO_LA,
} ModConfig_ButtonCombo;

static void clearPressedInputButtons(Input *input, u16 buttons) {
    input->press.button &= (~buttons);
}

static bool isOpenMenuComboPressed(PlayState *play) {
    Input *input = CONTROLLER1(&play->state);

    switch (recomp_get_config_u32("open_menu_buttons")) {
        case MODCFG_BUTTON_COMBO_LR:
            if ((CHECK_BTN_ALL(input->press.button, BTN_L) && CHECK_BTN_ALL(input->cur.button, BTN_R)) ||
                (CHECK_BTN_ALL(input->cur.button, BTN_L) && CHECK_BTN_ALL(input->press.button, BTN_R))) {
                clearPressedInputButtons(input, BTN_L | BTN_R);
                return true;
            }
            break;

        case MODCFG_BUTTON_COMBO_LA:

            if ((CHECK_BTN_ALL(input->press.button, BTN_L) && CHECK_BTN_ALL(input->cur.button, BTN_A)) ||
                (CHECK_BTN_ALL(input->cur.button, BTN_L) && CHECK_BTN_ALL(input->press.button, BTN_A))) {
                    clearPressedInputButtons(input, BTN_L | BTN_A);
                    return true;
            }
            break;

        default:
            break;
    }

    return false;
}

static void openModelMenu(void) {
    if (!sIsUIContextShown) {
        sIsLivePreviewEnabled = recomp_get_config_u32("is_live_preview_enabled");
        fillRealEntries();
        recompui_show_context(sUIContext);
        sIsUIContextShown = true;
    }
}

static bool sIsModelManagerReady;

RECOMP_CALLBACK(".", onReady) void allowUIOnReady(void) {
    sIsModelManagerReady = true;
}

// Hook Play_UpdateMain to check if the L button is pressed and show this mod's UI if so.
void checkToOpenModelMenu_on_Play_UpdateMain(PlayState *play) {
    if (isOpenMenuComboPressed(play) && sIsModelManagerReady) {
        openModelMenu();
    }

    if (sIsFileListRefreshRequested && sIsUIContextShown) {
        sIsFileListRefreshRequested = false;
        recompui_open_context(sUIContext);
        refreshFileList();
        recompui_close_context(sUIContext);
    }

    if (sShouldClearAllButtonsNextFrame) {
        clearPressedInputButtons(CONTROLLER1(&play->state), 0xFFFFU);
    }

    sShouldClearAllButtonsNextFrame = false;
}

void initUIFileList(void) {
    recompui_open_context(sUIContext);

    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        CategoryInfo *catInf = &sCategoryInfos[i];
        size_t count = 0;
        ModelEntryManager_getCategoryEntryData(catInf->category, &count);
        catInf->isVisible = catInf->isVisible || (catInf->isUsedByCurrentGame && count > 0);

        if (catInf->isVisible) {
            RecompuiResource button = createAndPushButtonToList(sUIContext, sCategoriesListElement, catInf->displayName, BUTTONSTYLE_PRIMARY, sCategoryListButtons);
            recompui_register_callback(button, onCategoryButtonPressed, catInf);
            setListButtonData(sCategoryButtonsToData, button, catInf);
        }
    }

    const RecompuiResource *catButtons = getCategoryButtonArray();
    size_t catButtonsNum = getCategoryButtonArraySize();
    connectListBoxButtons(catButtons, catButtonsNum);
    recompui_set_nav(sButtonCategoriesHide, NAVDIRECTION_DOWN, catButtons[0]);
    recompui_set_nav(sButtonCategoriesHide, NAVDIRECTION_RIGHT, sButtonRemoveAllModels);
    recompui_set_nav(sButtonCategoriesHide, NAVDIRECTION_UP, catButtons[catButtonsNum - 1]);
    recompui_set_nav(sButtonCategoriesHide, NAVDIRECTION_LEFT, sButtonClose);

    recompui_set_nav(sButtonRemoveAllModels, NAVDIRECTION_DOWN, catButtons[0]);
    recompui_set_nav(sButtonRemoveAllModels, NAVDIRECTION_RIGHT, catButtons[0]);
    recompui_set_nav(sButtonRemoveAllModels, NAVDIRECTION_UP, catButtons[catButtonsNum - 1]);
    recompui_set_nav(sButtonRemoveAllModels, NAVDIRECTION_LEFT, sButtonCategoriesHide);

    recompui_set_nav(catButtons[0], NAVDIRECTION_LEFT, sButtonRemoveAllModels);
    recompui_set_nav(catButtons[0], NAVDIRECTION_UP, sButtonCategoriesHide);

    recompui_set_nav(catButtons[catButtonsNum - 1], NAVDIRECTION_DOWN, sButtonCategoriesHide);

    requestRefreshFileList();
    recompui_close_context(sUIContext);
}

RECOMP_CALLBACK(".", _internal_initHashObjects) void initUiObjects(void) {
    sModelButtonsToData = recomputil_create_u32_value_hashmap();
    sCategoryButtonsToData = recomputil_create_u32_value_hashmap();
    sModelListButtons = YAZMTCore_IterableU32Set_new();
    sCategoryListButtons = YAZMTCore_IterableU32Set_new();
}

RECOMP_CALLBACK(".", _internal_preInitHashObjects) void preInitUiObjects(void) {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        sCategoryInfos[i].index = i;
    }
}
