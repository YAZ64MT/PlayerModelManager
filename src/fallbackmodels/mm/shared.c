#include "global.h"
#include "modding.h"
#include "recompdata.h"
#include "globalobjects_api.h"
#include "modelreplacer_api.h"
#include "utils.h"
#include "assets/objects/object_link_boy/object_link_boy.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_mir_ray/object_mir_ray.h"
#include "maskdls.h"
#include "modelentrymanager.h"
#include "modelentry.h"
#include "customdls.h"

ModelEntryForm *gSharedModelEntry;

Mtx *getHumanMtx(Mtx *mtx) {
    return (Mtx *)((uintptr_t)GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD) + SEGMENT_OFFSET(mtx));
}

Gfx *getGameplayKeepDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, dl);
}

static void setupSharedFallbackModel() {
    ModelEntry *modelEntry = ModelEntryManager_getEntry(ModelEntryManager_createMemoryHandle(PMM_MODEL_TYPE_CHILD, "__mm_object_link_shared__"));

    ModelEntryManager_setEntryHidden(modelEntry, true);

    gSharedModelEntry = (ModelEntryForm *)modelEntry;

    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK, getHumanMtx(&gLinkHumanSheathedKokiriSwordMtx));
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK, getHumanMtx(&gLinkHumanSheathedRazorSwordMtx));
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SWORD_GILDED_BACK, getHumanMtx(&gLinkHumanSheathedKokiriSwordMtx));
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SHIELD_HERO_BACK, getHumanMtx(&gLinkHumanHerosShieldMtx));
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK, getHumanMtx(&gLinkHumanMirrorShieldMtx));
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK, getHumanMtx(&gLinkHumanMirrorShieldMtx));
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SWORD3_PEDESTAL, &gIdentityMtx);
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_SWORD3_PEDESTAL_GRABBED, &gIdentityMtx);
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_ARROW_DRAWN, &gIdentityMtx);
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK, &gIdentityMtx);
    ModelEntry_setMatrix(modelEntry, LINK_EQUIP_MATRIX_MASKS, &gIdentityMtx);

    ModelEntry_setDisplayList(modelEntry, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_FIERCE_DEITY, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gElegyShellHumanDL));
    ModelEntry_setDisplayList(modelEntry, LINK_DL_FPS_HOOKSHOT, gLinkHumanFirstPersonHookshotDL);
    ModelEntry_setDisplayList(modelEntry, LINK_DL_FPS_BOW, gLinkHumanFirstPersonBowDL);
    ModelEntry_setDisplayList(modelEntry, LINK_DL_OPT_FPS_LSHOULDER, gEmptyDL);
    ModelEntry_setDisplayList(modelEntry, LINK_DL_OPT_FPS_LHAND_SLINGSHOT, gEmptyDL);
    ModelEntry_setDisplayList(modelEntry, LINK_DL_OPT_FPS_LFOREARM_SLINGSHOT, gEmptyDL);
    ModelEntry_setDisplayList(modelEntry, LINK_DL_SWORD_FIERCE_DEITY_HILT, gEmptyDL);
    ModelEntry_setDisplayList(modelEntry, LINK_DL_SWORD_GREAT_FAIRY_HILT, gEmptyDL);

    ModelEntry_setDisplayList(modelEntry, LINK_DL_BOMB_BODY_2D, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gBombBodyDL));
    ModelEntry_setDisplayList(modelEntry, LINK_DL_BOMB_BODY_3D, gEmptyDL);
    ModelEntry_setDisplayList(modelEntry, LINK_DL_BOMB_CAP, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gBombCapDL));
    ModelEntry_setDisplayList(modelEntry, LINK_DL_BOMBCHU, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gBombchuDL));
}

static void setupDL(ObjectId id, Gfx *vanillaDL, Link_DisplayList linkDLId) {
    ModelEntry_setDisplayList(ModelEntryForm_getModelEntry(gSharedModelEntry), linkDLId, GlobalObjects_getGlobalGfxPtr(id, vanillaDL));
}

