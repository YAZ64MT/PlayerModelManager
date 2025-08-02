#include "global.h"
#include "modding.h"
#include "recompdata.h"
#include "recomputils.h"
#include "playermodelmanager.h"
#include "model_common.h"
#include "externs_z_player_lib.h"
#include "maskdls.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/object_link_boy/object_link_boy.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "assets/objects/object_link_zora/object_link_zora.h"

typedef struct {
    Gfx *target;
    Link_DisplayList replacementId;
} GfxHookDisplayList;

#define DECLARE_GFX_HOOK_DL(targetDL, replacementDLId) {.target = targetDL, .replacementId = replacementDLId}

typedef struct {
    U32ValueHashmapHandle gfxPtrsToDLs;
    GfxHookDisplayList *rawHookDLEntries;
    size_t numHookDLEntries;
} GfxHookLookup;

#define DECLARE_GFX_HOOK_LUT(hookDLs)             \
    {                                             \
        .gfxPtrsToDLs = 0,                        \
        .rawHookDLEntries = hookDLs,              \
        .numHookDLEntries = ARRAY_COUNT(hookDLs), \
    }

typedef struct {
    PlayState *play;
    Link_FormProxy *formProxy;
    Gfx *startOpa;
    Gfx *startXlu;
} GfxHookData;

U32ValueHashmapHandle initReplacementList(GfxHookDisplayList replacements[], int numReplacements) {
    U32ValueHashmapHandle h = recomputil_create_u32_value_hashmap();

    for (int i = 0; i < numReplacements; ++i) {
        recomputil_u32_value_hashmap_insert(h, (uintptr_t)(replacements->target), replacements->replacementId);
    }

    return h;
}

// By coincidence, none of these DLs have overlapping segmented addresses
// Any new entries should be checked to ensure that their segmented addresses do not overlap
static GfxHookDisplayList sLinkFormsDLReplacements[] = {
    DECLARE_GFX_HOOK_DL(object_link_child_DL_017818, LINK_DL_BOW_STRING),
    DECLARE_GFX_HOOK_DL(object_link_child_DL_01D960, LINK_DL_HOOKSHOT_HOOK),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_007390, LINK_DL_PIPE_MOUTH),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_007548, LINK_DL_PIPE_RIGHT),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_0076A0, LINK_DL_PIPE_UP),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_0077D0, LINK_DL_PIPE_DOWN),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_007900, LINK_DL_PIPE_LEFT),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_007A28, LINK_DL_PIPE_A),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_009AB8, LINK_DL_PAD_WOOD),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_009C48, LINK_DL_PAD_GRASS),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_009DB8, LINK_DL_PAD_OPENING),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_008860, LINK_DL_PETAL_PARTICLE),
    DECLARE_GFX_HOOK_DL(object_link_nuts_DL_00A348, LINK_DL_DEKU_GUARD),
    DECLARE_GFX_HOOK_DL(gLinkDekuOpenFlowerDL, LINK_DL_FLOWER_PROPELLER_OPEN),
    DECLARE_GFX_HOOK_DL(gLinkDekuClosedFlowerDL, LINK_DL_FLOWER_PROPELLER_CLOSED),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_00FC18, LINK_DL_DRUM_STRAP),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_00FCF0, LINK_DL_DRUM_UP),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_00FF18, LINK_DL_DRUM_LEFT),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_010140, LINK_DL_DRUM_RIGHT),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_010368, LINK_DL_DRUM_DOWN),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_010590, LINK_DL_DRUM_A),
    DECLARE_GFX_HOOK_DL(gLinkGoronCurledDL, LINK_DL_CURLED),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_00C540, LINK_DL_SPIKES),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_0127B0, LINK_DL_FIRE_INIT),
    DECLARE_GFX_HOOK_DL(object_link_goron_DL_0134D0, LINK_DL_FIRE_ROLL),
    DECLARE_GFX_HOOK_DL(gLinkGoronGoronPunchEffectDL, LINK_DL_FIRE_PUNCH),
    DECLARE_GFX_HOOK_DL(gLinkZoraLeftHandOpenDL, LINK_DL_LHAND),
    DECLARE_GFX_HOOK_DL(gLinkZoraRightHandOpenDL, LINK_DL_RHAND),
    DECLARE_GFX_HOOK_DL(object_link_zora_DL_00E088, LINK_DL_LHAND_GUITAR),
    DECLARE_GFX_HOOK_DL(object_link_zora_DL_0110A8, LINK_DL_FIN_SHIELD),
};
static GfxHookLookup sLinkFormsDLMap = DECLARE_GFX_HOOK_LUT(sLinkFormsDLReplacements);

