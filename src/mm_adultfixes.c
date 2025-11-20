#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "formproxy.h"
#include "proxyactorext.h"
#include "overlays/actors/ovl_En_Rd/z_en_rd.h"
#include "overlays/actors/ovl_En_Railgibud/z_en_railgibud.h"
#include "overlays/actors/ovl_En_Talk_Gibud/z_en_talk_gibud.h"
#include "overlays/actors/ovl_Arms_Hook/z_arms_hook.h"

static bool shouldUseAdultFixes(Player *player) {
    if (player->transformation != PLAYER_FORM_HUMAN) {
        return false;
    }

    FormProxy *fp = ProxyActorExt_getFormProxy(&player->actor);

    return fp && FormProxy_isAdultModelType(fp);
}

Gfx **gFirstPersonDList;
FormProxy *sFormProxyFpsRightForearm;

RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson")
void addFPSrightForearm_on_OverrideLimbDrawFirstPerson(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    if (limbIndex == PLAYER_LIMB_RIGHT_FOREARM) {
        sFormProxyFpsRightForearm = ProxyActorExt_getFormProxy(actor);
        gFirstPersonDList = dList;
    } else {
        sFormProxyFpsRightForearm = NULL;
    }
}

RECOMP_HOOK_RETURN("Player_OverrideLimbDrawGameplayFirstPerson")
void addFPSrightForearm_on_return_OverrideLimbDrawFirstPerson() {
    if (sFormProxyFpsRightForearm) {
        *gFirstPersonDList = FormProxy_getCurrentDL(sFormProxyFpsRightForearm, LINK_DL_FPS_RFOREARM);
    }
}

PlayerAgeProperties gAdultLinkAgeProps;

extern PlayerAgeProperties sPlayerAgeProperties[];

void initAdultLinkAgeProperties() {
    PlayerAgeProperties *fdProps = &sPlayerAgeProperties[PLAYER_FORM_FIERCE_DEITY];
    gAdultLinkAgeProps = sPlayerAgeProperties[PLAYER_FORM_ZORA];

    gAdultLinkAgeProps.unk_28 = 44.6f;
    gAdultLinkAgeProps.unk_3C = 15.0f;
    gAdultLinkAgeProps.unk_44 = fdProps->unk_44;
    gAdultLinkAgeProps.unk_4A->x = fdProps->unk_4A->x;
    gAdultLinkAgeProps.unk_4A->y = fdProps->unk_4A->y;
    gAdultLinkAgeProps.unk_4A->z = fdProps->unk_4A->z;
    gAdultLinkAgeProps.unk_62->x = fdProps->unk_62->x;
    gAdultLinkAgeProps.unk_62->y = fdProps->unk_62->y;
    gAdultLinkAgeProps.unk_62->z = fdProps->unk_62->z;
    gAdultLinkAgeProps.unk_7A->x = fdProps->unk_7A->x;
    gAdultLinkAgeProps.unk_7A->y = fdProps->unk_7A->y;
    gAdultLinkAgeProps.unk_7A->z = fdProps->unk_7A->z;
    gAdultLinkAgeProps.voiceSfxIdOffset = SFX_VOICE_BANK_SIZE * 0;
    gAdultLinkAgeProps.surfaceSfxIdOffset = 0x80;
}

extern LinkAnimationHeader gPlayerAnim_clink_demo_doorA_link;
extern LinkAnimationHeader gPlayerAnim_clink_demo_doorB_link;

extern u8 sPlayerMass[PLAYER_FORM_MAX];
static u8 sHumanMass;

RECOMP_HOOK("Player_UpdateCommon")
void changePlayerMass_on_Player_UpdateCommon(Player *this, PlayState *play, Input *input) {
    sHumanMass = sPlayerMass[PLAYER_FORM_HUMAN];

    if (shouldUseAdultFixes(this)) {
        sPlayerMass[PLAYER_FORM_HUMAN] = sPlayerMass[PLAYER_FORM_ZORA];
    }
}

RECOMP_HOOK_RETURN("Player_UpdateCommon")
void changePlayerMass_on_return_Player_UpdateCommon() {
    sPlayerMass[PLAYER_FORM_HUMAN] = sHumanMass;
}

#define NUM_BOOT_PROPERTIES 18
extern s16 D_801BFE14[PLAYER_BOOTS_MAX][NUM_BOOT_PROPERTIES];
#define PLAYER_BOOTS_ARR D_801BFE14

