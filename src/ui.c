#include "modding.h"
#include "sys_cmpdma.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "recompui.h"
#include "custommodelentrymanager.h"
#include "model_common.h"

void refreshFileList();
void refreshButtonEntryColors();

typedef struct {
    FormModelEntry *modelEntry;
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

RecompuiResource imageview;
RecompuiTextureHandle bomb_texture_handle;
u8 bomb_texture_data[ICON_ITEM_TEX_SIZE];
RecompuiTextureHandle bow_texture_handle;
u8 bow_texture_data[ICON_ITEM_TEX_SIZE];

RecompuiResource passwordinput;
RecompuiResource slider;
RecompuiResource labelradio;

bool context_shown = false;
bool showing_bow = false;

RecompuiResource containerModelList;
RecompuiResource labelUiTitle;
RecompuiResource labellabelModelAuthorPrefix;
RecompuiResource labelModelAuthor;

static FormModelEntry *sRealEntries[PLAYER_FORM_MAX];

static bool sIsDiskSaveNeeded = false;

static bool sIsLivePreviewEnabled = false;

static PlayerTransformation sSelectionOrder[] = {
    PLAYER_FORM_HUMAN,
    PLAYER_FORM_DEKU,
    PLAYER_FORM_GORON,
    PLAYER_FORM_ZORA,
    PLAYER_FORM_FIERCE_DEITY,
};

static PlayerTransformation sCurrentMenuIndex = 0;

PlayerTransformation getSelectedForm() {
    return sSelectionOrder[sCurrentMenuIndex];
}

bool shouldLivePreview() {
    return sIsLivePreviewEnabled && getSelectedForm() == GET_PLAYER_FORM;
}

void destroyAuthor() {
    if (labelModelAuthor) {
        recompui_destroy_element(row2, labelModelAuthor);
        recompui_destroy_element(row2, labellabelModelAuthorPrefix);
    }
    labelModelAuthor = 0;
    labellabelModelAuthorPrefix = 0;
}

void setAuthor(const char *author) {
    if (!author) {
        author = "N/A";
    }

    destroyAuthor();
    labellabelModelAuthorPrefix = recompui_create_label(context, row2, "Author(s): ", LABELSTYLE_NORMAL);
    labelModelAuthor = recompui_create_label(context, row2, author, LABELSTYLE_NORMAL);
}

void applyRealEntry() {
    CMEM_tryApplyEntry(getSelectedForm(), sRealEntries[getSelectedForm()]);
}

void removeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        Audio_PlaySfx(NA_SE_SY_DECIDE);
        sRealEntries[getSelectedForm()] = NULL;
        CMEM_tryApplyEntry(getSelectedForm(), NULL);
        refreshButtonEntryColors();
        sIsDiskSaveNeeded = true;
    } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
        destroyAuthor();

        if (shouldLivePreview()) {
            applyRealEntry();
        }
    }
}

void closeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        applyRealEntry();
        recompui_hide_context(context);
        context_shown = false;
        if (sIsDiskSaveNeeded) {
            for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
                CMEM_saveCurrentEntry(i);
            }
            Audio_PlaySfx(NA_SE_SY_PIECE_OF_HEART);
        } else {
            Audio_PlaySfx(NA_SE_SY_DECIDE);
        }
    } else if (data->type == UI_EVENT_FOCUS || data->type == UI_EVENT_HOVER) {
        destroyAuthor();

        if (shouldLivePreview()) {
            applyRealEntry();
        }
    }
}

void clearRealEntries() {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        sRealEntries[i] = NULL;
    }
}

void fillRealEntries() {
    for (int i = 0; i < PLAYER_FORM_MAX; ++i) {
        sRealEntries[i] = CMEM_getCurrentEntry(i);
    }
}

void destroyModelButtons();
void createModelButtons();
void refreshButtonEntryColors();

static bool sTrue = true;
static bool sFalse = false;

static bool sIsCategoryRowExist = false;

static const char *sCategoryNames[] = {
    "Fierce Deity",
    "Goron",
    "Zora",
    "Deku",
    "Human",
};

void refreshCategoryName() {
    if (sIsCategoryRowExist) {
        recompui_destroy_element(rowCategory, labelCategory);
    }

    labelCategory = recompui_create_label(context, rowCategory, sCategoryNames[getSelectedForm()], LABELSTYLE_LARGE);

    sIsCategoryRowExist = true;
}

void changeCategoryButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        Audio_PlaySfx(NA_SE_SY_DECIDE);

        bool *isNextButton = userdata;

        if (*isNextButton) {
            sCurrentMenuIndex++;

            while (sCurrentMenuIndex >= ARRAY_COUNT(sSelectionOrder)) {
                sCurrentMenuIndex -= ARRAY_COUNT(sSelectionOrder);
            }
        } else {
            sCurrentMenuIndex--;

            while ((signed)sCurrentMenuIndex < 0) {
                sCurrentMenuIndex += ARRAY_COUNT(sSelectionOrder);
            }
        }

        sCurrentMenuIndex = (sCurrentMenuIndex % ARRAY_COUNT(sSelectionOrder) + ARRAY_COUNT(sSelectionOrder)) % ARRAY_COUNT(sSelectionOrder);

        destroyModelButtons();
        createModelButtons();
        refreshButtonEntryColors();
        refreshCategoryName();
    }
}

