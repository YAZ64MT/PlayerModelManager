#include "modding.h"
#include "sys_cmpdma.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "recompui.h"
#include "custommodelentrymanager.h"
#include "model_common.h"
#include "yazmtcorelib_api.h"

static void refreshFileList();
static void refreshButtonEntryColors();

typedef struct {
    ModelEntry *modelEntry;
    RecompuiResource button;
} ListEntry;

RecompuiContext context;
RecompuiResource root;
RecompuiResource container;
RecompuiResource row1;
RecompuiResource row2;
RecompuiResource rowCategory;
RecompuiResource buttonCategoryNext;
RecompuiResource buttonCategoryPrev;
RecompuiResource labelCategory;
RecompuiResource buttonClose;
RecompuiResource buttonRemoveModel;
RecompuiResource buttonUpOneMenuLevel;

bool context_shown = false;

RecompuiResource containerModelList;
RecompuiResource labelUiTitle;
RecompuiResource labellabelModelAuthorPrefix;
RecompuiResource labelModelAuthor;

static bool sIsLivePreviewEnabled = false;

typedef struct {
    const char *displayName;
    bool isVisible;
    bool isUsedByCurrentGame;
    ModelEntry *realEntry;
    Link_CustomModelCategory category;
    bool isNeedsDiskSave;
} CategoryInfo;

static bool sIsForceAllCategoriesVisible = false;

#define DECLARE_DEFAULT_CAT_INFO(name, isUsed, cat, isVisibleByDefault) {.displayName = name, .isVisible = isVisibleByDefault, .isUsedByCurrentGame = isUsed, .realEntry = NULL, .category = cat, .isNeedsDiskSave = false}
#define DECLARE_CAT_INFO(name, isUsed, cat) DECLARE_DEFAULT_CAT_INFO(name, isUsed, cat, false)

#define CAT_USED_MM true

// Always opposite of MM
#define CAT_USED_OOT (!CAT_USED_MM)

// used by both games
#define CAT_USED_Z64 true