static void initSharedDLs() {
    setupSharedFallbackModel();

    // Gameplay Keep
    setupDL(GAMEPLAY_KEEP, gKokiriSwordHandleDL, LINK_DL_SWORD_KOKIRI_HILT);
    setupDL(GAMEPLAY_KEEP, gRazorSwordHandleDL, LINK_DL_SWORD_RAZOR_HILT);
    setupDL(GAMEPLAY_KEEP, gKokiriSwordBladeDL, LINK_DL_SWORD_KOKIRI_BLADE);
    setupDL(GAMEPLAY_KEEP, gRazorSwordBladeDL, LINK_DL_SWORD_RAZOR_BLADE);
    setupDL(GAMEPLAY_KEEP, gDekuStickDL, LINK_DL_DEKU_STICK);
    setupDL(GAMEPLAY_KEEP, gameplay_keep_DL_013FF0, LINK_DL_BOW_ARROW);
    setupDL(GAMEPLAY_KEEP, gHookshotChainDL, LINK_DL_HOOKSHOT_CHAIN);
    setupDL(GAMEPLAY_KEEP, gHookshotReticleDL, LINK_DL_HOOKSHOT_RETICLE);
    setupDL(GAMEPLAY_KEEP, gBottleContentsDL, LINK_DL_BOTTLE_CONTENTS);
    setupDL(GAMEPLAY_KEEP, gBottleGlassDL, LINK_DL_BOTTLE_GLASS);
    setupDL(GAMEPLAY_KEEP, gElegyShellHumanDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_HUMAN);
    setupDL(GAMEPLAY_KEEP, gElegyShellDekuDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU);
    setupDL(GAMEPLAY_KEEP, gElegyShellGoronDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_GORON);
    setupDL(GAMEPLAY_KEEP, gElegyShellZoraDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA);
    setupDL(GAMEPLAY_KEEP, gDekuMaskDL, LINK_DL_MASK_DEKU);
    setupDL(GAMEPLAY_KEEP, gGoronMaskDL, LINK_DL_MASK_GORON);
    setupDL(GAMEPLAY_KEEP, gZoraMaskDL, LINK_DL_MASK_ZORA);
    setupDL(GAMEPLAY_KEEP, gFierceDeityMaskDL, LINK_DL_MASK_FIERCE_DEITY);
    setupDL(GAMEPLAY_KEEP, gameplay_keep_DL_06FE20, LINK_DL_LFIN_BOOMERANG);
    setupDL(GAMEPLAY_KEEP, gameplay_keep_DL_06FF68, LINK_DL_RFIN_BOOMERANG);

    // Human
    setupDL(OBJECT_LINK_CHILD, gLinkHumanKokiriSwordSheathDL, LINK_DL_SWORD_KOKIRI_SHEATH);
    setupDL(OBJECT_LINK_CHILD, gLinkHumanRazorSwordSheathDL, LINK_DL_SWORD_RAZOR_SHEATH);
    setupDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordSheathDL, LINK_DL_SWORD_GILDED_SHEATH);

    setupDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordHandleDL, LINK_DL_SWORD_GILDED_HILT);
    setupDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordBladeDL, LINK_DL_SWORD_GILDED_BLADE);

    setupDL(OBJECT_LINK_CHILD, gLinkHumanHerosShieldDL, LINK_DL_SHIELD_HERO);
    setupDL(OBJECT_LINK_CHILD, gLinkHumanMirrorShieldDL, LINK_DL_SHIELD_MIRROR);
    setupDL(OBJECT_MIR_RAY, object_mir_ray_DL_0004B0, LINK_DL_SHIELD_MIRROR_RAY);
    setupDL(OBJECT_MIR_RAY, object_mir_ray_DL_000168, LINK_DL_SHIELD_MIRROR_RAY_BEAM);

    setupDL(OBJECT_LINK_CHILD, gLinkHumanBowDL, LINK_DL_BOW);
    setupDL(OBJECT_LINK_CHILD, object_link_child_DL_017818, LINK_DL_BOW_STRING);
    setupDL(OBJECT_LINK_CHILD, gLinkHumanHookshotDL, LINK_DL_HOOKSHOT);
    setupDL(OBJECT_LINK_CHILD, object_link_child_DL_01D960, LINK_DL_HOOKSHOT_HOOK);

    // not in Link obj
    //models[LINK_DL_FPS_HOOKSHOT] = gLinkHumanFirstPersonHookshotDL;
    //models[LINK_DL_FPS_BOW] = gLinkHumanFirstPersonBowDL;

    setupDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL, LINK_DL_SWORD_GREAT_FAIRY_BLADE);
    // models[LINK_DL_SWORD_GREAT_FAIRY_HILT] = gEmptyDL;

    setupDL(OBJECT_MASK_TRUTH, object_mask_truth_DL_0001A0, LINK_DL_MASK_TRUTH);
    setupDL(OBJECT_MASK_KERFAY, gKafeisMaskDL, LINK_DL_MASK_KAFEIS_MASK);
    setupDL(OBJECT_MASK_YOFUKASI, object_mask_yofukasi_DL_000490, LINK_DL_MASK_ALL_NIGHT);
    setupDL(OBJECT_MASK_RABIT, object_mask_rabit_DL_000610, LINK_DL_MASK_BUNNY);
    setupDL(OBJECT_MASK_KI_TAN, object_mask_ki_tan_DL_0004A0, LINK_DL_MASK_KEATON);
    setupDL(OBJECT_MASK_JSON, object_mask_json_DL_0004C0, LINK_DL_MASK_GARO);
    setupDL(OBJECT_MASK_ROMERNY, object_mask_romerny_DL_0007A0, LINK_DL_MASK_ROMANI);
    setupDL(OBJECT_MASK_ZACHO, object_mask_zacho_DL_000700, LINK_DL_MASK_CIRCUS_LEADER);
    setupDL(OBJECT_MASK_POSTHAT, object_mask_posthat_DL_000290, LINK_DL_MASK_POSTMAN);
    setupDL(OBJECT_MASK_MEOTO, object_mask_meoto_DL_0005A0, LINK_DL_MASK_COUPLE);
    setupDL(OBJECT_MASK_BIGELF, object_mask_bigelf_DL_0016F0, LINK_DL_MASK_GREAT_FAIRY);
    setupDL(OBJECT_MASK_GIBUDO, object_mask_gibudo_DL_000250, LINK_DL_MASK_GIBDO);
    setupDL(OBJECT_MASK_GERO, gDonGeroMaskDL, LINK_DL_MASK_DON_GERO);
    setupDL(OBJECT_MASK_DANCER, object_mask_dancer_DL_000EF0, LINK_DL_MASK_KAMARO);
    setupDL(OBJECT_MASK_SKJ, object_mask_skj_DL_0009F0, LINK_DL_MASK_CAPTAIN);
    setupDL(OBJECT_MASK_STONE, object_mask_stone_DL_000820, LINK_DL_MASK_STONE);
    setupDL(OBJECT_MASK_BREE, object_mask_bree_DL_0003C0, LINK_DL_MASK_BREMEN);
    setupDL(OBJECT_MASK_BAKURETU, object_mask_bakuretu_DL_0005C0, LINK_DL_MASK_BLAST);
    setupDL(OBJECT_MASK_BAKURETU, object_mask_bakuretu_DL_000440, LINK_DL_MASK_BLAST_COOLING_DOWN);
    setupDL(OBJECT_MASK_BU_SAN, object_mask_bu_san_DL_000710, LINK_DL_MASK_SCENTS);
    setupDL(OBJECT_MASK_KYOJIN, object_mask_kyojin_DL_000380, LINK_DL_MASK_GIANT);
    setupDL(OBJECT_MASK_BOY, object_mask_boy_DL_000900, LINK_DL_MASK_FIERCE_DEITY_SCREAM);
    setupDL(OBJECT_MASK_GORON, object_mask_goron_DL_0014A0, LINK_DL_MASK_GORON_SCREAM);
    setupDL(OBJECT_MASK_ZORA, object_mask_zora_DL_000DB0, LINK_DL_MASK_ZORA_SCREAM);
    setupDL(OBJECT_MASK_NUTS, object_mask_nuts_DL_001D90, LINK_DL_MASK_DEKU_SCREAM);

    // Zora DLs
    setupDL(OBJECT_LINK_ZORA, object_link_zora_DL_00CC38, LINK_DL_LFIN);
    setupDL(OBJECT_LINK_ZORA, object_link_zora_DL_010868, LINK_DL_LFIN_SWIM);
    setupDL(OBJECT_LINK_ZORA, object_link_zora_DL_00CDA0, LINK_DL_RFIN);
    setupDL(OBJECT_LINK_ZORA, object_link_zora_DL_010978, LINK_DL_RFIN_SWIM);
    setupDL(OBJECT_LINK_ZORA, object_link_zora_DL_00E2A0, LINK_DL_GUITAR);
    setupDL(OBJECT_LINK_ZORA, object_link_zora_DL_0110A8, LINK_DL_FIN_SHIELD);
    setupDL(OBJECT_LINK_ZORA, object_link_zora_DL_011760, LINK_DL_MAGIC_BARRIER);

    // Fierce Deity DLs
    setupDL(OBJECT_LINK_BOY, gLinkFierceDeitySwordDL, LINK_DL_SWORD_FIERCE_DEITY_BLADE);
    // models[LINK_DL_SWORD_FIERCE_DEITY_HILT] = gEmptyDL;

    // Deku DLs
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007390, LINK_DL_PIPE_MOUTH);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007548, LINK_DL_PIPE_RIGHT);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0076A0, LINK_DL_PIPE_UP);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0077D0, LINK_DL_PIPE_DOWN);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007900, LINK_DL_PIPE_LEFT);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007A28, LINK_DL_PIPE_A);

    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008660, LINK_DL_STEM_RIGHT);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008760, LINK_DL_STEM_LEFT);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0089F0, LINK_DL_FLOWER_CENTER_CLOSED);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008BA0, LINK_DL_FLOWER_CENTER_OPEN);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008AB8, LINK_DL_FLOWER_PROPELLER_OPEN);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008908, LINK_DL_FLOWER_PROPELLER_CLOSED);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008860, LINK_DL_PETAL_PARTICLE);

    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_009AB8, LINK_DL_PAD_WOOD);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_009C48, LINK_DL_PAD_GRASS);
    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_009DB8, LINK_DL_PAD_OPENING);

    setupDL(OBJECT_LINK_NUTS, object_link_nuts_DL_00A348, LINK_DL_DEKU_GUARD);

    // Goron Link
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_00FC18, LINK_DL_DRUM_STRAP);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_00FCF0, LINK_DL_DRUM_UP);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_00FF18, LINK_DL_DRUM_LEFT);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_010140, LINK_DL_DRUM_RIGHT);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_010368, LINK_DL_DRUM_DOWN);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_010590, LINK_DL_DRUM_A);

    setupDL(OBJECT_LINK_GORON, gLinkGoronCurledDL, LINK_DL_CURLED);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_00C540, LINK_DL_SPIKES);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_0127B0, LINK_DL_FIRE_INIT);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_0134D0, LINK_DL_FIRE_ROLL);
    setupDL(OBJECT_LINK_GORON, gLinkGoronGoronPunchEffectDL, LINK_DL_FIRE_PUNCH);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_017180, LINK_DL_BODY_SHIELD_BODY);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_017620, LINK_DL_BODY_SHIELD_HEAD);
    setupDL(OBJECT_LINK_GORON, object_link_goron_DL_016A88, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
}

RECOMP_CALLBACK(".", _internal_setupVanillaModels)
void initSharedDLs_on_event() {
    initSharedDLs();
}
