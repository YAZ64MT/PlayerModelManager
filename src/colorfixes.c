#include "global.h"
#include "modding.h"
#include "recompconfig.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} TunicColor;

static TunicColor sTunicColor = {
    .r = 30,
    .g = 105,
    .b = 27,
};

void fixTunicColor(PlayState *play) {
    if (recomp_get_config_u32("is_modify_tunic_color")) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPSetEnvColor(POLY_OPA_DISP++, sTunicColor.r, sTunicColor.g, sTunicColor.b, 0);

        CLOSE_DISPS(play->state.gfxCtx);
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

RECOMP_HOOK("Player_Draw")
void readTunicColor_on_Player_DrawGameplay(PlayState *play, Player *this, s32 lod, Gfx *cullDList, OverrideLimbDrawFlex overrideLimbDraw) {
    if (recomp_get_config_u32("is_modify_tunic_color")) {
        char *color = recomp_get_config_string("tunic_color");

        if (color) {
            if (isValidHexString(color)) {
                sTunicColor.r = sToU8(color);
                sTunicColor.g = sToU8(color + 2);
                sTunicColor.b = sToU8(color + 4);
            }
        }

        recomp_free_config_string(color);
    }
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayDefault")
void Recolor_OverrideLimbDrawDefault(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    fixTunicColor(play);
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson")
void Recolor_OverrideLimbDrawFirstPerson(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    fixTunicColor(play);
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