static CategoryInfo sCategoryInfos[] = {
    // At least one category must be visible or the category selector goes into an infinite loop
    DECLARE_DEFAULT_CAT_INFO("Young Link", CAT_USED_OOT, LINK_CMC_CHILD, CAT_USED_OOT),
    DECLARE_DEFAULT_CAT_INFO("Adult Link", CAT_USED_OOT, LINK_CMC_ADULT, CAT_USED_OOT),
    DECLARE_DEFAULT_CAT_INFO("Human", CAT_USED_MM, LINK_CMC_HUMAN, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Deku", CAT_USED_MM, LINK_CMC_DEKU, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Goron", CAT_USED_MM, LINK_CMC_GORON, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Zora", CAT_USED_MM, LINK_CMC_ZORA, CAT_USED_MM),
    DECLARE_DEFAULT_CAT_INFO("Fierce Deity", CAT_USED_MM, LINK_CMC_FIERCE_DEITY, CAT_USED_MM),
    DECLARE_CAT_INFO("Kokiri Sword", CAT_USED_Z64, LINK_CMC_SWORD1),
    DECLARE_CAT_INFO("Razor Sword", CAT_USED_MM, LINK_CMC_SWORD2),
    DECLARE_CAT_INFO("Gilded Sword", CAT_USED_Z64, LINK_CMC_SWORD3),
    DECLARE_CAT_INFO("Fierce Deity's Sword", CAT_USED_Z64, LINK_CMC_SWORD4),
    DECLARE_CAT_INFO("Great Fairy Sword", CAT_USED_MM, LINK_CMC_SWORD5),
    DECLARE_CAT_INFO("Deku Shield", CAT_USED_OOT, LINK_CMC_SHIELD1),
    DECLARE_CAT_INFO("Hero's Shield", CAT_USED_Z64, LINK_CMC_SHIELD2),
    DECLARE_CAT_INFO("Mirror Shield", CAT_USED_Z64, LINK_CMC_SHIELD3),
    DECLARE_CAT_INFO("Hookshot", CAT_USED_Z64, LINK_CMC_HOOKSHOT),
    DECLARE_CAT_INFO("Bow", CAT_USED_Z64, LINK_CMC_BOW),
    DECLARE_CAT_INFO("Slingshot", CAT_USED_OOT, LINK_CMC_SLINGSHOT),
    DECLARE_CAT_INFO("Bottle", CAT_USED_Z64, LINK_CMC_BOTTLE),
    DECLARE_CAT_INFO("Fairy Ocarina", CAT_USED_OOT, LINK_CMC_OCARINA_FAIRY),
    DECLARE_CAT_INFO("Ocarina of Time", CAT_USED_Z64, LINK_CMC_OCARINA_TIME),
    DECLARE_CAT_INFO("Boomerang", CAT_USED_OOT, LINK_CMC_BOOMERANG),
    DECLARE_CAT_INFO("Hammer", CAT_USED_OOT, LINK_CMC_HAMMER),
    DECLARE_CAT_INFO("Deku Stick", CAT_USED_Z64, LINK_CMC_DEKU_STICK),
    DECLARE_CAT_INFO("Deku Pipes", CAT_USED_MM, LINK_CMC_PIPES),
    DECLARE_CAT_INFO("Goron Drums", CAT_USED_MM, LINK_CMC_DRUMS),
    DECLARE_CAT_INFO("Zora Guitar", CAT_USED_MM, LINK_CMC_GUITAR),
    DECLARE_CAT_INFO("Skull Mask", CAT_USED_OOT, LINK_CMC_MASK_SKULL),
    DECLARE_CAT_INFO("Spooky Mask", CAT_USED_OOT, LINK_CMC_MASK_SPOOKY),
    DECLARE_CAT_INFO("Gerudo Mask", CAT_USED_OOT, LINK_CMC_MASK_GERUDO),
    DECLARE_CAT_INFO("Mask of Truth", CAT_USED_Z64, LINK_CMC_MASK_TRUTH),
    DECLARE_CAT_INFO("Kafei's Mask", CAT_USED_MM, LINK_CMC_MASK_KAFEIS_MASK),
    DECLARE_CAT_INFO("All Night Mask", CAT_USED_MM, LINK_CMC_MASK_ALL_NIGHT),
    DECLARE_CAT_INFO("Bunny Hood", CAT_USED_Z64, LINK_CMC_MASK_BUNNY),
    DECLARE_CAT_INFO("Keaton Mask", CAT_USED_Z64, LINK_CMC_MASK_KEATON),
    DECLARE_CAT_INFO("Garo Mask", CAT_USED_MM, LINK_CMC_MASK_GARO),
    DECLARE_CAT_INFO("Romani Mask", CAT_USED_MM, LINK_CMC_MASK_ROMANI),
    DECLARE_CAT_INFO("Circus Leader's Mask", CAT_USED_MM, LINK_CMC_MASK_CIRCUS_LEADER),
    DECLARE_CAT_INFO("Couple's Mask", CAT_USED_MM, LINK_CMC_MASK_COUPLE),
    DECLARE_CAT_INFO("Postman's Hat", CAT_USED_MM, LINK_CMC_MASK_POSTMAN),
    DECLARE_CAT_INFO("Great Fairy Mask", CAT_USED_MM, LINK_CMC_MASK_GREAT_FAIRY),
    DECLARE_CAT_INFO("Gibdo Mask", CAT_USED_MM, LINK_CMC_MASK_GIBDO),
    DECLARE_CAT_INFO("Don Gero's Mask", CAT_USED_MM, LINK_CMC_MASK_DON_GERO),
    DECLARE_CAT_INFO("Kamaro Mask", CAT_USED_MM, LINK_CMC_MASK_KAMARO),
    DECLARE_CAT_INFO("Captain's Hat", CAT_USED_MM, LINK_CMC_MASK_CAPTAIN),
    DECLARE_CAT_INFO("Stone Mask", CAT_USED_MM, LINK_CMC_MASK_STONE),
    DECLARE_CAT_INFO("Bremen Mask", CAT_USED_MM, LINK_CMC_MASK_BREMEN),
    DECLARE_CAT_INFO("Blast Mask", CAT_USED_MM, LINK_CMC_MASK_BLAST),
    DECLARE_CAT_INFO("Mask of Scents", CAT_USED_MM, LINK_CMC_MASK_SCENTS),
    DECLARE_CAT_INFO("Giant's Mask", CAT_USED_MM, LINK_CMC_MASK_GIANT),
    DECLARE_CAT_INFO("Deku Mask", CAT_USED_MM, LINK_CMC_MASK_DEKU),
    DECLARE_CAT_INFO("Goron Mask", CAT_USED_Z64, LINK_CMC_MASK_GORON),
    DECLARE_CAT_INFO("Zora Mask", CAT_USED_Z64, LINK_CMC_MASK_ZORA),
    DECLARE_CAT_INFO("Fierce Deity Mask", CAT_USED_MM, LINK_CMC_MASK_FIERCE_DEITY),
};

#define SELECTING_CATEGORY -99

static int sCurrentCategoryInfo = SELECTING_CATEGORY;

static bool isSelectingCategory() {
    return sCurrentCategoryInfo == SELECTING_CATEGORY;
}

static bool isSelectingModel() {
    return sCurrentCategoryInfo >= 0 && sCurrentCategoryInfo < ARRAY_COUNT(sCategoryInfos);
}

static CategoryInfo *getCurrentCategoryInfo() {
    if (isSelectingModel()) {
        return &sCategoryInfos[sCurrentCategoryInfo];
    } else {
        recomp_printf("PlayerModelManager: getCurrentCategoryInfo found invalid sCurrentCategoryInfo value %d\n", sCurrentCategoryInfo);
    }

    return NULL;
}

static bool shouldLivePreview() {
    return sIsLivePreviewEnabled;
}

static void destroyAuthor() {
    if (labelModelAuthor) {
        recompui_destroy_element(row2, labelModelAuthor);
        recompui_destroy_element(row2, labellabelModelAuthorPrefix);
    }
    labelModelAuthor = 0;
    labellabelModelAuthorPrefix = 0;
}

static void setAuthor(const char *author) {
    if (!author) {
        author = "N/A";
    }

    destroyAuthor();
    labellabelModelAuthorPrefix = recompui_create_label(context, row2, "Author(s): ", LABELSTYLE_NORMAL);
    labelModelAuthor = recompui_create_label(context, row2, author, LABELSTYLE_NORMAL);
}

static void applyRealEntry(int entryIndex) {
    if (entryIndex >= 0 && entryIndex < ARRAY_COUNT(sCategoryInfos)) {
        CMEM_tryApplyEntry(sCategoryInfos[entryIndex].category, sCategoryInfos[entryIndex].realEntry);
    } else {
        recomp_printf("PlayerModelManager: applyRealEntry received invalid entryIndex %d\n", entryIndex);
    }
}

static void applyRealEntries() {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        applyRealEntry(i);
    }
}

