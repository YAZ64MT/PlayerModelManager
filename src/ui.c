#include "modding.h"
#include "sys_cmpdma.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "recompui.h"
#include "recompdata.h"
#include "z64recomp_api.h"
#include "simplefileloader.h"
#include "ml64compat_mm.h"
#include "model_common.h"
#include "mm_adultfixes.h"
#include "zproxy_manager.h"

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());

RecompuiContext context;
RecompuiResource root;
RecompuiResource container;
RecompuiResource row1;
RecompuiResource button;
RecompuiResource button2;
RecompuiResource textinput;
RecompuiResource textinput_button;

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

char *zobjDir;

RecompuiResource modelListContainer;

void button_pressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        if (resource == button) {
            recomp_printf("Pressed button 1\n");
            showing_bow = !showing_bow;
            if (showing_bow) {
                recompui_set_imageview_texture(imageview, bow_texture_handle);
            } else {
                recompui_set_imageview_texture(imageview, bomb_texture_handle);
            }
            recomp_printf("Slider value: %d (%5.2f)\n", recompui_get_input_value_u32(slider), recompui_get_input_value_float(slider));
            recomp_printf("Label radio value: %d (%5.2f)\n", recompui_get_input_value_u32(labelradio), recompui_get_input_value_float(labelradio));
            char *password_value = recompui_get_input_text(passwordinput);
            if (password_value != NULL) {
                recomp_printf("Password value: %s\n", password_value);
                recomp_free(password_value);
            }
        } else {
            recomp_printf("Pressed close button\n");
            recompui_hide_context(context);
            context_shown = false;
        }
    }
}