static void copyBootProperties(s16 *dest, s16 *src) {
    for (int i = 0; i < NUM_BOOT_PROPERTIES; ++i) {
        dest[i] = src[i];
    }
}

s16 sBootsPropsTemp[NUM_BOOT_PROPERTIES];
static bool sIsBootsOverwritten;

RECOMP_HOOK("func_80123140")
void setAdultBootData_on_func_80123140(PlayState *play, Player *player) {
    sIsBootsOverwritten = shouldUseAdultFixes(player);

    if (sIsBootsOverwritten) {
        copyBootProperties(sBootsPropsTemp, PLAYER_BOOTS_ARR[PLAYER_BOOTS_HYLIAN]);
        copyBootProperties(PLAYER_BOOTS_ARR[PLAYER_BOOTS_HYLIAN], PLAYER_BOOTS_ARR[PLAYER_BOOTS_ZORA_LAND]);
    }
}

RECOMP_HOOK("func_80123140")
void setAdultBootData_on_return_func_80123140(PlayState *play, Player *player) {
    if (sIsBootsOverwritten) {
        copyBootProperties(PLAYER_BOOTS_ARR[PLAYER_BOOTS_HYLIAN], sBootsPropsTemp);
    }
}

extern PlayerAnimationHeader *D_8085BE84[PLAYER_ANIMGROUP_MAX][PLAYER_ANIMTYPE_MAX];
#define PLAYER_ANIM_GROUPS D_8085BE84

static PlayerAnimationHeader *sDoorAAnimsTmp[PLAYER_ANIMTYPE_MAX];
static PlayerAnimationHeader *sDoorBAnimsTmp[PLAYER_ANIMTYPE_MAX];

RECOMP_HOOK("Player_Door_Knob")
void replaceDoorAnim_on_Player_Door_Knob(PlayState *play, Player *this, Actor *door) {
    if (shouldUseAdultFixes(this)) {
        for (int i = 0; i < PLAYER_ANIMTYPE_MAX; i++) {
            sDoorAAnimsTmp[i] = PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorA][i];
            sDoorBAnimsTmp[i] = PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorB][i];
            PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorA][i] = D_8085BE84[PLAYER_ANIMGROUP_doorA_free][i];
            PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorB][i] = D_8085BE84[PLAYER_ANIMGROUP_doorB_free][i];
        }
    }
}

RECOMP_HOOK_RETURN("Player_Door_Knob")
void replaceDoorAnim_on_return_Player_Door_Knob() {
    for (int i = 0; i < PLAYER_ANIMTYPE_MAX; i++) {
        PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorA][i] = sDoorAAnimsTmp[i];
        PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorB][i] = sDoorBAnimsTmp[i];
    }
}

RECOMP_HOOK("Player_Init")
void initVanillaProps_on_Player_Init(Actor *thisx, PlayState *play) {
    static bool isFirstTimeInitDone;
    if (!isFirstTimeInitDone) {
        isFirstTimeInitDone = true;
        initAdultLinkAgeProperties();
    }
}

// Need to do this every frame or adult Link clips into the floor on unpause
RECOMP_HOOK("Play_Main")
void Play_Main(GameState *thisx) {
    PlayState *play = (PlayState *)thisx;

    Player *player = GET_PLAYER(play);

    while (player) {
        if (player->transformation == PLAYER_FORM_HUMAN) {
            if (shouldUseAdultFixes(player)) {
                player->ageProperties = &gAdultLinkAgeProps;
            } else {
                player->ageProperties = &sPlayerAgeProperties[PLAYER_FORM_HUMAN];
            }
        }

        player = (Player *)player->actor.next;
    }
}

static Player *sPlayerFromHeight;
static PlayerTransformation sRealPlayerFormHeight;

