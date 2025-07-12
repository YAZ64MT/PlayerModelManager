#include "modding.h"
#include "global.h"
#include "recompdata.h"
#include "globalobjects_api.h"
#include "modelreplacer_api.h"
#include "playermodelmanager.h"
#include "playermodelmanager_mm.h"
#include "playermodelmanager_utils.h"
#include "assets/objects/object_link_boy/object_link_boy.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_mir_ray/object_mir_ray.h"
#include "maskdls.h"
#include "modelreplacer_compat.h"
#include "model_common.h"

// handless first person hookshot
Gfx gLinkHumanFirstPersonHookshotDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal256(object_link_child_TLUT_018A40),
    gsDPLoadTextureBlock(object_link_child_Tex_01B380, G_IM_FMT_CI, G_IM_SIZ_8b, 16, 16, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&object_link_childVtx_010538[452], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(2, 4, 5, 0, 2, 5, 0, 0),
    gsSP2Triangles(6, 3, 0, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 9, 11, 0, 10, 7, 9, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 12, 14, 0),
    gsSP2Triangles(15, 16, 12, 0, 17, 18, 19, 0),
    gsSP2Triangles(10, 17, 19, 0, 10, 20, 17, 0),
    gsSP2Triangles(21, 22, 23, 0, 24, 23, 25, 0),
    gsSP2Triangles(26, 27, 28, 0, 29, 28, 30, 0),
    gsSPVertex(&object_link_childVtx_010538[483], 31, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(3, 4, 0, 0, 5, 6, 7, 0),
    gsSP2Triangles(5, 7, 2, 0, 8, 5, 2, 0),
    gsSP2Triangles(9, 10, 11, 0, 12, 9, 11, 0),
    gsSP2Triangles(12, 13, 9, 0, 14, 15, 16, 0),
    gsSP2Triangles(17, 18, 19, 0, 14, 17, 19, 0),
    gsSP2Triangles(19, 20, 14, 0, 20, 15, 14, 0),
    gsSP2Triangles(21, 22, 23, 0, 24, 21, 25, 0),
    gsSP2Triangles(26, 27, 28, 0, 29, 26, 30, 0),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal256(object_link_child_TLUT_018A40),
    gsDPLoadTextureBlock(object_link_child_Tex_01A280, G_IM_FMT_CI, G_IM_SIZ_8b, 16, 16, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&object_link_childVtx_010538[514], 18, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
    gsSP2Triangles(4, 5, 3, 0, 6, 0, 2, 0),
    gsSP2Triangles(3, 0, 4, 0, 2, 7, 6, 0),
    gsSP2Triangles(8, 9, 10, 0, 10, 9, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 13, 12, 0),
    gsSP2Triangles(4, 16, 17, 0, 17, 5, 4, 0),
    gsSP2Triangles(6, 17, 16, 0, 6, 7, 17, 0),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal256(object_link_child_TLUT_018A40),
    gsDPLoadTextureBlock(object_link_child_Tex_019A80, G_IM_FMT_CI, G_IM_SIZ_8b, 32, 64, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&object_link_childVtx_010538[532], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(4, 5, 6, 0, 6, 5, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 12, 0, 0),
    gsSP2Triangles(13, 14, 15, 0, 15, 8, 10, 0),
    gsSP2Triangles(0, 13, 11, 0, 4, 16, 17, 0),
    gsSP2Triangles(10, 13, 15, 0, 18, 19, 20, 0),
    gsSP2Triangles(13, 10, 11, 0, 7, 21, 17, 0),
    gsSP2Triangles(17, 20, 19, 0, 22, 23, 24, 0),
    gsSP2Triangles(17, 16, 7, 0, 25, 22, 16, 0),
    gsSP2Triangles(25, 26, 22, 0, 24, 16, 22, 0),
    gsSP2Triangles(27, 21, 28, 0, 24, 7, 16, 0),
    gsSP2Triangles(6, 26, 25, 0, 24, 23, 6, 0),
    gsSP2Triangles(6, 7, 24, 0, 23, 29, 6, 0),
    gsSP2Triangles(25, 4, 6, 0, 6, 29, 26, 0),
    gsSP1Triangle(30, 27, 31, 0),
    gsSPVertex(&object_link_childVtx_010538[564], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 8, 6, 5, 0),
    gsSP2Triangles(8, 9, 6, 0, 0, 2, 4, 0),
    gsSP2Triangles(10, 11, 12, 0, 0, 13, 1, 0),
    gsSP2Triangles(0, 14, 13, 0, 15, 4, 16, 0),
    gsSP2Triangles(17, 18, 3, 0, 4, 15, 0, 0),
    gsSP2Triangles(18, 19, 3, 0, 16, 4, 20, 0),
    gsSP2Triangles(3, 21, 22, 0, 15, 11, 23, 0),
    gsSP2Triangles(16, 11, 15, 0, 22, 20, 3, 0),
    gsSP2Triangles(20, 22, 23, 0, 20, 24, 16, 0),
    gsSP2Triangles(25, 26, 27, 0, 22, 28, 0, 0),
    gsSP1Triangle(29, 30, 31, 0),
    gsSPVertex(&object_link_childVtx_010538[596], 11, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 0, 4, 0, 4, 6, 7, 0),
    gsSP2Triangles(4, 2, 6, 0, 8, 4, 7, 0),
    gsSP2Triangles(8, 5, 4, 0, 7, 9, 10, 0),
    gsSP1Triangle(7, 6, 9, 0),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal256(object_link_child_TLUT_018A40),
    gsSPEndDisplayList(),
};

Gfx *gSharedDisplayLists[LINK_DL_MAX];

Mtx *gSharedMatrixes[LINK_EQUIP_MATRIX_MAX];

Mtx *getHumanMtx(Mtx *mtx) {
    return (Mtx *)((uintptr_t)GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD) + SEGMENT_OFFSET(mtx));
}

Gfx *getGameplayKeepDL(Gfx *dl) {
    return GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, dl);
}

void initSharedDLs() {
    Mtx **equipMatrixes = gSharedMatrixes;
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK] = getHumanMtx(&gLinkHumanSheathedKokiriSwordMtx);
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK] = getHumanMtx(&gLinkHumanSheathedRazorSwordMtx);
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD_GILDED_BACK] = getHumanMtx(&gLinkHumanSheathedKokiriSwordMtx);
    equipMatrixes[LINK_EQUIP_MATRIX_SHIELD_HERO_BACK] = getHumanMtx(&gLinkHumanHerosShieldMtx);
    equipMatrixes[LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK] = getHumanMtx(&gLinkHumanMirrorShieldMtx);
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL] = &gIdentityMtx;
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD3_PEDESTAL_GRABBED] = &gIdentityMtx;
    equipMatrixes[LINK_EQUIP_MATRIX_ARROW_DRAWN] = &gIdentityMtx;
    equipMatrixes[LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK] = &gIdentityMtx;

    Gfx **models = gSharedDisplayLists;

    // Gameplay Keep
    setupSharedListenerDL(GAMEPLAY_KEEP, gKokiriSwordBladeDL, LINK_DL_SWORD_KOKIRI_HILT);
    setupSharedListenerDL(GAMEPLAY_KEEP, gRazorSwordHandleDL, LINK_DL_SWORD_KOKIRI_HILT);
    setupSharedListenerDL(GAMEPLAY_KEEP, gKokiriSwordBladeDL, LINK_DL_SWORD_KOKIRI_BLADE);
    setupSharedListenerDL(GAMEPLAY_KEEP, gRazorSwordBladeDL, LINK_DL_SWORD_RAZOR_BLADE);
    setupSharedListenerDL(GAMEPLAY_KEEP, gDekuStickDL, LINK_DL_DEKU_STICK);
    setupSharedListenerDL(GAMEPLAY_KEEP, gameplay_keep_DL_013FF0, LINK_DL_BOW_ARROW);
    setupSharedListenerDL(GAMEPLAY_KEEP, gHookshotChainDL, LINK_DL_HOOKSHOT_CHAIN);
    setupSharedListenerDL(GAMEPLAY_KEEP, gHookshotReticleDL, LINK_DL_HOOKSHOT_RETICLE);
    setupSharedListenerDL(GAMEPLAY_KEEP, gBottleContentsDL, LINK_DL_BOTTLE_CONTENTS);
    setupSharedListenerDL(GAMEPLAY_KEEP, gBottleGlassDL, LINK_DL_BOTTLE_GLASS);
    setupSharedListenerDL(GAMEPLAY_KEEP, gElegyShellHumanDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_HUMAN);
    setupSharedListenerDL(GAMEPLAY_KEEP, gElegyShellDekuDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU);
    setupSharedListenerDL(GAMEPLAY_KEEP, gElegyShellGoronDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_GORON);
    setupSharedListenerDL(GAMEPLAY_KEEP, gElegyShellZoraDL, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA);
    setupSharedListenerDL(GAMEPLAY_KEEP, gDekuMaskDL, LINK_DL_MASK_DEKU);
    setupSharedListenerDL(GAMEPLAY_KEEP, gGoronMaskDL, LINK_DL_MASK_GORON);
    setupSharedListenerDL(GAMEPLAY_KEEP, gZoraMaskDL, LINK_DL_MASK_ZORA);
    setupSharedListenerDL(GAMEPLAY_KEEP, gFierceDeityMaskDL, LINK_DL_MASK_FIERCE_DEITY);
    setupSharedListenerDL(GAMEPLAY_KEEP, gameplay_keep_DL_06FE20, LINK_DL_LFIN_BOOMERANG);
    setupSharedListenerDL(GAMEPLAY_KEEP, gameplay_keep_DL_06FF68, LINK_DL_RFIN_BOOMERANG);

    // Human
    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanKokiriSwordSheathDL, LINK_DL_SWORD_KOKIRI_SHEATH);
    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanRazorSwordSheathDL, LINK_DL_SWORD_RAZOR_SHEATH);
    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordSheathDL, LINK_DL_SWORD_GILDED_SHEATH);

    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordHandleDL, LINK_DL_SWORD_GILDED_HILT);
    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordBladeDL, LINK_DL_SWORD_GILDED_BLADE);

    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanHerosShieldDL, LINK_DL_SHIELD_HERO);
    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanMirrorShieldDL, LINK_DL_SHIELD_MIRROR);
    setupSharedListenerDL(OBJECT_MIR_RAY, object_mir_ray_DL_0004B0, LINK_DL_SHIELD_MIRROR_RAY);

    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanBowDL, LINK_DL_BOW);
    setupSharedListenerDL(OBJECT_LINK_CHILD, object_link_child_DL_017818, LINK_DL_BOW_STRING);
    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanHookshotDL, LINK_DL_HOOKSHOT);
    setupSharedListenerDL(OBJECT_LINK_CHILD, object_link_child_DL_01D960, LINK_DL_HOOKSHOT_HOOK);

    models[LINK_DL_FPS_HOOKSHOT] = gLinkHumanFirstPersonHookshotDL;       // not in Link obj
    GlobalObjects_rebaseDL(GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD), gLinkHumanFirstPersonHookshotDL, 0x06); // repoint vertices, textures, etc. to static link obj

    setupSharedListenerDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL, LINK_DL_SWORD_GREAT_FAIRY_BLADE);
    models[LINK_DL_SWORD_GREAT_FAIRY_HILT] = gEmptyDL;

    setupSharedListenerDL(OBJECT_MASK_TRUTH, object_mask_truth_DL_0001A0, LINK_DL_MASK_TRUTH);
    setupSharedListenerDL(OBJECT_MASK_KERFAY, gKafeisMaskDL, LINK_DL_MASK_KAFEIS_MASK);
    setupSharedListenerDL(OBJECT_MASK_YOFUKASI, object_mask_yofukasi_DL_000490, LINK_DL_MASK_ALL_NIGHT);
    setupSharedListenerDL(OBJECT_MASK_RABIT, object_mask_rabit_DL_000610, LINK_DL_MASK_BUNNY);
    setupSharedListenerDL(OBJECT_MASK_KI_TAN, object_mask_ki_tan_DL_0004A0, LINK_DL_MASK_KEATON);
    setupSharedListenerDL(OBJECT_MASK_JSON, object_mask_json_DL_0004C0, LINK_DL_MASK_GARO);
    setupSharedListenerDL(OBJECT_MASK_ROMERNY, object_mask_romerny_DL_0007A0, LINK_DL_MASK_ROMANI);
    setupSharedListenerDL(OBJECT_MASK_ZACHO, object_mask_zacho_DL_000700, LINK_DL_MASK_CIRCUS_LEADER);
    setupSharedListenerDL(OBJECT_MASK_POSTHAT, object_mask_posthat_DL_000290, LINK_DL_MASK_POSTMAN);
    setupSharedListenerDL(OBJECT_MASK_BIGELF, object_mask_bigelf_DL_0016F0, LINK_DL_MASK_GREAT_FAIRY);
    setupSharedListenerDL(OBJECT_MASK_GIBUDO, object_mask_gibudo_DL_000250, LINK_DL_MASK_GIBDO);
    setupSharedListenerDL(OBJECT_MASK_GERO, gDonGeroMaskDL, LINK_DL_MASK_DON_GERO);
    setupSharedListenerDL(OBJECT_MASK_DANCER, object_mask_dancer_DL_000EF0, LINK_DL_MASK_KAMARO);
    setupSharedListenerDL(OBJECT_MASK_SKJ, object_mask_skj_DL_0009F0, LINK_DL_MASK_CAPTAIN);
    setupSharedListenerDL(OBJECT_MASK_STONE, object_mask_stone_DL_000820, LINK_DL_MASK_STONE);
    setupSharedListenerDL(OBJECT_MASK_BREE, object_mask_bree_DL_0003C0, LINK_DL_MASK_BREMEN);
    setupSharedListenerDL(OBJECT_MASK_BAKURETU, object_mask_bakuretu_DL_0005C0, LINK_DL_MASK_BLAST);
    setupSharedListenerDL(OBJECT_MASK_BAKURETU, object_mask_bakuretu_DL_000440, LINK_DL_MASK_BLAST_COOLING_DOWN);
    setupSharedListenerDL(OBJECT_MASK_BU_SAN, object_mask_bu_san_DL_000710, LINK_DL_MASK_SCENTS);
    setupSharedListenerDL(OBJECT_MASK_KYOJIN, object_mask_kyojin_DL_000380, LINK_DL_MASK_GIANT);
    setupSharedListenerDL(OBJECT_MASK_BOY, object_mask_boy_DL_000900, LINK_DL_MASK_FIERCE_DEITY_SCREAM);
    setupSharedListenerDL(OBJECT_MASK_GORON, object_mask_goron_DL_0014A0, LINK_DL_MASK_GORON_SCREAM);
    setupSharedListenerDL(OBJECT_MASK_ZORA, object_mask_zora_DL_000DB0, LINK_DL_MASK_ZORA_SCREAM);
    setupSharedListenerDL(OBJECT_MASK_NUTS, object_mask_nuts_DL_001D90, LINK_DL_MASK_DEKU_SCREAM);

    // Zora DLs
    setupSharedListenerDL(OBJECT_LINK_ZORA, object_link_zora_DL_00CC38, LINK_DL_LFIN);
    setupSharedListenerDL(OBJECT_LINK_ZORA, object_link_zora_DL_010868, LINK_DL_LFIN_SWIM);
    setupSharedListenerDL(OBJECT_LINK_ZORA, object_link_zora_DL_00CDA0, LINK_DL_RFIN);
    setupSharedListenerDL(OBJECT_LINK_ZORA, object_link_zora_DL_010978, LINK_DL_RFIN_SWIM);
    setupSharedListenerDL(OBJECT_LINK_ZORA, object_link_zora_DL_00E2A0, LINK_DL_GUITAR);
    setupSharedListenerDL(OBJECT_LINK_ZORA, object_link_zora_DL_0110A8, LINK_DL_FIN_SHIELD);
    setupSharedListenerDL(OBJECT_LINK_ZORA, object_link_zora_DL_011760, LINK_DL_MAGIC_BARRIER);

    // Fierce Deity DLs
    setupSharedListenerDL(OBJECT_LINK_BOY, gLinkFierceDeitySwordDL, LINK_DL_SWORD_FIERCE_DEITY_BLADE);
    models[LINK_DL_SWORD_FIERCE_DEITY_HILT] = gEmptyDL;

    // Deku DLs
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007390, LINK_DL_PIPE_MOUTH);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007548, LINK_DL_PIPE_RIGHT);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0076A0, LINK_DL_PIPE_UP);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0077D0, LINK_DL_PIPE_DOWN);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007900, LINK_DL_PIPE_LEFT);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007A28, LINK_DL_PIPE_A);

    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008660, LINK_DL_STEM_RIGHT);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008760, LINK_DL_STEM_LEFT);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0089F0, LINK_DL_FLOWER_CENTER_CLOSED);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008BA0, LINK_DL_FLOWER_CENTER_OPEN);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008AB8, LINK_DL_FLOWER_PROPELLER_OPEN);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008908, LINK_DL_FLOWER_PROPELLER_CLOSED);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_008860, LINK_DL_PETAL_PARTICLE);

    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_009AB8, LINK_DL_PAD_WOOD);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_009C48, LINK_DL_PAD_GRASS);
    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_009DB8, LINK_DL_PAD_OPENING);

    setupSharedListenerDL(OBJECT_LINK_NUTS, object_link_nuts_DL_00A348, LINK_DL_DEKU_GUARD);

    // Goron Link
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_00FC18, LINK_DL_DRUM_STRAP);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_00FCF0, LINK_DL_DRUM_UP);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_00FF18, LINK_DL_DRUM_LEFT);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_010140, LINK_DL_DRUM_RIGHT);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_010368, LINK_DL_DRUM_DOWN);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_010590, LINK_DL_DRUM_A);

    setupSharedListenerDL(OBJECT_LINK_GORON, gLinkGoronCurledDL, LINK_DL_CURLED);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_00C540, LINK_DL_SPIKES);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_0127B0, LINK_DL_FIRE_INIT);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_0134D0, LINK_DL_FIRE_ROLL);
    setupSharedListenerDL(OBJECT_LINK_GORON, gLinkGoronGoronPunchEffectDL, LINK_DL_FIRE_PUNCH);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_017180, LINK_DL_BODY_SHIELD_BODY);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_017620, LINK_DL_BODY_SHIELD_HEAD);
    setupSharedListenerDL(OBJECT_LINK_GORON, object_link_goron_DL_016A88, LINK_DL_BODY_SHIELD_ARMS_AND_LEGS);
}

RECOMP_CALLBACK(".", _internal_onReadyFormProxies)
void initSharedDLs_on_event() {
    initSharedDLs();
}
