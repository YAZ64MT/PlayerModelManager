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
#include "assets/objects/gameplay_keep/gameplay_keep.h"

static bool shouldUseAdultFixes(Player *player) {
    if (player->transformation != PLAYER_FORM_HUMAN) {
        return false;
    }

    FormProxy *fp = ProxyActorExt_getFormProxy(&player->actor);

    return fp && FormProxy_isAdultModelType(fp);
}

static Gfx **sFirstPersonOverrideDLPtr;
static Gfx *sFirstPersonDLToOverrideWith;

static void setFirstPersonPlayerDLOverride(Player *player, Link_DisplayList hookshotDLId, Link_DisplayList bowDLId) {
    FormProxy *fp = ProxyActorExt_getFormProxy(&player->actor);

    if (fp) {
        if (Player_IsHoldingHookshot(player)) {
            sFirstPersonDLToOverrideWith = FormProxy_getCurrentDL(fp, hookshotDLId);
        } else {
            sFirstPersonDLToOverrideWith = FormProxy_getCurrentDL(fp, bowDLId);
        }
    }
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson")
void addFPSrightForearm_on_Player_OverrideLimbDrawFirstPerson(PlayState *play, s32 limbIndex, Gfx **dList, Vec3f *pos, Vec3s *rot, Actor *actor) {
    sFirstPersonOverrideDLPtr = dList;
    sFirstPersonDLToOverrideWith = NULL;
    Player *player = (Player *)actor;

    if (player->unk_AA5 == PLAYER_UNKAA5_3) { // vanilla first person limb override doesn't draw unless this condition is met
        if (limbIndex == PLAYER_LIMB_LEFT_SHOULDER) {
            setFirstPersonPlayerDLOverride(player, LINK_DL_OPT_FPS_LSHOULDER_HOOKSHOT, LINK_DL_OPT_FPS_LSHOULDER_BOW);
        } else if (limbIndex == PLAYER_LIMB_RIGHT_FOREARM) {
            setFirstPersonPlayerDLOverride(player, LINK_DL_OPT_FPS_RFOREARM_HOOKSHOT, LINK_DL_OPT_FPS_RFOREARM_BOW);
        }
    }
}

RECOMP_HOOK_RETURN("Player_OverrideLimbDrawGameplayFirstPerson")
void addFPSrightForearm_on_return_Player_OverrideLimbDrawFirstPerson() {
    if (sFirstPersonDLToOverrideWith) {
        *sFirstPersonOverrideDLPtr = sFirstPersonDLToOverrideWith;
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

    for (int i = 0; i < ARRAY_COUNT(gAdultLinkAgeProps.unk_4A); ++i) {
        gAdultLinkAgeProps.unk_4A[i] = fdProps->unk_4A[i];
        gAdultLinkAgeProps.unk_62[i] = fdProps->unk_62[i];
        gAdultLinkAgeProps.unk_7A[i] = fdProps->unk_7A[i];
    }
    gAdultLinkAgeProps.voiceSfxIdOffset = SFX_VOICE_BANK_SIZE * 0;
    gAdultLinkAgeProps.surfaceSfxIdOffset = 0x80;
}

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
bool sWasDoorAnimChanged;

RECOMP_HOOK("Player_Door_Knob")
void replaceDoorAnim_on_Player_Door_Knob(PlayState *play, Player *this, Actor *door) {
    if (shouldUseAdultFixes(this)) {
        sWasDoorAnimChanged = true;
        for (int i = 0; i < PLAYER_ANIMTYPE_MAX; i++) {
            sDoorAAnimsTmp[i] = PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorA][i];
            sDoorBAnimsTmp[i] = PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorB][i];
            PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorA][i] = PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorA_free][i];
            PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorB][i] = PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorB_free][i];
        }
    }
}

RECOMP_HOOK_RETURN("Player_Door_Knob")
void replaceDoorAnim_on_return_Player_Door_Knob() {
    if (sWasDoorAnimChanged) {
        sWasDoorAnimChanged = false;
        for (int i = 0; i < PLAYER_ANIMTYPE_MAX; i++) {
            PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorA][i] = sDoorAAnimsTmp[i];
            PLAYER_ANIM_GROUPS[PLAYER_ANIMGROUP_doorB][i] = sDoorBAnimsTmp[i];
        }
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
void updateAdultProperties_on_Play_Main(GameState *thisx) {
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

extern Vec3f D_801C08C0[PLAYER_FORM_MAX];

static Vec3f sTmpAdjustSingleLegVec;

extern f32 D_801C08FC[PLAYER_FORM_MAX];
extern f32 D_801C0910[PLAYER_FORM_MAX];
extern f32 D_801C0924[PLAYER_FORM_MAX];
extern f32 D_801C0938[PLAYER_FORM_MAX];

typedef struct SingleLegFloat {
    f32 *vanillaArr;
    f32 tmp;
} SingleLegFloat;

SingleLegFloat sSingleLegFloatsTmp[] = {
    {.vanillaArr = D_801C08FC},
    {.vanillaArr = D_801C0910},
    {.vanillaArr = D_801C0924},
    {.vanillaArr = D_801C0938},
};

static bool sIsSingleLegAdjusted;

RECOMP_HOOK("Player_AdjustSingleLeg")
void adjustAdultLeg_on_Player_AdjustSingleLeg(PlayState *play, Player *player, SkelAnime *skelAnime, Vec3f *pos, Vec3s *rot,
                            s32 thighLimbIndex, s32 shinLimbIndex, s32 footLimbIndex) {
    sIsSingleLegAdjusted = shouldUseAdultFixes(player) && player->skelAnime.animation != &gPlayerAnim_cl_maskoff;

    if (sIsSingleLegAdjusted) {
        sTmpAdjustSingleLegVec = D_801C08C0[PLAYER_FORM_HUMAN];

        for (int i = 0; i < ARRAY_COUNT(sSingleLegFloatsTmp); ++i) {
            SingleLegFloat *curr = &sSingleLegFloatsTmp[i];

            curr->tmp = curr->vanillaArr[PLAYER_FORM_HUMAN];
            curr->vanillaArr[PLAYER_FORM_HUMAN] = curr->vanillaArr[PLAYER_FORM_FIERCE_DEITY];
        }
    }
}

RECOMP_HOOK_RETURN("Player_AdjustSingleLeg")
void adjustAdultLeg_on_return_Player_AdjustSingleLeg() {
    if (sIsSingleLegAdjusted) {
        D_801C08C0[PLAYER_FORM_HUMAN] = sTmpAdjustSingleLegVec;

        for (int i = 0; i < ARRAY_COUNT(sSingleLegFloatsTmp); ++i) {
            SingleLegFloat *curr = &sSingleLegFloatsTmp[i];

            curr->vanillaArr[PLAYER_FORM_HUMAN] = curr->tmp;
        }
    }
}
