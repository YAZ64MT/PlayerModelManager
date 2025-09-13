#include "global.h"
#include "modding.h"
#include "recompconfig.h"
#include "rt64_extended_gbi.h"
#include "colorfixes.h"
#include "model_common.h"

void setFormTunicColor(PlayerTransformation form, u8 r, u8 g, u8 b, u8 a) {
    if (form >= PLAYER_FORM_MAX) {
        return;
    }

    Link_TunicColor *tunicColor = &gLinkFormProxies[form].tunicColor;

    tunicColor->isOverrideRequested = true;

    tunicColor->requested.r = r;
    tunicColor->requested.g = g;
    tunicColor->requested.b = b;
    tunicColor->requested.a = a;
}

void setTunicColor(u8 r, u8 g, u8 b, u8 a) {
    for (PlayerTransformation i = 0; i < PLAYER_FORM_MAX; ++i) {
        setFormTunicColor(i, r, g, b, a);
    }
}

// Convert char to its numeric value
// Returns 0xFF if invalid character
u8 cToNum(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    return 0xFF;
}

u8 sToU8(const char *s) {
    return (cToNum(s[0]) << 4) | cToNum(s[1]);
}

bool isValidHexString(const char *s) {
    bool isValid = true;

    size_t len = 0;

    while (*s != '\0' && isValid) {
        if (len > 6 || cToNum(*s) == 0xFF) {
            isValid = false;
        }
        s++;
        len++;
    }

    if (len < 6) {
        isValid = false;
    }

    return isValid;
}

typedef enum {
    TUNIC_COLOR_OFF,
    TUNIC_COLOR_AUTO,
    TUNIC_COLOR_FORCE,
} TunicColorConfigOption;

RECOMP_HOOK("Player_Draw")
void readTunicColor_on_Player_Draw(Actor *thisx, PlayState *play) {
    Player *player = (Player *)thisx;

    Link_TunicColor *playerTunic = &gLinkFormProxies[player->transformation].tunicColor;

    TunicColorConfigOption tunicColorOpt = recomp_get_config_u32("is_modify_tunic_color");

    bool shouldPullFromConfig = false;

    switch (tunicColorOpt) {
        case TUNIC_COLOR_AUTO:
            shouldPullFromConfig = !playerTunic->isOverrideRequested;

            if (!shouldPullFromConfig) {
                playerTunic->current = playerTunic->requested;
            }
            break;

        case TUNIC_COLOR_FORCE:
            shouldPullFromConfig = true;
            break;

        default:
            break;
    }

    if (shouldPullFromConfig) {
        char *color = recomp_get_config_string("tunic_color");

        if (color) {
            if (isValidHexString(color)) {
                playerTunic->current.r = sToU8(color);
                playerTunic->current.g = sToU8(color + 2);
                playerTunic->current.b = sToU8(color + 4);
            }

            recomp_free_config_string(color);
        }
    }

    OPEN_DISPS(play->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, playerTunic->current.r, playerTunic->current.g, playerTunic->current.b, playerTunic->current.a);
    CLOSE_DISPS(play->state.gfxCtx);
}

extern Color_RGB8 sPlayerBottleColors[];

RECOMP_HOOK("func_80128640")
void fixBottleEnv_func_80128640(PlayState *play, Player *player, Gfx *dList) {
    if (player->leftHandType == PLAYER_MODELTYPE_LH_BOTTLE) {
        PlayerBottle bottle = Player_BottleFromIA(player, player->itemAction);

        OPEN_DISPS(play->state.gfxCtx);

        Color_RGB8 *bottleColor = &sPlayerBottleColors[bottle];

        gDPSetEnvColor(POLY_XLU_DISP++, bottleColor->r, bottleColor->g, bottleColor->b, 0);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}