RECOMP_HOOK("Player_GetHeight")
void adjustHeight_on_Player_GetHeight(Player *player) {
    sPlayerFromHeight = player;
    sRealPlayerFormHeight = player->transformation;

    if (shouldUseAdultFixes(player)) {
        player->transformation = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("Player_GetHeight")
void adjustHeight_on_return_Player_GetHeight() {
    sPlayerFromHeight->transformation = sRealPlayerFormHeight;
}

// Use adult camera for first person bow/hookshot
static PlayerTransformation sRealPlayerFormFPCamera = PLAYER_FORM_HUMAN;
static Player *sPlayerFPCamera;

RECOMP_HOOK("func_8083868C")
void fixFPCmaera_on_func_8083868C(PlayState *play, Player *this) {
    sRealPlayerFormFPCamera = this->transformation;
    sPlayerFPCamera = this;

    if (shouldUseAdultFixes(this)) {
        this->transformation = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("func_8083868C")
void fixFPCmaera_on_return_func_8083868C() {
    sPlayerFPCamera->transformation = sRealPlayerFormFPCamera;
}

static PlayerTransformation sRealPlayerFormEnRdGrab;
static Player *sPlayerEnRdGrab;

RECOMP_HOOK("EnRd_Grab")
void fixEnemyHeight_on_EnRd_Grab(EnRd *this, PlayState *play) {
    Player *player = sPlayerEnRdGrab = GET_PLAYER(play);

    sRealPlayerFormEnRdGrab = player->transformation;

    if (shouldUseAdultFixes(player)) {
        player->transformation = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("EnRd_Grab")
void fixEnemyHeight_on_return_EnRd_Grab() {
    sPlayerEnRdGrab->transformation = sRealPlayerFormEnRdGrab;
}

// Music Box Gibdo height fix
static PlayerTransformation sRealPlayerFormIdealPosMusicBox;

RECOMP_HOOK("EnRailgibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_EnRailgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    sRealPlayerFormIdealPosMusicBox = GET_PLAYER_FORM;

    if (shouldUseAdultFixes(GET_PLAYER(play))) {
        gSaveContext.save.playerForm = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("EnRailgibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_return_EnRailgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    gSaveContext.save.playerForm = sRealPlayerFormIdealPosMusicBox;
}

// Talking Redead/Gibdo height fix
static PlayerTransformation sRealPlayerFormIdealPosTalk;

RECOMP_HOOK("EnTalkGibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_EnTalkgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    sRealPlayerFormIdealPosTalk = GET_PLAYER_FORM;

    if (shouldUseAdultFixes(GET_PLAYER(play))) {
        gSaveContext.save.playerForm = PLAYER_FORM_ZORA;
    }
}

RECOMP_HOOK_RETURN("EnTalkGibud_MoveToIdealGrabPositionAndRotation")
void fixEnemyHeight_on_return_EnTalkgibud_MoveToIdealGrabPositionAndRotation(EnRailgibud *this, PlayState *play) {
    gSaveContext.save.playerForm = sRealPlayerFormIdealPosTalk;
}

// Move adult models higher on Epona
#define EPONA_HEIGHT_OFFSET 1100.f
static Player *sPlayerEponaFix;

RECOMP_HOOK("Player_UpdateCommon")
void doEponaHeightOffset_on_Player_UpdateCommon(Player *this, PlayState *play, Input *input) {
    sPlayerEponaFix = this;
}

RECOMP_HOOK_RETURN("Player_UpdateCommon")
void doEponaHeightOffset_on_return_Player_UpdateCommon(void) {
    if (shouldUseAdultFixes(sPlayerEponaFix) && sPlayerEponaFix->stateFlags1 & PLAYER_STATE1_800000) {
        sPlayerEponaFix->actor.shape.yOffset -= EPONA_HEIGHT_OFFSET;
    }
}

extern Vec3f D_801C0D60; // Adult arrow hand position
extern Vec3f D_801C0D6C; // Child arrow hand position
static Vec3f sChildArrowOffset;

extern Vec3f D_801C0D98; // Bow string data
static Vec3f sChildBowStringData;
static Vec3f sAdultBowStringData = {0.0f, -360.4f, 0.0f}; // From OoT

static bool sIsAdultBowData = false;

RECOMP_HOOK("Player_Draw")
void fixBowProperties_on_Player_Draw(Actor *thisx, PlayState *play) {
    Player *player = (Player *)thisx;

    sIsAdultBowData = shouldUseAdultFixes(player);

    if (sIsAdultBowData) {
        sChildBowStringData = D_801C0D98;
        D_801C0D98 = sAdultBowStringData;

        sChildArrowOffset = D_801C0D6C;
        D_801C0D6C = D_801C0D60;
    }
}

RECOMP_HOOK_RETURN("Player_Draw")
void fixBowProperties_on_return_Player_Draw() {
    if (sIsAdultBowData) {
        D_801C0D6C = sChildArrowOffset;
        D_801C0D98 = sChildBowStringData;
    }
}
