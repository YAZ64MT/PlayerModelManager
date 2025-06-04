#include "modding.h"
#include "sys_cmpdma.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "recompui.h"
#include "recompdata.h"
#include "z64recomp_api.h"
#include "qdfileloader_api.h"
#include "modelreplacer_api.h"
#include "ml64compat_mm.h"
#include "defines_z64o.h"
#include "model_common.h"
#include "mm_adultfixes.h"
#include "playermodelmanager_utils.h"
#include "custommodelentry.h"
#include "libc/string.h"

RECOMP_IMPORT("*", unsigned char *recomp_get_mod_folder_path());

typedef struct {
    CustomModelEntry modelEntry;
    RecompuiResource button;
} DiskModelEntry;

typedef struct {
    DiskModelEntry *entries;
    unsigned length;
    unsigned maxLength;
} DiskModelEntries;

RecompuiContext context;
RecompuiResource root;
RecompuiResource container;
RecompuiResource row1;
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

char *zobjDir;

void *currentZobj = NULL;

RecompuiResource modelListContainer;
RecompuiResource uiTitle;

void removeButtonPressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        Link_FormProxy *humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

        clearLinkModelInfo(&humanProxy->current);

        refreshFormProxy(humanProxy);

        gIsAgePropertyRefreshRequested = true;

        matchFaceTexturesToProxy(&GET_PLAYER_FORM_PROXY);

        if (currentZobj) {
            recomp_free(currentZobj);
        }

        currentZobj = NULL;
    }
}

void button_pressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(context);
        context_shown = false;
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
    recompui_register_callback(buttonClose, button_pressed, NULL);

    // Load the texture data and create UI textures for the bomb and bow icons.
    CmpDma_LoadFile(SEGMENT_ROM_START(icon_item_static_yar), ITEM_BOMB, bomb_texture_data, sizeof(bomb_texture_data));
    bomb_texture_handle = recompui_create_texture_rgba32(bomb_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);
    CmpDma_LoadFile(SEGMENT_ROM_START(icon_item_static_yar), ITEM_BOW, bow_texture_data, sizeof(bow_texture_data));
    bow_texture_handle = recompui_create_texture_rgba32(bow_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);

    // Create an imageview for the item icon.
    imageview = recompui_create_imageview(context, row1, bomb_texture_handle);
    recompui_set_min_width(imageview, 100.0f, UNIT_DP);
    recompui_set_min_height(imageview, 100.0f, UNIT_DP);

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
    recompui_register_callback(buttonRemoveModel, removeButtonPressed, NULL);

    recompui_close_context(context);

    context_shown = false;

    unsigned char *modFolderPath = recomp_get_mod_folder_path();
    zobjDir = QDFL_getCombinedPath(2, modFolderPath, "zobj");
    recomp_free(modFolderPath);
    recomp_printf("zobj folder path: %s\n", zobjDir);
}

DiskModelEntries gCurrentDiskModels;

void button_zobj_pressed(RecompuiResource resource, const RecompuiEventData *data, void *userdata) {
    if (data->type == UI_EVENT_CLICK) {
        DiskModelEntry *entry = userdata;

        char *zobjPath = QDFL_getCombinedPath(2, zobjDir, entry->modelEntry.filePath);

        void *newZobjFile = NULL;

        QDFL_loadFile(zobjPath, &newZobjFile);

        if (newZobjFile) {
            Link_FormProxy *humanProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

            setupZobjZ64o(&humanProxy->current, newZobjFile);

            refreshFormProxy(humanProxy);

            gIsAgePropertyRefreshRequested = true;

            matchFaceTexturesToProxy(&GET_PLAYER_FORM_PROXY);

            if (currentZobj) {
                recomp_free(currentZobj);
            }

            currentZobj = newZobjFile;
        }

        recomp_free(zobjPath);
    }
}

bool isValidZobj(const char *path) {
    char *zobjPath = QDFL_getCombinedPath(2, zobjDir, path);

    void *file = NULL;

    unsigned long fileSize = 0;

    QDFL_getFileSize(zobjPath, &fileSize);

    // playas zobjs always have an alias table of at least 0x800 bytes at offset 0x5000
    bool isValid = fileSize >= 0x5800;

    if (isValid) {
        QDFL_loadFile(zobjPath, &file);

        if (file) {
            u8 *zobj = file;

            const char ML64_HEADER[] = "MODLOADER64";

            isValid = true;

            for (unsigned i = 0; isValid && i < ARRAY_COUNT(ML64_HEADER) - 1; ++i) {
                if (zobj[i + Z64O_MODLOADER_HEADER] != ML64_HEADER[i]) {
                    isValid = false;
                }
            }

            recomp_free(file);
        }
    }

    recomp_free(zobjPath);

    return isValid;
}

void refreshFileList() {
    recompui_open_context(context);

    unsigned long numFiles;

    QDFL_STATUS err = QDFL_getNumDirEntries(zobjDir, &numFiles);

    if (err == QDFL_STATUS_OK) {

        if (gCurrentDiskModels.maxLength < numFiles) {
            DiskModelEntry *newEntries = recomp_alloc(sizeof(DiskModelEntry) * numFiles);

            Lib_MemCpy(newEntries, gCurrentDiskModels.entries, gCurrentDiskModels.length * sizeof(DiskModelEntry *));

            recomp_free(gCurrentDiskModels.entries);

            gCurrentDiskModels.entries = newEntries;

            gCurrentDiskModels.maxLength = numFiles;
        }

        for (unsigned i = 0; i < gCurrentDiskModels.length; ++i) {
            recompui_destroy_element(modelListContainer, gCurrentDiskModels.entries[i].button);
            if (gCurrentDiskModels.entries[i].modelEntry.filePath) {
                recomp_free(gCurrentDiskModels.entries[i].modelEntry.filePath);
                recomp_free(gCurrentDiskModels.entries[i].modelEntry.displayName);
            }
        }

        unsigned newLength = 0;
        char *path;

        for (unsigned i = 0; i < numFiles; i++) {
            path = NULL;

            QDFL_getDirEntryNameByIndex(zobjDir, i, &path);

            if (path && isValidZobj(path)) {
                DiskModelEntry *entry = &gCurrentDiskModels.entries[newLength];

                CustomModelEntry_init(&entry->modelEntry);

                entry->modelEntry.filePath = path;

                unsigned nameSize = strlen(path);

                entry->modelEntry.displayName = recomp_alloc(nameSize);

                strcpy(entry->modelEntry.displayName, path);

                char *c = &entry->modelEntry.displayName[nameSize - 1];

                // get rid of file extension
                while (c != entry->modelEntry.displayName) {
                    if (*c == '.') {
                        *c = '\0';
                        break;
                    }

                    c--;
                }

                RecompuiResource button = recompui_create_button(context, modelListContainer, entry->modelEntry.displayName, BUTTONSTYLE_PRIMARY);

                entry->button = button;

                recompui_set_flex_shrink(button, 0);

                recompui_register_callback(button, button_zobj_pressed, entry);

                newLength++;
            }
        }

        gCurrentDiskModels.length = newLength;
    }

    // freeOldFiles();

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