static GfxHookDisplayList sGameplayKeepDLReplacements[] = {
    DECLARE_GFX_HOOK_DL(gDekuStickDL, LINK_DL_DEKU_STICK),
    DECLARE_GFX_HOOK_DL(gBottleContentsDL, LINK_DL_BOTTLE_CONTENTS),
    DECLARE_GFX_HOOK_DL(gBottleGlassDL, LINK_DL_BOTTLE_GLASS),
    DECLARE_GFX_HOOK_DL(gameplay_keep_DL_013FF0, LINK_DL_BOW_ARROW),
    DECLARE_GFX_HOOK_DL(gameplay_keep_DL_014370, LINK_DL_BOW_ARROW),
    DECLARE_GFX_HOOK_DL(gHookshotChainDL, LINK_DL_HOOKSHOT_CHAIN),
    DECLARE_GFX_HOOK_DL(gHookshotReticleDL, LINK_DL_HOOKSHOT_RETICLE),
    DECLARE_GFX_HOOK_DL(gElegyShellHumanDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_HUMAN),
    DECLARE_GFX_HOOK_DL(gElegyShellDekuDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU),
    DECLARE_GFX_HOOK_DL(gElegyShellGoronDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_GORON),
    DECLARE_GFX_HOOK_DL(gElegyShellZoraDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA),
};
static GfxHookLookup sGameplayKeepDLMap = DECLARE_GFX_HOOK_LUT(sGameplayKeepDLReplacements);

static GfxHookDisplayList sMaskDLReplacements[] = {
    DECLARE_GFX_HOOK_DL(object_mask_bakuretu_DL_000440, LINK_DL_MASK_BLAST_COOLING_DOWN),
};
static GfxHookLookup sMaskDLMap = DECLARE_GFX_HOOK_LUT(sMaskDLReplacements);

void GfxHookLookup_init(GfxHookLookup *ghl) {
    ghl->gfxPtrsToDLs = recomputil_create_u32_value_hashmap();
    for (size_t i = 0; i < ghl->numHookDLEntries; ++i) {
        GfxHookDisplayList *currHookDL = &ghl->rawHookDLEntries[i];
        if (!recomputil_u32_value_hashmap_insert(ghl->gfxPtrsToDLs, (uintptr_t)(currHookDL->target), currHookDL->replacementId)) {
            recomp_printf("PlayerModelManager: WARNING! GfxHookLookup_init: Passed in GfxHookLookup contains duplicate keys!\n");
        }
    }
}

RECOMP_CALLBACK(".", _internal_initHashObjects)
void initGfxHookReplacmentMaps() {
    GfxHookLookup_init(&sGameplayKeepDLMap);
    GfxHookLookup_init(&sLinkFormsDLMap);
    GfxHookLookup_init(&sMaskDLMap);
}

// assumed hookdat has valid playstate ptr
void fillGfxHookData(GfxHookData *hookDat, PlayState *play, Link_FormProxy *formProxy) {
    hookDat->play = play;
    hookDat->formProxy = formProxy;

    OPEN_DISPS(play->state.gfxCtx);

    hookDat->startOpa = POLY_OPA_DISP;
    hookDat->startXlu = POLY_XLU_DISP;

    CLOSE_DISPS(play->state.gfxCtx);
}