void textinput_enter_pressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        char *textinput_value = recompui_get_input_text(textinput);
        recomp_printf("  Textinput submitted: %s\n", textinput_value);
        recomp_free(textinput_value);
        recompui_set_input_value_u32(slider, 0);
        recompui_set_input_value_float(labelradio, 0.0);
    }
}

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
    recompui_set_background_color(root, &bg_color);

    // Set up the flexbox properties of the root element.
    recompui_set_flex_direction(root, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(root, JUSTIFY_CONTENT_CENTER);
    recompui_set_align_items(root, ALIGN_ITEMS_CENTER);

    // Create a container to act as the modal background and hold the elements in the modal.
    container = recompui_create_element(context, root);

    // Take up the a fixed height and the full width, up to a maximum width.
    recompui_set_height(container, 25.0f, UNIT_PERCENT);
    recompui_set_flex_grow(container, 1.0f);
    recompui_set_max_width(container, modal_max_width, UNIT_DP);
    recompui_set_width(container, 100.0f, UNIT_PERCENT);

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

    // Create the two rows in the container.
    row1 = recompui_create_element(context, container);
    recompui_set_flex_basis(row1, 100.0f, UNIT_DP);
    recompui_set_flex_grow(row1, 0);
    recompui_set_display(row1, DISPLAY_FLEX);
    recompui_set_flex_direction(row1, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(row1, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(row1, ALIGN_ITEMS_FLEX_END);
    recompui_set_gap(row1, 16.0f, UNIT_DP);

    // Create some buttons.
    button = recompui_create_button(context, row1, "Swap item", BUTTONSTYLE_PRIMARY);
    recompui_set_text_align(button, TEXT_ALIGN_CENTER);

    button2 = recompui_create_button(context, row1, "Close", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(button2, TEXT_ALIGN_CENTER);

    // Bind the shared callback to the two buttons.
    recompui_register_callback(button, button_pressed, NULL);
    recompui_register_callback(button2, button_pressed, NULL);

    // Create a text input
    textinput = recompui_create_textinput(context, row1);

    // Create a button to correspond to the text input
    textinput_button = recompui_create_button(context, row1, "Start", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(textinput_button, TEXT_ALIGN_CENTER);
    recompui_set_flex_basis(textinput_button, 150.0f, UNIT_DP);
    recompui_set_flex_grow(textinput_button, 0.0f);
    recompui_set_flex_shrink(textinput_button, 0.0f);

    // Bind the callback for the text input button.
    recompui_register_callback(textinput_button, textinput_enter_pressed, NULL);

    // Load the texture data and create UI textures for the bomb and bow icons.
    CmpDma_LoadFile(SEGMENT_ROM_START(icon_item_static_yar), ITEM_BOMB, bomb_texture_data, sizeof(bomb_texture_data));
    bomb_texture_handle = recompui_create_texture_rgba32(bomb_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);
    CmpDma_LoadFile(SEGMENT_ROM_START(icon_item_static_yar), ITEM_BOW, bow_texture_data, sizeof(bow_texture_data));
    bow_texture_handle = recompui_create_texture_rgba32(bow_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);

    // Create an imageview for the item icon.
    imageview = recompui_create_imageview(context, row1, bomb_texture_handle);
    recompui_set_min_width(imageview, 100.0f, UNIT_DP);
    recompui_set_min_height(imageview, 100.0f, UNIT_DP);

    // Create a label above the password
    recompui_create_label(context, container, "Password", LABELSTYLE_NORMAL);

    // Create a password input
    passwordinput = recompui_create_passwordinput(context, container);
    recompui_set_width(passwordinput, 400.0f, UNIT_DP);

    // Create a label above the password
    recompui_create_label(context, container, "Slider", LABELSTYLE_NORMAL);

    // Create a slider
    slider = recompui_create_slider(context, container, SLIDERTYPE_INTEGER, 0.0f, 100.0f, 1.0f, 45.0f);
    recompui_set_width(slider, 380.0f, UNIT_DP);

    // Create a label above the password
    recompui_create_label(context, container, "Label Radio", LABELSTYLE_NORMAL);

    // Create a label radio
    const char *labelradio_options[] = {
        "First",
        "Second",
        "Third"};
    labelradio = recompui_create_labelradio(context, container, labelradio_options, ARRAY_COUNT(labelradio_options));

    recompui_close_context(context);

    context_shown = false;

    unsigned char *modFolderPath = recomp_get_mod_folder_path();
    zobjDir = SFL_getCombinedPath(2, modFolderPath, "zobj");
    recomp_free(modFolderPath);
    recomp_printf("zobj folder path: %s\n", zobjDir);
}

typedef struct {
    char **files;
    RecompuiResource *buttons;
    u32 *buttonIndexes;
    u32 length;
} FileList;

FileList detectedFiles = {.files = NULL, .length = 0, .buttons = NULL, .buttonIndexes = NULL};
FileList oldFileList = {.files = NULL, .length = 0, .buttons = NULL};

char NULL_STRING[] = "null";

void freeOldFiles() {

    if (oldFileList.files) {
        for (u32 i = 0; i < oldFileList.length; i++) {
            if (oldFileList.files[i] == NULL_STRING) {
                oldFileList.files[i] = NULL;
            }
        }

        for (u32 i = 0; i < oldFileList.length; i++) {
            if (oldFileList.files[i]) {
                recomp_free(oldFileList.files[i]);
            }

            if (oldFileList.buttons[i]) {
                recompui_destroy_element(container, oldFileList.buttons[i]);
            }
        }

        recomp_free(oldFileList.files);
        recomp_free(oldFileList.buttons);
        recomp_free(oldFileList.buttonIndexes);

        oldFileList.buttons = NULL;
        oldFileList.files = NULL;
        oldFileList.buttonIndexes = 0;
        oldFileList.length = 0;
    }
}

extern Gfx gBottleGlassDL[];
extern Gfx gBottleContentsDL[];

u8* currentZobj = NULL;

void button_zobj_pressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        u32 index = *(u32 *)userdata;

        if (index >= 0 && index < detectedFiles.length && detectedFiles.files[index] != NULL_STRING) {

            char *zobjPath = SFL_getCombinedPath(2, zobjDir, detectedFiles.files[index]);

            SFL_File newZobjFile = SFL_loadFile(zobjPath);

            if (newZobjFile.data) {

                Link_FormProxy* humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

                setupZobjZ64o(&humanProxy->current, newZobjFile.data);
                refreshFormProxy(humanProxy);
                gIsAgePropertyRefreshRequested = true;
                matchFaceTexturesToProxy(&gLinkFormProxies[GET_PLAYER_FORM]);

                ZProxyManager_setCustomDisplayList(GAMEPLAY_KEEP, &gBottleGlassDL, &humanProxy->proxyDisplayLists[LINK_DL_BOTTLE_GLASS]);
                ZProxyManager_setCustomDisplayList(GAMEPLAY_KEEP, &gBottleContentsDL, &humanProxy->proxyDisplayLists[LINK_DL_BOTTLE_FILLING]);

                if (currentZobj) {
                    recomp_free(currentZobj);
                }

                currentZobj = newZobjFile.data;
            }
            
            recomp_free(zobjPath);
        } else {
            recomp_printf("Invalid button index: %d", index);
        }
    }
}

void refreshFileList() {
    freeOldFiles();

    recompui_open_context(context);

    u32 numFiles = SFL_getNumDirEntries(zobjDir);

    oldFileList = detectedFiles;

    detectedFiles.files = recomp_alloc(numFiles * sizeof(char *));
    detectedFiles.buttons = recomp_alloc(numFiles * sizeof(RecompuiResource));
    detectedFiles.buttonIndexes = recomp_alloc(numFiles * sizeof(u32));
    detectedFiles.length = numFiles;

    for (u32 i = 0; i < numFiles; i++) {
        detectedFiles.files[i] = SFL_getDirEntryNameByIndex(zobjDir, i);
        detectedFiles.buttonIndexes[i] = i;
        if (!detectedFiles.files[i]) {
            detectedFiles.files[i] = NULL_STRING;
            detectedFiles.buttons[i] = 0;
        } else {
            detectedFiles.buttons[i] = recompui_create_button(context, container, detectedFiles.files[i], BUTTONSTYLE_PRIMARY);
            recompui_register_callback(detectedFiles.buttons[i], button_zobj_pressed, &detectedFiles.buttonIndexes[i]);
        }
    }

    // recompui_destroy_element(container, labelradio);

    // labelradio = recompui_create_labelradio(context, container, detectedFiles.files, numFiles);

    freeOldFiles();

    recompui_close_context(context);
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
            refreshFileList();
            recompui_show_context(context);
            context_shown = true;
        }
    }
}
