#include "global.h"
#include "modelreplacements.h"
#include "modelreplacer_api.h"
#include "model_common.h"
#include "playermodelmanager.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "assets/objects/object_mir_ray/object_mir_ray.h"
#include "maskdls.h"

typedef struct {
    PlayerTransformation form;
    ModelReplacerHandle handle;
    ObjectId objId;
    Gfx *vanillaDL;
    Link_DisplayList linkDLId;
} ModelReplacement;

#define PLAYER_FORM_ANY PLAYER_FORM_MAX

#define DECLARE_REPLACEMENT(targetForm, objectId, vanillaDispList, linkDL) {.form = targetForm, .handle = 0, .vanillaDL = vanillaDispList, .objId = objectId, .linkDLId = LINK_DL_##linkDL}

static ModelReplacement sModelReplacements[] = {
    DECLARE_REPLACEMENT(PLAYER_FORM_ANY, GAMEPLAY_KEEP, gBottleGlassDL, BOTTLE_GLASS),
    DECLARE_REPLACEMENT(PLAYER_FORM_ANY, GAMEPLAY_KEEP, gBottleContentsDL, BOTTLE_CONTENTS),
    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, GAMEPLAY_KEEP, gDekuStickDL, DEKU_STICK),
    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, GAMEPLAY_KEEP, gameplay_keep_DL_013FF0, BOW_ARROW),
    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, GAMEPLAY_KEEP, gameplay_keep_DL_014370, BOW_ARROW),
    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, GAMEPLAY_KEEP, gHookshotChainDL, HOOKSHOT_CHAIN),
    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, GAMEPLAY_KEEP, gHookshotReticleDL, HOOKSHOT_RETICLE),

    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, OBJECT_LINK_CHILD, object_link_child_DL_017818, BOW_STRING),
    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, OBJECT_LINK_CHILD, object_link_child_DL_01D960, HOOKSHOT_HOOK),

    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, OBJECT_MIR_RAY, object_mir_ray_DL_0004B0, SHIELD3_RAY),

    DECLARE_REPLACEMENT(PLAYER_FORM_ZORA, OBJECT_LINK_ZORA, gLinkZoraLeftHandOpenDL, LHAND),
    DECLARE_REPLACEMENT(PLAYER_FORM_ZORA, OBJECT_LINK_ZORA, object_link_zora_DL_00E088, LHAND_GUITAR),
    DECLARE_REPLACEMENT(PLAYER_FORM_ZORA, OBJECT_LINK_ZORA, gLinkZoraRightHandOpenDL, RHAND),

    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_007390, PIPE_MOUTH),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_007548, PIPE_RIGHT),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_0076A0, PIPE_UP),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_0077D0, PIPE_DOWN),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_007900, PIPE_LEFT),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_007A28, PIPE_A),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_009AB8, PAD_WOOD),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_009C48, PAD_GRASS),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_009DB8, PAD_OPENING),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_008860, PETAL_PARTICLE),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, object_link_nuts_DL_00A348, DEKU_GUARD),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, gLinkDekuOpenFlowerDL, CENTER_FLOWER_PROPELLER_OPEN),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, OBJECT_LINK_NUTS, gLinkDekuClosedFlowerDL, CENTER_FLOWER_PROPELLER_CLOSED),

    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_00FC18, DRUM_STRAP),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_00FCF0, DRUM_UP),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_00FF18, DRUM_LEFT),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_010140, DRUM_RIGHT),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_010368, DRUM_DOWN),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_010590, DRUM_A),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, gLinkGoronCurledDL, CURLED),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_00C540, SPIKES),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_0127B0, FIRE_INIT),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_0134D0, FIRE_ROLL),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, gLinkGoronGoronPunchEffectDL, FIRE_PUNCH),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_017180, BODY_SHIELD_BODY),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_017620, BODY_SHIELD_HEAD),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, OBJECT_LINK_GORON, object_link_goron_DL_016A88, BODY_SHIELD_ARMS_AND_LEGS),

    DECLARE_REPLACEMENT(PLAYER_FORM_HUMAN, OBJECT_MASK_BAKURETU, object_mask_bakuretu_DL_000440, MASK_BLAST_COOLING_DOWN),

    DECLARE_REPLACEMENT(PLAYER_FORM_FIERCE_DEITY, GAMEPLAY_KEEP, gElegyShellHumanDL, ELEGY_OF_EMPTINESS_SHELL_HUMAN),
    DECLARE_REPLACEMENT(PLAYER_FORM_DEKU, GAMEPLAY_KEEP, gElegyShellDekuDL, ELEGY_OF_EMPTINESS_SHELL_DEKU),
    DECLARE_REPLACEMENT(PLAYER_FORM_GORON, GAMEPLAY_KEEP, gElegyShellGoronDL, ELEGY_OF_EMPTINESS_SHELL_GORON),
    DECLARE_REPLACEMENT(PLAYER_FORM_ZORA, GAMEPLAY_KEEP, gElegyShellZoraDL, ELEGY_OF_EMPTINESS_SHELL_ZORA),

    DECLARE_REPLACEMENT(PLAYER_FORM_ZORA, GAMEPLAY_KEEP, gameplay_keep_DL_06FE20, LFIN_BOOMERANG),
    DECLARE_REPLACEMENT(PLAYER_FORM_ZORA, GAMEPLAY_KEEP, gameplay_keep_DL_06FF68, RFIN_BOOMERANG),
};

#undef DECLARE_REPLACEMENT

MODEL_REPLACER_CALLBACK_ON_REGISTER_REPLACERS
void setupCodeReplacements() {
    for (int i = 0; i < ARRAY_COUNT(sModelReplacements); ++i) {
        ModelReplacement *curr = &sModelReplacements[i];
        curr->handle = ModelReplacer_registerReplacer(curr->objId, curr->vanillaDL, NULL);
    }
}

void refreshExternalDLs(Link_FormProxy *proxy, bool useOriginalForms) {
    if (!useOriginalForms) {
        for (int i = 0; i < ARRAY_COUNT(sModelReplacements); ++i) {
            ModelReplacement *curr = &sModelReplacements[i];
            ModelReplacer_setReplacerModel(curr->handle, &proxy->displayLists[curr->linkDLId]);
        }
    } else {
        for (int i = 0; i < ARRAY_COUNT(sModelReplacements); ++i) {
            ModelReplacement *curr = &sModelReplacements[i];

            Link_FormProxy *fp = proxy;

            if (curr->form < PLAYER_FORM_ANY) {
                fp = &gLinkFormProxies[curr->form];
            }

            ModelReplacer_setReplacerModel(curr->handle, &fp->displayLists[curr->linkDLId]);
        }
    }


}