void replaceHookedGfxCommands(GfxHookData *hookDat, Gfx *startDL, Gfx *endDL) {
    Gfx *curr = startDL;

    while (curr <= endDL) {
        // Check if this command is segmented gSPDisplayList command
        if ((curr->words.w0 >> 16) == (G_DL << 8 | G_DL_PUSH) && curr->words.w1 < K0BASE) {
            U32ValueHashmapHandle hashmap = 0;

            switch (SEGMENT_NUMBER(curr->words.w1)) {
                case 0x04:
                    hashmap = sGameplayKeepDLMap.gfxPtrsToDLs;
                    break;

                case 0x06:
                    hashmap = sLinkFormsDLMap.gfxPtrsToDLs;
                    break;

                case 0x0A:
                    hashmap = sMaskDLMap.gfxPtrsToDLs;
                    break;

                default:
                    break;
            }

            if (hashmap) {
                unsigned long tmp = curr->words.w1;
                if (recomputil_u32_value_hashmap_get(hashmap, curr->words.w1, &tmp)) {
                    gSPDisplayList(curr, &hookDat->formProxy->displayLists[tmp]);
                }
            }
        }

        curr++;
    }
}

void replaceHookedOpaGfxCommands(GfxHookData *hookDat) {
    OPEN_DISPS(hookDat->play->state.gfxCtx);
    replaceHookedGfxCommands(hookDat, hookDat->startOpa, POLY_OPA_DISP);
    CLOSE_DISPS(hookDat->play.state.gfxCtx);
}

void replaceHookedXluGfxCommands(GfxHookData *hookDat) {
    OPEN_DISPS(hookDat->play->state.gfxCtx);
    replaceHookedGfxCommands(hookDat, hookDat->startXlu, POLY_XLU_DISP);
    CLOSE_DISPS(hookDat->play.state.gfxCtx);
}

static GfxHookData sPlayerDrawGfxHook;

RECOMP_HOOK("Player_Draw")
void hookGfx_on_Player_Draw(Actor *thisx, PlayState *play) {
    fillGfxHookData(&sPlayerDrawGfxHook, play, &gLinkFormProxies[((Player *)thisx)->transformation]);
}

RECOMP_HOOK_RETURN("Player_Draw")
void hookGfx_on_return_Player_Draw() {
    replaceHookedOpaGfxCommands(&sPlayerDrawGfxHook);
    replaceHookedXluGfxCommands(&sPlayerDrawGfxHook);
}

static GfxHookData sBoomerangDrawGfxHook;

RECOMP_HOOK("EnBoom_Draw")
void hookGfx_on_EnBoom_Draw(Actor *thisx, PlayState *play) {
    fillGfxHookData(&sBoomerangDrawGfxHook, play, &gLinkFormProxies[PLAYER_FORM_ZORA]);
}

RECOMP_HOOK_RETURN("EnBoom_Draw")
void hookGfx_on_return_EnBoom_Draw() {
    replaceHookedOpaGfxCommands(&sBoomerangDrawGfxHook);
    replaceHookedXluGfxCommands(&sBoomerangDrawGfxHook);
}

static GfxHookData sElegyShellDrawGfxHook;

RECOMP_HOOK("EnTorch2_Draw")
void hookGfx_on_EnTorch2_Draw(Actor *thisx, PlayState *play) {
    fillGfxHookData(&sElegyShellDrawGfxHook, play, &gLinkFormProxies[thisx->params]);
}

RECOMP_HOOK_RETURN("EnTorch2_Draw")
void hookGfx_on_return_EnTorch2_Draw() {
    replaceHookedOpaGfxCommands(&sElegyShellDrawGfxHook);
    replaceHookedXluGfxCommands(&sElegyShellDrawGfxHook);
}

static GfxHookData sHookshotGfxHook;

RECOMP_HOOK("ArmsHook_Draw")
void hookGfx_on_ArmsHook_Draw(Actor *thisx, PlayState *play) {
    fillGfxHookData(&sHookshotGfxHook, play, &gLinkFormProxies[GET_PLAYER(play)->transformation]);
}

RECOMP_HOOK_RETURN("ArmsHook_Draw")
void hookGfx_on_return_ArmsHook_Draw() {
    replaceHookedOpaGfxCommands(&sHookshotGfxHook);
    replaceHookedXluGfxCommands(&sHookshotGfxHook);
}