RECOMP_DECLARE_EVENT(_internal_onReadyUI());

RECOMP_CALLBACK("*", recomp_on_init)
void on_init() {
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
    recompui_set_align_items(rowCategory, ALIGN_ITEMS_FLEX_END);
    recompui_set_gap(rowCategory, 16.0f, UNIT_DP);

    buttonClose = recompui_create_button(context, row1, "Apply", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(buttonClose, TEXT_ALIGN_CENTER);

    // Bind the shared callback to the two buttons.
    recompui_register_callback(buttonClose, closeButtonPressed, NULL);

    /*
    // Load the texture data and create UI textures for the bomb and bow icons.
    CmpDma_LoadFile(SEGMENT_ROM_START(icon_item_static_yar), ITEM_BOMB, bomb_texture_data, sizeof(bomb_texture_data));
    bomb_texture_handle = recompui_create_texture_rgba32(bomb_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);
    CmpDma_LoadFile(SEGMENT_ROM_START(icon_item_static_yar), ITEM_BOW, bow_texture_data, sizeof(bow_texture_data));
    bow_texture_handle = recompui_create_texture_rgba32(bow_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);

    // Create an imageview for the item icon.
    imageview = recompui_create_imageview(context, row1, bomb_texture_handle);
    recompui_set_min_width(imageview, 100.0f, UNIT_DP);
    recompui_set_min_height(imageview, 100.0f, UNIT_DP);
    */

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
    buttonRemoveModel = recompui_create_button(context, containerModelList, "Remove Model", BUTTONSTYLE_SECONDARY);
    recompui_set_flex_shrink(buttonRemoveModel, 0);
    recompui_register_callback(buttonRemoveModel, removeButtonPressed, NULL);
    recompui_set_width(buttonRemoveModel, 100.0f, UNIT_PERCENT);
    recompui_set_text_align(buttonRemoveModel, TEXT_ALIGN_CENTER);

    buttonCategoryPrev = recompui_create_button(context, rowCategory, "◀", BUTTONSTYLE_SECONDARY);
    buttonCategoryNext = recompui_create_button(context, rowCategory, "▶", BUTTONSTYLE_SECONDARY);

    recompui_register_callback(buttonCategoryPrev, changeCategoryButtonPressed, &sFalse);
    recompui_register_callback(buttonCategoryNext, changeCategoryButtonPressed, &sTrue);

    recompui_set_nav(buttonClose, NAVDIRECTION_RIGHT, buttonCategoryPrev);
    recompui_set_nav(buttonClose, NAVDIRECTION_DOWN, buttonCategoryPrev);

    recompui_set_nav(buttonRemoveModel, NAVDIRECTION_UP, buttonCategoryPrev);
    recompui_set_nav(buttonRemoveModel, NAVDIRECTION_LEFT, buttonCategoryNext);

    recompui_set_nav(buttonCategoryPrev, NAVDIRECTION_LEFT, buttonClose);
    recompui_set_nav(buttonCategoryPrev, NAVDIRECTION_DOWN, buttonRemoveModel);
    recompui_set_nav(buttonCategoryPrev, NAVDIRECTION_UP, buttonClose);

    recompui_set_nav(buttonCategoryNext, NAVDIRECTION_DOWN, buttonRemoveModel);
    recompui_set_nav(buttonCategoryNext, NAVDIRECTION_RIGHT, buttonRemoveModel);

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

typedef struct {
    FormModelEntry **entries;
    RecompuiResource *buttons;
    size_t count;
    size_t capacity;
} ModelButtonEntries;

static ModelButtonEntries sButtonEntries;

void refreshButtonEntryColors() {
    void *entry = (void *)CMEM_getCurrentEntry(getSelectedForm());
    for (size_t i = 0; i < sButtonEntries.count; ++i) {
        if (entry == sButtonEntries.entries[i]) {
            recompui_set_background_color(sButtonEntries.buttons[i], &sModelSelectedButtonColor.bgColor);
            recompui_set_border_color(sButtonEntries.buttons[i], &sModelSelectedButtonColor.borderColor);
        } else {
            recompui_set_background_color(sButtonEntries.buttons[i], &sPrimaryButtonColor.bgColor);
            recompui_set_border_color(sButtonEntries.buttons[i], &sPrimaryButtonColor.borderColor);
        }
    }
}

void onModelButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    FormModelEntry *entry = userdata;

    if (entry->type != PMM_MODEL_TYPE_NONE) {
        PlayerTransformation form = getFormFromModelType(entry->type);

        if (form < PLAYER_FORM_MAX) {
            if (data->type == UI_EVENT_CLICK) {
                Audio_PlaySfx(NA_SE_SY_DECIDE);

                CMEM_tryApplyEntry(form, entry);

                sRealEntries[getSelectedForm()] = entry;

                refreshButtonEntryColors();

                sIsDiskSaveNeeded = true;
            } else if (data->type == UI_EVENT_HOVER || data->type == UI_EVENT_FOCUS) {
                destroyAuthor();

                setAuthor(entry->authorName);

                if (shouldLivePreview()) {
                    CMEM_tryApplyEntry(form, entry);
                }
            }
        }
        else {
            Audio_PlaySfx(NA_SE_SY_ERROR);
        }
    }
}

void destroyModelButtons() {
    if (sButtonEntries.entries) {
        recomp_free(sButtonEntries.entries);
        sButtonEntries.entries = NULL;

        for (size_t i = 0; i < sButtonEntries.count; ++i) {
            recompui_destroy_element(containerModelList, sButtonEntries.buttons[i]);
        }
    }
}

void createModelButtons() {
    // MUST CALL INSIDE UI CONTEXT

    sButtonEntries.entries = CMEM_getCombinedEntries(getSelectedForm(), &sButtonEntries.count);

    if (sButtonEntries.count > sButtonEntries.capacity) {
        recomp_free(sButtonEntries.buttons);
        sButtonEntries.buttons = recomp_alloc(sizeof(*sButtonEntries.buttons) * sButtonEntries.count);
        sButtonEntries.capacity = sButtonEntries.count;
    }

    for (size_t i = 0; i < sButtonEntries.count; ++i) {
        const char *name = sButtonEntries.entries[i]->displayName;
        if (!name) {
            name = sButtonEntries.entries[i]->internalName;
        }

        RecompuiResource button = recompui_create_button(context, containerModelList, name, BUTTONSTYLE_PRIMARY);

        recompui_set_text_align(button, TEXT_ALIGN_CENTER);

        recompui_set_width(button, 100.0f, UNIT_PERCENT);

        recompui_register_callback(button, onModelButtonPressed, sButtonEntries.entries[i]);

        recompui_set_flex_shrink(button, 0.0f);

        sButtonEntries.buttons[i] = button;
    }

    if (sButtonEntries.count > 0) {
        RecompuiResource buttonFirst = sButtonEntries.buttons[0];
        RecompuiResource buttonLast = sButtonEntries.buttons[sButtonEntries.count - 1];

        recompui_set_nav(buttonClose, NAVDIRECTION_UP, buttonLast);
        recompui_set_nav(buttonRemoveModel, NAVDIRECTION_RIGHT, buttonLast);
        recompui_set_nav(buttonRemoveModel, NAVDIRECTION_DOWN, buttonFirst);

        recompui_set_nav(buttonFirst, NAVDIRECTION_UP, buttonRemoveModel);
        recompui_set_nav(buttonFirst, NAVDIRECTION_LEFT, buttonRemoveModel);

        recompui_set_nav(buttonLast, NAVDIRECTION_LEFT, buttonRemoveModel);

        if (sButtonEntries.count > 1) {
            recompui_set_nav(buttonFirst, NAVDIRECTION_DOWN, sButtonEntries.buttons[1]);
            recompui_set_nav(buttonFirst, NAVDIRECTION_RIGHT, buttonLast);

            recompui_set_nav(buttonLast, NAVDIRECTION_UP, sButtonEntries.buttons[sButtonEntries.count - 2]);

            for (size_t i = 1; i < sButtonEntries.count - 1; ++i) {
                RecompuiResource button = sButtonEntries.buttons[i];
                recompui_set_nav(button, NAVDIRECTION_UP, sButtonEntries.buttons[i - 1]);
                recompui_set_nav(button, NAVDIRECTION_DOWN, sButtonEntries.buttons[i + 1]);
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

void refreshFileList() {
    // MUST CALL INSIDE UI CONTEXT
    destroyModelButtons();
    //CMEM_refreshDiskEntries();
    createModelButtons();
}

typedef enum {
    MODCFG_BUTTON_COMBO_NONE,
    MODCFG_BUTTON_COMBO_LR,
    MODCFG_BUTTON_COMBO_LA,
} ModConfig_ButtonCombo;

bool isOpenMenuComboPressed(PlayState *play) {
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
        sIsDiskSaveNeeded = false;
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
    recompui_open_context(context);
    refreshFileList();
    recompui_close_context(context);
}

RECOMP_CALLBACK(".", _internal_onSavedModelsApplied)
void refreshButtonsWhenSavedModelApplied() {
    recompui_open_context(context);
    refreshButtonEntryColors();
    recompui_close_context(context);
}
