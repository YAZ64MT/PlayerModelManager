#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "playermodelmanager_mm.h"
#include "playermodelmanager_utils.h"
#include "model_common.h"
#include "globalobjects_api.h"

// handless ocarina
Gfx gLinkHumanOcarinaDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gLinkHumanOcarinaTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 5, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&object_link_childVtx_00E858[53], 23, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 3, 2, 0, 4, 2, 1, 0),
    gsSP2Triangles(5, 6, 7, 0, 8, 9, 10, 0),
    gsSP2Triangles(10, 6, 8, 0, 5, 8, 6, 0),
    gsSP2Triangles(11, 9, 0, 0, 10, 9, 11, 0),
    gsSP2Triangles(12, 1, 13, 0, 11, 3, 14, 0),
    gsSP2Triangles(11, 0, 3, 0, 0, 9, 1, 0),
    gsSP2Triangles(12, 4, 1, 0, 1, 9, 8, 0),
    gsSP2Triangles(1, 8, 5, 0, 14, 10, 11, 0),
    gsSP2Triangles(1, 5, 13, 0, 15, 13, 5, 0),
    gsSP2Triangles(15, 5, 7, 0, 7, 14, 15, 0),
    gsSP2Triangles(10, 14, 6, 0, 6, 14, 7, 0),
    gsSP2Triangles(12, 13, 16, 0, 12, 16, 17, 0),
    gsSP2Triangles(17, 18, 15, 0, 15, 16, 13, 0),
    gsSP2Triangles(17, 15, 14, 0, 17, 14, 19, 0),
    gsSP2Triangles(14, 20, 21, 0, 14, 21, 19, 0),
    gsSP2Triangles(4, 20, 3, 0, 3, 20, 14, 0),
    gsSP2Triangles(12, 17, 19, 0, 12, 19, 22, 0),
    gsSP2Triangles(12, 22, 4, 0, 21, 20, 4, 0),
    gsSPEndDisplayList(),
};

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