static void clearRealEntries() {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        sCategoryInfos[i].realEntry = NULL;
    }
}

static void fillRealEntries() {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        sCategoryInfos[i].realEntry = CMEM_getCurrentEntry(sCategoryInfos[i].category);
    }
}

static void removeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (context_shown) {
        if (data->type == UI_EVENT_CLICK) {
            if (isSelectingCategory()) {
                Audio_PlaySfx(NA_SE_SY_DECIDE);
                for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
                    CategoryInfo *catInf = &sCategoryInfos[i];
                    CMEM_tryApplyEntry(catInf->category, NULL);
                    catInf->isNeedsDiskSave = true;
                    catInf->realEntry = NULL;
                }
            } else if (isSelectingModel()) {
                CategoryInfo *catInf = getCurrentCategoryInfo();
                if (catInf) {
                    Audio_PlaySfx(NA_SE_SY_DECIDE);
                    catInf->realEntry = NULL;
                    CMEM_tryApplyEntry(catInf->category, NULL);
                    refreshButtonEntryColors();
                    catInf->isNeedsDiskSave = true;
                } else {
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                }
            }
        } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
            destroyAuthor();

            if (shouldLivePreview() && isSelectingModel()) {
                applyRealEntry(sCurrentCategoryInfo);
            }
        }
    }
}

static void closeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (context_shown) {
        if (data->type == UI_EVENT_CLICK) {
            applyRealEntries();
            recompui_hide_context(context);
            context_shown = false;
            bool wasModelChanged = false;

            for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
                CategoryInfo *catInf = &sCategoryInfos[i];

                if (catInf->isNeedsDiskSave) {
                    catInf->isNeedsDiskSave = false;
                    wasModelChanged = true;
                    CMEM_saveCurrentEntry(catInf->category);
                }
            }

            if (wasModelChanged) {
                Audio_PlaySfx(NA_SE_SY_PIECE_OF_HEART);
            } else {
                Audio_PlaySfx(NA_SE_SY_DECIDE);
            }

            clearRealEntries();
        } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
            destroyAuthor();

            if (shouldLivePreview()) {
                applyRealEntries();
            }
        }
    }
}

static void destroyModelButtons();
static void createModelButtons();
static void refreshButtonEntryColors();

