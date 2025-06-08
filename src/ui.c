#include "modding.h"
#include "sys_cmpdma.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "recompui.h"
#include "custommodelentrymanager.h"
#include "model_common.h"

void refreshFileList();

typedef struct {
    CustomModelEntry *modelEntry;
    RecompuiResource button;
} ListEntry;

RecompuiContext context;
RecompuiResource root;
RecompuiResource container;
RecompuiResource row1;
RecompuiResource buttonClose;
RecompuiResource buttonRemoveModel;
RecompuiResource buttonRefreshFiles;

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

RecompuiResource modelListContainer;
RecompuiResource uiTitle;

void removeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        CMEM_removeModel(&gLinkFormProxies[PLAYER_FORM_HUMAN]);
    }
}

void closeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(context);
        context_shown = false;
    }
}

void refreshButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        refreshFileList();
    }
}

RECOMP_DECLARE_EVENT(PlayerModelManager_internal_onReadyUI());

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

    buttonClose = recompui_create_button(context, row1, "Close", BUTTONSTYLE_SECONDARY);
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
    modelListContainer = recompui_create_element(context, container);
    recompui_set_flex_basis(modelListContainer, 100.0f, UNIT_DP);
    recompui_set_flex_grow(modelListContainer, 1.0f);
    recompui_set_flex_shrink(modelListContainer, 0.0f);
    recompui_set_display(modelListContainer, DISPLAY_FLEX);
    recompui_set_flex_direction(modelListContainer, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(modelListContainer, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(modelListContainer, ALIGN_ITEMS_FLEX_START);
    recompui_set_gap(modelListContainer, 16.0f, UNIT_DP);
    recompui_set_overflow_y(modelListContainer, OVERFLOW_SCROLL);
    recompui_set_overflow_x(modelListContainer, OVERFLOW_HIDDEN);

    // set up and hook up remove model button to close button and vice versa
    buttonRemoveModel = recompui_create_button(context, modelListContainer, "Remove Model", BUTTONSTYLE_SECONDARY);
    recompui_set_flex_shrink(buttonRemoveModel, 0);
    recompui_set_nav(buttonClose, NAVDIRECTION_DOWN, buttonRemoveModel);
    recompui_set_nav(buttonRemoveModel, NAVDIRECTION_UP, buttonClose);
    recompui_set_nav(buttonRemoveModel, NAVDIRECTION_LEFT, buttonClose);
    recompui_register_callback(buttonRemoveModel, removeButtonPressed, NULL);

    buttonRefreshFiles = recompui_create_button(context, row1, "Refresh File List", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(buttonRefreshFiles, TEXT_ALIGN_CENTER);
    recompui_set_nav(buttonRefreshFiles, NAVDIRECTION_DOWN, buttonRemoveModel);
    recompui_set_nav(buttonRefreshFiles, NAVDIRECTION_RIGHT, buttonRemoveModel);
    recompui_register_callback(buttonRefreshFiles, refreshButtonPressed, NULL);

    recompui_close_context(context);

    context_shown = false;

    PlayerModelManager_internal_onReadyUI();
}

void onModelButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        CMEM_tryApplyEntry(userdata, &gLinkFormProxies[PLAYER_FORM_HUMAN]);
    }
}

typedef struct {
    CustomModelEntry** entries;
    RecompuiResource *buttons;
    size_t count;
    size_t capacity;
} ModelButtonEntries;

static ModelButtonEntries sButtonEntries;

void destroyModelButtons() {
    if (sButtonEntries.entries) {
        recomp_free(sButtonEntries.entries);
        sButtonEntries.entries = NULL;

        for (size_t i = 0; i < sButtonEntries.count; ++i) {
            recompui_destroy_element(modelListContainer, sButtonEntries.buttons[i]);
        }
    }
}

void createModelButtons() {
    // MUST CALL INSIDE UI CONTEXT

    CMEM_refreshDiskEntries();

    sButtonEntries.entries = CMEM_getCombinedEntries(&sButtonEntries.count);

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

        RecompuiResource button = recompui_create_button(context, modelListContainer, name, BUTTONSTYLE_PRIMARY);

        recompui_register_callback(button, onModelButtonPressed, sButtonEntries.entries[i]);

        recompui_set_flex_shrink(button, 0.0f);

        sButtonEntries.buttons[i] = button;
    }
}

void refreshFileList() {
    // MUST CALL INSIDE UI CONTEXT
    destroyModelButtons();
    createModelButtons();
}

bool checkButtonCombo(PlayState *play) {
    Input *input = CONTROLLER1(&play->state);
    return (CHECK_BTN_ALL(input->press.button, BTN_L) && CHECK_BTN_ALL(input->cur.button, BTN_R)) ||
           (CHECK_BTN_ALL(input->cur.button, BTN_L) && CHECK_BTN_ALL(input->press.button, BTN_R));
}

// Hook Play_UpdateMain to check if the L button is pressed and show this mod's UI if so.
RECOMP_HOOK("Play_UpdateMain")
void on_play_update(PlayState *play) {
    if (checkButtonCombo(play)) {
        if (!context_shown) {
            recompui_show_context(context);
            context_shown = true;
        }
    }
}

RECOMP_CALLBACK(".", PlayerModelManager_internal_onFinishedRegisterModels)
void populateFirstFileList() {
    recompui_open_context(context);
    refreshFileList();
    recompui_close_context(context);
}