// first person arm
Gfx gLinkHumanFirstPersonArmDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal256(object_link_child_TLUT_018A40),
    gsDPLoadTextureBlock(object_link_child_Tex_019240, G_IM_FMT_CI, G_IM_SIZ_8b, 8, 8, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 3, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&object_link_childVtx_010538[607], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 2, 1, 0, 7, 2, 8, 0),
    gsSP2Triangles(7, 8, 9, 0, 10, 11, 12, 0),
    gsSP2Triangles(13, 14, 15, 0, 16, 17, 18, 0),
    gsSP2Triangles(19, 20, 21, 0, 22, 23, 24, 0),
    gsSP2Triangles(25, 20, 26, 0, 27, 28, 29, 0),
    gsSP1Triangle(27, 25, 30, 0),
    gsSPVertex(&object_link_childVtx_010538[638], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 0, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(7, 9, 10, 0, 11, 7, 12, 0),
    gsSP2Triangles(13, 14, 12, 0, 10, 9, 15, 0),
    gsSP2Triangles(10, 15, 6, 0, 16, 8, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 18, 16, 0),
    gsSP2Triangles(18, 20, 22, 0, 23, 24, 16, 0),
    gsSP2Triangles(3, 0, 25, 0, 26, 21, 27, 0),
    gsSP2Triangles(26, 27, 28, 0, 24, 29, 30, 0),
    gsSP1Triangle(25, 0, 31, 0),
    gsSPVertex(&object_link_childVtx_010538[670], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 1, 0, 12, 13, 14, 0),
    gsSP2Triangles(3, 2, 15, 0, 3, 15, 16, 0),
    gsSP2Triangles(17, 18, 19, 0, 20, 21, 22, 0),
    gsSP2Triangles(23, 24, 25, 0, 23, 26, 24, 0),
    gsSP2Triangles(23, 21, 20, 0, 27, 28, 29, 0),
    gsSP2Triangles(23, 25, 30, 0, 30, 21, 23, 0),
    gsSPVertex(&object_link_childVtx_010538[701], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 7, 6, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 12, 0),
    gsSP2Triangles(15, 0, 6, 0, 1, 0, 15, 0),
    gsSP2Triangles(16, 17, 1, 0, 15, 5, 18, 0),
    gsSP2Triangles(2, 17, 14, 0, 6, 19, 15, 0),
    gsSP2Triangles(15, 18, 1, 0, 20, 21, 22, 0),
    gsSP2Triangles(23, 24, 8, 0, 14, 17, 25, 0),
    gsSP2Triangles(26, 27, 28, 0, 11, 10, 29, 0),
    gsSP1Triangle(12, 30, 10, 0),
    gsSPVertex(&object_link_childVtx_010538[732], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 6, 4, 0, 6, 7, 4, 0),
    gsSP2Triangles(0, 8, 9, 0, 10, 11, 12, 0),
    gsSP2Triangles(0, 9, 13, 0, 3, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 20, 21, 0),
    gsSP2Triangles(22, 14, 23, 0, 3, 23, 14, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
    gsSP2Triangles(28, 29, 30, 0, 27, 14, 22, 0),
    gsSPVertex(&object_link_childVtx_010538[763], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 5, 6, 0, 2, 7, 0, 0),
    gsSP2Triangles(2, 1, 8, 0, 0, 9, 10, 0),
    gsSP2Triangles(2, 11, 12, 0, 13, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 3, 19, 20, 0),
    gsSP2Triangles(4, 21, 22, 0, 21, 23, 22, 0),
    gsSP2Triangles(12, 7, 2, 0, 24, 25, 8, 0),
    gsSP2Triangles(23, 26, 27, 0, 0, 10, 28, 0),
    gsSP1Triangle(29, 9, 30, 0),
    gsSPVertex(&object_link_childVtx_010538[794], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 1, 13, 14, 0),
    gsSP2Triangles(15, 16, 17, 0, 16, 15, 18, 0),
    gsSP2Triangles(18, 15, 19, 0, 20, 21, 17, 0),
    gsSP2Triangles(20, 22, 21, 0, 23, 24, 25, 0),
    gsSP2Triangles(26, 24, 23, 0, 20, 27, 22, 0),
    gsSP2Triangles(18, 28, 29, 0, 18, 30, 28, 0),
    gsSPVertex(&object_link_childVtx_010538[825], 31, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 0, 2, 0, 7, 3, 5, 0),
    gsSP2Triangles(8, 9, 4, 0, 6, 2, 10, 0),
    gsSP2Triangles(11, 10, 9, 0, 12, 13, 7, 0),
    gsSP2Triangles(7, 5, 12, 0, 14, 13, 12, 0),
    gsSP2Triangles(15, 16, 8, 0, 17, 3, 7, 0),
    gsSP2Triangles(18, 17, 7, 0, 17, 15, 3, 0),
    gsSP2Triangles(19, 20, 21, 0, 13, 20, 19, 0),
    gsSP2Triangles(20, 0, 21, 0, 14, 20, 13, 0),
    gsSP2Triangles(20, 1, 0, 0, 14, 1, 20, 0),
    gsSP2Triangles(6, 11, 22, 0, 6, 10, 11, 0),
    gsSP2Triangles(23, 24, 25, 0, 24, 26, 25, 0),
    gsSP2Triangles(23, 27, 24, 0, 23, 28, 27, 0),
    gsSP2Triangles(28, 29, 27, 0, 29, 30, 27, 0),
    gsSPEndDisplayList(),
};

void setupVanillaHuman() {
    Link_FormProxy *formProxy = &gLinkFormProxies[PLAYER_FORM_HUMAN];

    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];

    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        eyesTex[i] = formProxy->vanilla.eyesTextures[i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        mouthTex[i] = formProxy->vanilla.mouthTextures[i];
    }

    clearLinkModelInfo(&formProxy->vanilla);

    for (u32 i = 0; i < PLAYER_EYES_MAX; ++i) {
        formProxy->vanilla.eyesTextures[i] = eyesTex[i];
    }

    for (u32 i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        formProxy->vanilla.mouthTextures[i] = mouthTex[i];
    }

    loadVanillaSkeletonTransforms(&formProxy->vanilla, OBJECT_LINK_CHILD, gLinkHumanSkelLimbs);

    void *human = ObjectManager_getById(OBJECT_LINK_CHILD);

    Mtx **equipMatrixes = formProxy->vanilla.equipMtx;
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK] = (Mtx *)((uintptr_t)human + SEGMENT_OFFSET(&gLinkHumanSheathedKokiriSwordMtx));
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK] = (Mtx *)((uintptr_t)human + SEGMENT_OFFSET(&gLinkHumanSheathedRazorSwordMtx));
    equipMatrixes[LINK_EQUIP_MATRIX_SWORD_GILDED_BACK] = (Mtx *)((uintptr_t)human + SEGMENT_OFFSET(&gLinkHumanSheathedKokiriSwordMtx));
    equipMatrixes[LINK_EQUIP_MATRIX_SHIELD_HERO_BACK] = (Mtx *)((uintptr_t)human + SEGMENT_OFFSET(&gLinkHumanHerosShieldMtx));
    equipMatrixes[LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK] = (Mtx *)((uintptr_t)human + SEGMENT_OFFSET(&gLinkHumanMirrorShieldMtx));

    Gfx **models = formProxy->vanilla.models;

    // limbs
    models[LINK_DL_WAIST] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanWaistDL);
    models[LINK_DL_RTHIGH] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRightThighDL);
    models[LINK_DL_RSHIN] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRightShinDL);
    models[LINK_DL_RFOOT] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRightFootDL);
    models[LINK_DL_LTHIGH] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftThighDL);
    models[LINK_DL_LSHIN] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftShinDL);
    models[LINK_DL_LFOOT] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftFootDL);
    models[LINK_DL_HEAD] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanHeadDL);
    models[LINK_DL_HAT] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanHatDL);
    models[LINK_DL_COLLAR] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanCollarDL);
    models[LINK_DL_LSHOULDER] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftShoulderDL);
    models[LINK_DL_LFOREARM] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftForearmDL);
    models[LINK_DL_RSHOULDER] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRightShoulderDL);
    models[LINK_DL_RFOREARM] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRightForearmDL);
    models[LINK_DL_TORSO] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanTorsoDL);

    // hands
    models[LINK_DL_LHAND] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftHandOpenDL);
    models[LINK_DL_LFIST] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftHandClosedDL);
    models[LINK_DL_LHAND_BOTTLE] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftHandHoldBottleDL);
    models[LINK_DL_RHAND] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRightHandOpenDL);
    models[LINK_DL_RFIST] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRightHandClosedDL);

    // sword sheathes
    models[LINK_DL_SWORD_KOKIRI_SHEATH] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanKokiriSwordSheathDL);
    models[LINK_DL_SWORD_RAZOR_SHEATH] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRazorSwordSheathDL);
    models[LINK_DL_SWORD_GILDED_SHEATH] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanGildedSwordSheathDL);

    // sword hilts
    models[LINK_DL_SWORD_KOKIRI_HILT] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gKokiriSwordHandleDL);
    models[LINK_DL_SWORD_RAZOR_HILT] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gRazorSwordHandleDL);
    models[LINK_DL_SWORD_GILDED_HILT] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanGildedSwordHandleDL);

    // sword blades
    models[LINK_DL_SWORD_KOKIRI_BLADE] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gKokiriSwordBladeDL);
    models[LINK_DL_SWORD_RAZOR_BLADE] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gRazorSwordBladeDL);
    models[LINK_DL_SWORD_GILDED_BLADE] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanGildedSwordBladeDL);

    // shields
    models[LINK_DL_SHIELD_HERO] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanHerosShieldDL);
    models[LINK_DL_SHIELD_MIRROR] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanMirrorShieldDL);

    // items
    models[LINK_DL_OCARINA_TIME] = gLinkHumanOcarinaDL; // not in Link obj
    ZGlobalObj_rebaseDL(human, gLinkHumanOcarinaDL, 0x06); // repoint vertices, textures, etc. to static link obj

    models[LINK_DL_DEKU_STICK] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gDekuStickDL);
    models[LINK_DL_BOW] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanBowDL);
    //models[LINK_DL_BOW_STRING];
    models[LINK_DL_HOOKSHOT] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanHookshotDL);
    models[LINK_DL_HOOKSHOT_CHAIN] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gHookshotChainDL);
    models[LINK_DL_HOOKSHOT_RETICLE] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gHookshotReticleDL);

    // First Person
    models[LINK_DL_FPS_LFOREARM] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftForearmDL);
    models[LINK_DL_FPS_LHAND] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanLeftHandClosedDL);
    models[LINK_DL_FPS_RFOREARM] = dfCommand;

    models[LINK_DL_FPS_RHAND] = gLinkHumanFirstPersonArmDL; // not in Link obj
    ZGlobalObj_rebaseDL(human, gLinkHumanFirstPersonArmDL, 0x06); // repoint vertices, textures, etc. to static link obj

    models[LINK_DL_FPS_HOOKSHOT] = gLinkHumanFirstPersonHookshotDL; // not in Link obj
    ZGlobalObj_rebaseDL(human, gLinkHumanFirstPersonHookshotDL, 0x06); // repoint vertices, textures, etc. to static link obj

    // masks
    models[LINK_DL_MASK_DEKU] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gDekuMaskDL);
    models[LINK_DL_MASK_GORON] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gGoronMaskDL);
    models[LINK_DL_MASK_ZORA] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gZoraMaskDL);
    models[LINK_DK_MASK_DEITY] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gFierceDeityMaskDL);

    // bottles
    models[LINK_DL_BOTTLE_FILLING] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gBottleContentsDL);
    models[LINK_DL_BOTTLE_GLASS] = ZGlobalObj_getGlobalGfxPtr(GAMEPLAY_KEEP, gBottleGlassDL);

    // whole sword
    models[LINK_DL_SWORD_GREATFAIRY] = ZGlobalObj_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL);
}