void refreshCategoryName() {
    if (labelCategory) {
        recompui_destroy_element(rowCategory, labelCategory);
        labelCategory = 0;
    }

    if (isSelectingCategory()) {
        labelCategory = recompui_create_label(context, rowCategory, "Categories", LABELSTYLE_LARGE);
        
    }
    else if (isSelectingModel()) {
        CategoryInfo *catInf = getCurrentCategoryInfo();

        if (catInf) {
            labelCategory = recompui_create_label(context, rowCategory, catInf->displayName, LABELSTYLE_LARGE);
        }
    }
}

static void incrementCurrentCategory() {
    CategoryInfo *currInf;

    do {
        sCurrentCategoryInfo = (sCurrentCategoryInfo + 1) % ARRAY_COUNT(sCategoryInfos);

        currInf = &sCategoryInfos[sCurrentCategoryInfo];
    } while (!(currInf->isVisible || sIsForceAllCategoriesVisible));
}

static void decrementCurrentCategory() {
    CategoryInfo *currInf;

    do {
        sCurrentCategoryInfo = (sCurrentCategoryInfo - 1 + ARRAY_COUNT(sCategoryInfos)) % ARRAY_COUNT(sCategoryInfos);

        currInf = &sCategoryInfos[sCurrentCategoryInfo];
    } while (!(currInf->isVisible || sIsForceAllCategoriesVisible));
}

static void changeCategoryButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (context_shown) {
        if (data->type == UI_EVENT_CLICK) {
            Audio_PlaySfx(NA_SE_SY_DECIDE);

            bool isNextButton = !!userdata;

            if (isNextButton) {
                incrementCurrentCategory();
            } else {
                decrementCurrentCategory();
            }

            refreshFileList();
            refreshButtonEntryColors();
            refreshCategoryName();
        }
    }
}

static void destroyNextPrevCategoryButtons() {
    if (buttonCategoryNext) {
        recompui_destroy_element(rowCategory, buttonCategoryNext);
        buttonCategoryNext = 0;
    }

    if (buttonCategoryPrev) {
        recompui_destroy_element(rowCategory, buttonCategoryPrev);
        recompui_set_nav_auto(buttonClose, NAVDIRECTION_RIGHT);
        recompui_set_nav_auto(buttonClose, NAVDIRECTION_DOWN);
        buttonCategoryPrev = 0;
    }
}

static void createNextPrevCategoryButtons() {
    destroyNextPrevCategoryButtons();

    buttonCategoryPrev = recompui_create_button(context, rowCategory, "◀", BUTTONSTYLE_SECONDARY);
    buttonCategoryNext = recompui_create_button(context, rowCategory, "▶", BUTTONSTYLE_SECONDARY);

    recompui_register_callback(buttonCategoryPrev, changeCategoryButtonPressed, NULL);
    recompui_register_callback(buttonCategoryNext, changeCategoryButtonPressed, (void *)1);

    recompui_set_nav(buttonClose, NAVDIRECTION_DOWN, buttonCategoryPrev);

    recompui_set_nav(buttonRemoveModel, NAVDIRECTION_UP, buttonCategoryPrev);
    recompui_set_nav(buttonRemoveModel, NAVDIRECTION_LEFT, buttonCategoryNext);

    recompui_set_nav(buttonCategoryPrev, NAVDIRECTION_LEFT, buttonClose);
    recompui_set_nav(buttonCategoryPrev, NAVDIRECTION_DOWN, buttonRemoveModel);
    recompui_set_nav(buttonCategoryPrev, NAVDIRECTION_UP, buttonClose);

    recompui_set_nav(buttonCategoryNext, NAVDIRECTION_DOWN, buttonRemoveModel);
    recompui_set_nav(buttonCategoryNext, NAVDIRECTION_RIGHT, buttonRemoveModel);
}

typedef struct {
    union {
        int index;
        void *ptr;
    } data;
    RecompuiResource button;
} ButtonEntry;

YAZMTCore_DynamicDataArray *sButtonEntries;

RECOMP_DECLARE_EVENT(_internal_onReadyUI());

RECOMP_CALLBACK("*", recomp_on_init)
void on_init() {
    sButtonEntries = YAZMTCore_DynamicDataArray_new(sizeof(ButtonEntry));

    RecompuiColor bg_color;
    bg_color.r = 255;
    bg_color.g = 255;
    bg_color.b = 255;
    bg_color.a = 0.1f * 255;

    RecompuiColor border_color;
    border_color.r = 255;
    border_color.g = 255;
    border_color.b = 255;
    border_color.a = 0.2f * 255;

    RecompuiColor modal_color;
    modal_color.r = 8;
    modal_color.g = 7;
    modal_color.b = 13;
    modal_color.a = 0.9f * 255;

    const float body_padding = 64.0f;
    const float modal_height = RECOMPUI_TOTAL_HEIGHT - (2 * body_padding);
    const float modal_max_width = modal_height * (16.0f / 9.0f);
    const float modal_border_width = 1.1f;
    const float modal_border_radius = 16.0f;

    context = recompui_create_context();
    recompui_open_context(context);

    root = recompui_context_root(context);
    // Set up the root element so it takes up the full screen.
    recompui_set_position(root, POSITION_ABSOLUTE);
    recompui_set_top(root, 0, UNIT_DP);
    recompui_set_right(root, 0, UNIT_DP);
    recompui_set_bottom(root, 0, UNIT_DP);
    recompui_set_left(root, 0, UNIT_DP);
    recompui_set_width_auto(root);
    recompui_set_height_auto(root);

    // Set up the root element's padding so the modal contents don't touch the screen edges.
    recompui_set_padding(root, body_padding, UNIT_DP);
    // recompui_set_background_color(root, &bg_color);

    // Set up the flexbox properties of the root element.
    recompui_set_flex_direction(root, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(root, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(root, ALIGN_ITEMS_FLEX_START);

    // Create a container to act as the modal background and hold the elements in the modal.
    container = recompui_create_element(context, root);

    // Take up the a fixed height and the full width, up to a maximum width.
    recompui_set_height(container, 25.0f, UNIT_PERCENT);
    recompui_set_flex_grow(container, 1.0f);
    recompui_set_max_width(container, modal_max_width, UNIT_DP);
    recompui_set_width(container, 33.0f, UNIT_PERCENT);

    // Set up the properties of the container.
    recompui_set_display(container, DISPLAY_FLEX);
    recompui_set_justify_content(container, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_flex_direction(container, FLEX_DIRECTION_COLUMN);
    recompui_set_padding(container, 16.0f, UNIT_DP);
    recompui_set_gap(container, 16.0f, UNIT_DP);
    recompui_set_align_items(container, ALIGN_ITEMS_FLEX_START);

    // Set up the container to be the modal's background.
    recompui_set_border_width(container, modal_border_width, UNIT_DP);
    recompui_set_border_radius(container, modal_border_radius, UNIT_DP);
    recompui_set_border_color(container, &border_color);
    recompui_set_background_color(container, &modal_color);

    row1 = recompui_create_element(context, container);
    recompui_set_flex_basis(row1, 100.0f, UNIT_DP);
    recompui_set_flex_grow(row1, 0);
    recompui_set_flex_shrink(row1, 0);
    recompui_set_display(row1, DISPLAY_FLEX);
    recompui_set_flex_direction(row1, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(row1, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(row1, ALIGN_ITEMS_FLEX_END);
    recompui_set_gap(row1, 16.0f, UNIT_DP);

    rowCategory = recompui_create_element(context, container);
    recompui_set_flex_basis(rowCategory, 100.0f, UNIT_DP);
    recompui_set_flex_grow(rowCategory, 0);
    recompui_set_flex_shrink(rowCategory, 0);
    recompui_set_display(rowCategory, DISPLAY_FLEX);
    recompui_set_flex_direction(rowCategory, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(rowCategory, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(rowCategory, ALIGN_ITEMS_CENTER);
    recompui_set_gap(rowCategory, 16.0f, UNIT_DP);

    buttonClose = recompui_create_button(context, row1, "Apply", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(buttonClose, TEXT_ALIGN_CENTER);

    // Bind the shared callback to the two buttons.
    recompui_register_callback(buttonClose, closeButtonPressed, NULL);

    // set up scrolling container for models
    containerModelList = recompui_create_element(context, container);
    recompui_set_flex_basis(containerModelList, 100.0f, UNIT_DP);
    recompui_set_flex_grow(containerModelList, 1.0f);
    recompui_set_flex_shrink(containerModelList, 0.0f);
    recompui_set_display(containerModelList, DISPLAY_FLEX);
    recompui_set_flex_direction(containerModelList, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(containerModelList, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(containerModelList, ALIGN_ITEMS_FLEX_START);
    recompui_set_gap(containerModelList, 16.0f, UNIT_DP);
    recompui_set_overflow_y(containerModelList, OVERFLOW_SCROLL);
    recompui_set_overflow_x(containerModelList, OVERFLOW_HIDDEN);

    row2 = recompui_create_element(context, container);
    recompui_set_flex_basis(row2, 100.0f, UNIT_DP);
    recompui_set_flex_grow(row2, 0.0f);
    recompui_set_flex_shrink(row2, 0.0f);
    recompui_set_display(row2, DISPLAY_FLEX);
    recompui_set_flex_direction(row2, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(row2, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(row2, ALIGN_ITEMS_FLEX_END);
    recompui_set_gap(row2, 0.0f, UNIT_DP);

    // set up and hook up remove model button to close button and vice versa
    buttonRemoveModel = recompui_create_button(context, containerModelList, "Remove Model(s)", BUTTONSTYLE_SECONDARY);
    recompui_set_flex_shrink(buttonRemoveModel, 0);
    recompui_register_callback(buttonRemoveModel, removeButtonPressed, NULL);
    recompui_set_width(buttonRemoveModel, 100.0f, UNIT_PERCENT);
    recompui_set_text_align(buttonRemoveModel, TEXT_ALIGN_CENTER);

    refreshCategoryName();

    recompui_close_context(context);

    context_shown = false;

    _internal_onReadyUI();
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

static void refreshButtonEntryColors() {
    if (isSelectingCategory()) {
        return;
    }

    if (isSelectingModel()) {
        void *entry = (void *)CMEM_getCurrentEntry(getCurrentCategoryInfo()->category);
        {
            size_t count = YAZMTCore_DynamicDataArray_size(sButtonEntries);
            ButtonEntry *entries = YAZMTCore_DynamicDataArray_data(sButtonEntries);

            for (size_t i = 0; i < count; ++i) {
                RecompuiResource button = entries[i].button;
                if (entry == entries[i].data.ptr) {
                    recompui_set_background_color(button, &sModelSelectedButtonColor.bgColor);
                    recompui_set_border_color(button, &sModelSelectedButtonColor.borderColor);
                } else {
                    recompui_set_background_color(button, &sPrimaryButtonColor.bgColor);
                    recompui_set_border_color(button, &sPrimaryButtonColor.borderColor);
                }
            }
        }
    }
}

static void onModelButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (context_shown) {
        ModelEntry *entry = userdata;

        CategoryInfo *catInf = getCurrentCategoryInfo();

        if (catInf) {
            Link_CustomModelCategory cat = getCategoryFromModelType(entry->type);

            if (data->type == UI_EVENT_CLICK) {
                Audio_PlaySfx(NA_SE_SY_DECIDE);

                CMEM_tryApplyEntry(cat, entry);

                catInf->realEntry = entry;

                refreshButtonEntryColors();

                catInf->isNeedsDiskSave = true;
            } else if (data->type == UI_EVENT_HOVER || data->type == UI_EVENT_FOCUS) {
                setAuthor(entry->authorName);

                if (shouldLivePreview()) {
                    CMEM_tryApplyEntry(cat, entry);
                }
            }
        }
    }
}

static void onCategoryButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (context_shown) {
        if (data->type == UI_EVENT_CLICK) {
            ButtonEntry *entry = userdata;

            if (entry) {
                sCurrentCategoryInfo = entry->data.index;
                refreshFileList();
            }
        }
    }
}

static void onUpOneLevelButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (context_shown) {
        if (data->type == UI_EVENT_CLICK) {
            if (isSelectingModel()) {
                Audio_PlaySfx(NA_SE_SY_DECIDE);
                sCurrentCategoryInfo = SELECTING_CATEGORY;
                refreshFileList();
            } else {
                Audio_PlaySfx(NA_SE_SY_ERROR);
            }
        }
    }
}

static void destroyUpOneLevelButton() {
    if (buttonUpOneMenuLevel) {
        recompui_destroy_element(row1, buttonUpOneMenuLevel);
        buttonUpOneMenuLevel = 0;
        recompui_set_nav_auto(buttonClose, NAVDIRECTION_RIGHT);
    }
}

static void createUpOneLevelButton() {
    destroyUpOneLevelButton();
    buttonUpOneMenuLevel = recompui_create_button(context, row1, "▲", BUTTONSTYLE_SECONDARY);
    recompui_register_callback(buttonUpOneMenuLevel, onUpOneLevelButtonPressed, NULL);
    recompui_set_nav(buttonUpOneMenuLevel, NAVDIRECTION_DOWN, buttonRemoveModel);
    recompui_set_nav(buttonUpOneMenuLevel, NAVDIRECTION_LEFT, buttonClose);
    recompui_set_nav(buttonClose, NAVDIRECTION_RIGHT, buttonUpOneMenuLevel);
}

static void destroyModelButtons() {
    {
        size_t count = YAZMTCore_DynamicDataArray_size(sButtonEntries);
        ButtonEntry *entries = YAZMTCore_DynamicDataArray_data(sButtonEntries);

        for (size_t i = 0; i < count; ++i) {
            recompui_destroy_element(containerModelList, entries[i].button);
        }
    }

    YAZMTCore_DynamicDataArray_clear(sButtonEntries);

    if (isSelectingCategory()) {
        destroyNextPrevCategoryButtons();
        destroyUpOneLevelButton();
    }
}

static void createModelButtons() {
    // MUST CALL INSIDE UI CONTEXT

    bool isCategoryMode = isSelectingCategory();
    bool isModelMode = isSelectingModel();

    CategoryInfo *catInf = isModelMode ? getCurrentCategoryInfo() : NULL;

    if (catInf || isCategoryMode) {
        if (isCategoryMode) {
            for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
                CategoryInfo *curr = &sCategoryInfos[i];

                if (curr->isVisible) {
                    ButtonEntry *entry = YAZMTCore_DynamicDataArray_createElement(sButtonEntries);
                    entry->button = recompui_create_button(context, containerModelList, curr->displayName, BUTTONSTYLE_PRIMARY);
                    entry->data.index = i;

                    recompui_register_callback(entry->button, onCategoryButtonPressed, entry);
                    recompui_set_flex_shrink(entry->button, 0.0f);
                    recompui_set_text_align(entry->button, TEXT_ALIGN_CENTER);
                    recompui_set_width(entry->button, 100.0f, UNIT_PERCENT);
                }
            }

            recompui_set_nav(buttonClose, NAVDIRECTION_DOWN, buttonRemoveModel);
            recompui_set_nav(buttonClose, NAVDIRECTION_RIGHT, buttonRemoveModel);
            recompui_set_nav(buttonRemoveModel, NAVDIRECTION_UP, buttonClose);
        } else if (isModelMode) {
            if (!buttonUpOneMenuLevel) {
                createUpOneLevelButton();
            }

            if (!buttonCategoryNext || !buttonCategoryPrev) {
                createNextPrevCategoryButtons();
            }

            recompui_set_nav(buttonUpOneMenuLevel, NAVDIRECTION_RIGHT, buttonCategoryPrev);
            recompui_set_nav(buttonUpOneMenuLevel, NAVDIRECTION_DOWN, buttonCategoryNext);
            recompui_set_nav(buttonCategoryNext, NAVDIRECTION_UP, buttonUpOneMenuLevel);
            recompui_set_nav(buttonCategoryPrev, NAVDIRECTION_LEFT, buttonUpOneMenuLevel);

            size_t count = 0;
            ModelEntry **modelEntries = CMEM_getCategoryEntryData(catInf->category, &count);

            for (size_t i = 0; i < count; ++i) {
                const char *name = NULL;

                name = modelEntries[i]->displayName;
                if (!name) {
                    name = modelEntries[i]->internalName;
                }

                ButtonEntry *buttonEntry = YAZMTCore_DynamicDataArray_createElement(sButtonEntries);
                buttonEntry->button = recompui_create_button(context, containerModelList, name, BUTTONSTYLE_PRIMARY);
                buttonEntry->data.ptr = modelEntries[i];

                recompui_set_text_align(buttonEntry->button, TEXT_ALIGN_CENTER);
                recompui_set_width(buttonEntry->button, 100.0f, UNIT_PERCENT);
                recompui_register_callback(buttonEntry->button, onModelButtonPressed, modelEntries[i]);
                recompui_set_flex_shrink(buttonEntry->button, 0.0f);
            }
        }

        size_t buttonCount = YAZMTCore_DynamicDataArray_size(sButtonEntries);
        if (buttonCount > 0) {
            ButtonEntry *buttonEntries = YAZMTCore_DynamicDataArray_data(sButtonEntries);

            RecompuiResource buttonFirst = buttonEntries[0].button;
            RecompuiResource buttonLast = buttonEntries[buttonCount - 1].button;

            recompui_set_nav(buttonClose, NAVDIRECTION_UP, buttonLast);
            recompui_set_nav(buttonRemoveModel, NAVDIRECTION_RIGHT, buttonLast);
            recompui_set_nav(buttonRemoveModel, NAVDIRECTION_DOWN, buttonFirst);

            recompui_set_nav(buttonFirst, NAVDIRECTION_UP, buttonRemoveModel);
            recompui_set_nav(buttonFirst, NAVDIRECTION_LEFT, buttonRemoveModel);

            recompui_set_nav(buttonLast, NAVDIRECTION_LEFT, buttonRemoveModel);

            if (buttonCount > 1) {
                recompui_set_nav(buttonFirst, NAVDIRECTION_DOWN, buttonEntries[1].button);
                recompui_set_nav(buttonFirst, NAVDIRECTION_RIGHT, buttonLast);

                recompui_set_nav(buttonLast, NAVDIRECTION_UP, buttonEntries[buttonCount - 2].button);

                for (size_t i = 1; i < buttonCount - 1; ++i) {
                    RecompuiResource button = buttonEntries[i].button;
                    recompui_set_nav(button, NAVDIRECTION_UP, buttonEntries[i - 1].button);
                    recompui_set_nav(button, NAVDIRECTION_DOWN, buttonEntries[i + 1].button);
                    recompui_set_nav(button, NAVDIRECTION_LEFT, buttonRemoveModel);
                    recompui_set_nav(button, NAVDIRECTION_RIGHT, buttonLast);
                }
            }

            recompui_set_nav(buttonLast, NAVDIRECTION_DOWN, buttonClose);
            recompui_set_nav_none(buttonLast, NAVDIRECTION_RIGHT);
        } else {
            recompui_set_nav(buttonClose, NAVDIRECTION_UP, buttonRemoveModel);
            recompui_set_nav_none(buttonRemoveModel, NAVDIRECTION_RIGHT);
            recompui_set_nav(buttonRemoveModel, NAVDIRECTION_DOWN, buttonClose);
        }
    }
}

static void refreshFileList() {
    // MUST CALL INSIDE UI CONTEXT
    destroyModelButtons();
    createModelButtons();
    refreshCategoryName();
    refreshButtonEntryColors();
}

typedef enum {
    MODCFG_BUTTON_COMBO_NONE,
    MODCFG_BUTTON_COMBO_LR,
    MODCFG_BUTTON_COMBO_LA,
} ModConfig_ButtonCombo;

static bool isOpenMenuComboPressed(PlayState *play) {
    Input *input = CONTROLLER1(&play->state);

    switch (recomp_get_config_u32("open_menu_buttons")) {
        case MODCFG_BUTTON_COMBO_LR:
            return (CHECK_BTN_ALL(input->press.button, BTN_L) && CHECK_BTN_ALL(input->cur.button, BTN_R)) ||
                   (CHECK_BTN_ALL(input->cur.button, BTN_L) && CHECK_BTN_ALL(input->press.button, BTN_R));
            break;

        case MODCFG_BUTTON_COMBO_LA:
            return (CHECK_BTN_ALL(input->press.button, BTN_L) && CHECK_BTN_ALL(input->cur.button, BTN_A)) ||
                   (CHECK_BTN_ALL(input->cur.button, BTN_L) && CHECK_BTN_ALL(input->press.button, BTN_A));
            break;

        default:
            break;
    }

    return false;
}

void openModelMenu() {
    if (!context_shown) {
        sIsLivePreviewEnabled = recomp_get_config_u32("is_live_preview_enabled");
        fillRealEntries();
        recompui_show_context(context);
        context_shown = true;
    }
}

// Hook Play_UpdateMain to check if the L button is pressed and show this mod's UI if so.
RECOMP_HOOK("Play_UpdateMain")
void on_play_update(PlayState *play) {
    if (isOpenMenuComboPressed(play)) {
        openModelMenu();
    }
}

RECOMP_CALLBACK(".", _internal_onFinishedRegisterModels)
void populateFirstFileList() {
    for (int i = 0; i < ARRAY_COUNT(sCategoryInfos); ++i) {
        CategoryInfo *catInf = &sCategoryInfos[i];
        size_t count = 0;
        CMEM_getCategoryEntryData(catInf->category, &count);
        catInf->isVisible = catInf->isVisible || (catInf->isUsedByCurrentGame && count > 0);
    }

    recompui_open_context(context);
    refreshFileList();
    refreshButtonEntryColors();
    recompui_close_context(context);
}

RECOMP_CALLBACK(".", _internal_onSavedModelsApplied)
void refreshButtonsWhenSavedModelApplied() {
    recompui_open_context(context);
    refreshButtonEntryColors();
    recompui_close_context(context);
}
