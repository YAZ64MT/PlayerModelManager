#include "global.h"
#include "globalobjects_api.h"
#include "playermodelmanager_api.h"
#include "playermodelmanager_mm.h"
#include "assets/objects/object_link_child/object_link_child.h"
#include "assets/objects/object_link_boy/object_link_boy.h"
#include "assets/objects/object_link_nuts/object_link_nuts.h"
#include "assets/objects/object_link_goron/object_link_goron.h"
#include "assets/objects/object_link_zora/object_link_zora.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_mir_ray/object_mir_ray.h"
#include "apilocal.h"

extern Gfx gLinkHumanFirstPersonHookshotDL[];
extern Gfx gLinkHumanFirstPersonBowDL[];
extern Gfx gLinkHumanOcarinaDL[];

#define DECLARE_STATIC_MATRIX_WRAPPED_DL(dlName, mtxName, dl)           \
    static Mtx mtxName;                                                 \
    static Gfx dlName[] = {                                             \
        gsSPMatrix(&mtxName, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW), \
        gsSPDisplayList(dl),                                            \
        gsSPPopMatrix(G_MTX_MODELVIEW),                                 \
        gsSPEndDisplayList(),                                           \
    }

static Gfx sCallHeroShieldDL[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHeroShieldAdultDL, sHeroShieldResizerMtx, sCallHeroShieldDL);

static Gfx sCallMirrorShieldDL[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sMirrorShieldAdultDL, sMirrorShieldResizerMtx, sCallMirrorShieldDL);

static Gfx sCallMirrorShieldRayDL[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sMirrorShieldRayAdultDL, sMirrorShieldRayResizerMtx, sCallMirrorShieldRayDL);

static Gfx sCallMirrorShieldRayBeamDL[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sMirrorShieldRayBeamAdultDL, sMirrorShieldRayBeamResizerMtx, sCallMirrorShieldRayBeamDL);

static Gfx sCallKokiriSwordHiltDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallKokiriSwordBladeDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallKokiriSwordSheathDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallRazorSwordHiltDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallRazorSwordBladeDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallRazorSwordSheathDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallGildedSwordHiltDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallGildedSwordBladeDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallGildedSwordSheathDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallHookshotDL[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHookshotAdultDL, sHookshotResizerMtx, sCallHookshotDL);

static Gfx sCallHookshotFirstPersonDL[] = {
    gsSPBranchList(gLinkHumanFirstPersonHookshotDL),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHookshotFirstPersonAdultDL, sHookshotFirstPersonResizerMtx, sCallHookshotFirstPersonDL);

static Gfx sCallHookshotHookDL[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHookshotHookAdultDL, sHookshotHookResizerMtx, sCallHookshotHookDL);

static Gfx sCallHookshotChainDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallHookshotReticleDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBowDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBowFirstPersonDL[] = {
    gsSPBranchList(gLinkHumanFirstPersonBowDL),
};

static Gfx sCallBowStringDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallArrowDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBottleGlassDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBottleContentsDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallOcarinaDL[] = {
    gsSPBranchList(gLinkHumanOcarinaDL),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sOcarinaAdultDL, sOcarinaResizerMtx, sCallOcarinaDL);

static Gfx sCallDekuStickDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallPipesMouthDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallPipesADL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallPipesUpDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallPipesDownDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallPipesLeftDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallPipesRightDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallDrumStrapDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallDrumUpDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallDrumDownDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallDrumLeftDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallDrumRightDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallDrumADL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallGuitarDL[] = {
    gsSPEndDisplayList(),
};

#undef DECLARE_STATIC_MATRIX_WRAPPED_DL

static Mtx sHookshotHookAndChainAdultMtx;

static Mtx sAdultShieldMtx;

static Gfx *getRepointedMMDL(ObjectId id, Gfx *g) {
    return GlobalObjects_getGlobalGfxPtr(id, g);
}

static void *getMMObject(ObjectId id) {
    return GlobalObjects_getGlobalObject(id);
}

static void initCustomDLs() {
    void *human = getMMObject(OBJECT_LINK_CHILD);

    guPosition(&sAdultShieldMtx, 0.f, 0.f, 180.f, 1.f, 935.f, 94.f, 29.f);

    gSPBranchList(sCallHeroShieldDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanHerosShieldDL));
    guPosition(&sHeroShieldResizerMtx, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallMirrorShieldDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanMirrorShieldDL));
    guPosition(&sMirrorShieldResizerMtx, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallMirrorShieldRayDL, getRepointedMMDL(OBJECT_MIR_RAY, object_mir_ray_DL_0004B0));
    guPosition(&sMirrorShieldRayResizerMtx, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallMirrorShieldRayBeamDL, getRepointedMMDL(OBJECT_MIR_RAY, object_mir_ray_DL_000168));
    guPosition(&sMirrorShieldRayBeamResizerMtx, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallKokiriSwordHiltDL, getRepointedMMDL(GAMEPLAY_KEEP, gKokiriSwordHandleDL));
    gSPBranchList(sCallKokiriSwordBladeDL, getRepointedMMDL(GAMEPLAY_KEEP, gKokiriSwordBladeDL));
    gSPBranchList(sCallKokiriSwordSheathDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanKokiriSwordSheathDL));

    gSPBranchList(sCallRazorSwordHiltDL, getRepointedMMDL(GAMEPLAY_KEEP, gRazorSwordHandleDL));
    gSPBranchList(sCallRazorSwordBladeDL, getRepointedMMDL(GAMEPLAY_KEEP, gRazorSwordBladeDL));
    gSPBranchList(sCallRazorSwordSheathDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanRazorSwordSheathDL));

    gSPBranchList(sCallGildedSwordHiltDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordHandleDL));
    gSPBranchList(sCallGildedSwordBladeDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordBladeDL));
    gSPBranchList(sCallGildedSwordSheathDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordSheathDL));

    gSPBranchList(sCallHookshotDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanHookshotDL));
    guPosition(&sHookshotResizerMtx, 0.f, 0.f, 0.f, 1.4f, 50.f, 105.f, 0.f);
    sHookshotFirstPersonResizerMtx = sHookshotResizerMtx;

    gSPBranchList(sCallHookshotChainDL, getRepointedMMDL(GAMEPLAY_KEEP, gHookshotChainDL));

    gSPBranchList(sCallHookshotHookDL, getRepointedMMDL(OBJECT_LINK_CHILD, object_link_child_DL_01D960));
    guPosition(&sHookshotHookResizerMtx, 0.f, 0.f, 0.f, 1.4f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallHookshotReticleDL, getRepointedMMDL(GAMEPLAY_KEEP, gHookshotReticleDL));

    guPosition(&sHookshotHookAndChainAdultMtx, 0.f, 0.f, 0.f, 1.f, 50.f, 420.f, 0.f);

    gSPBranchList(sCallBowDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanBowDL));
    gSPBranchList(sCallBowStringDL, getRepointedMMDL(OBJECT_LINK_CHILD, object_link_child_DL_017818));
    gSPBranchList(sCallArrowDL, getRepointedMMDL(GAMEPLAY_KEEP, gameplay_keep_DL_013FF0));

    gSPBranchList(sCallBottleGlassDL, getRepointedMMDL(GAMEPLAY_KEEP, gBottleGlassDL));
    gSPBranchList(sCallBottleContentsDL, getRepointedMMDL(GAMEPLAY_KEEP, gBottleContentsDL));

    guPosition(&sOcarinaResizerMtx, 7.f, -8.f, -4.f, 1.f, 72.f, 238.f, -3.f);

    gSPBranchList(sCallDekuStickDL, getRepointedMMDL(GAMEPLAY_KEEP, gDekuStickDL));

    gSPBranchList(sCallPipesMouthDL, getRepointedMMDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007390));
    gSPBranchList(sCallPipesRightDL, getRepointedMMDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007548));
    gSPBranchList(sCallPipesUpDL, getRepointedMMDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0076A0));
    gSPBranchList(sCallPipesDownDL, getRepointedMMDL(OBJECT_LINK_NUTS, object_link_nuts_DL_0077D0));
    gSPBranchList(sCallPipesLeftDL, getRepointedMMDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007900));
    gSPBranchList(sCallPipesADL, getRepointedMMDL(OBJECT_LINK_NUTS, object_link_nuts_DL_007A28));

    gSPBranchList(sCallDrumStrapDL, getRepointedMMDL(OBJECT_LINK_GORON, object_link_goron_DL_00FC18));
    gSPBranchList(sCallDrumUpDL, getRepointedMMDL(OBJECT_LINK_GORON, object_link_goron_DL_00FCF0));
    gSPBranchList(sCallDrumDownDL, getRepointedMMDL(OBJECT_LINK_GORON, object_link_goron_DL_010368));
    gSPBranchList(sCallDrumLeftDL, getRepointedMMDL(OBJECT_LINK_GORON, object_link_goron_DL_00FF18));
    gSPBranchList(sCallDrumRightDL, getRepointedMMDL(OBJECT_LINK_GORON, object_link_goron_DL_010140));
    gSPBranchList(sCallDrumADL, getRepointedMMDL(OBJECT_LINK_GORON, object_link_goron_DL_010590));

    gSPBranchList(sCallGuitarDL, getRepointedMMDL(OBJECT_LINK_ZORA, object_link_zora_DL_00E2A0));
}

RECOMP_CALLBACK(".", onRegisterModels)
void registerMMEquipment() {
    initCustomDLs();

    void *human = getMMObject(OBJECT_LINK_CHILD);
    void *deku = getMMObject(OBJECT_LINK_NUTS);
    void *goron = getMMObject(OBJECT_LINK_GORON);
    void *zora = getMMObject(OBJECT_LINK_ZORA);
    void *deity = getMMObject(OBJECT_LINK_BOY);

    // Hero's Shield
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_hero_shield2_c", PMM_MODEL_TYPE_SHIELD_HERO);
        PlayerModelManager_setDisplayName(h, "Hero's Shield");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_HERO_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanHerosShieldMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_HERO, sCallHeroShieldDL);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_hero_shield2_a", PMM_MODEL_TYPE_SHIELD_HERO);
        PlayerModelManager_setDisplayName(h, "Hero's Shield (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_HERO_BACK, &sAdultShieldMtx);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_HERO, sHeroShieldAdultDL);
    }

    // Mirror Shield (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_mirror_shield3_c", PMM_MODEL_TYPE_SHIELD_MIRROR);
        PlayerModelManager_setDisplayName(h, "Mirror Shield (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanMirrorShieldMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR, sCallMirrorShieldDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY, sCallMirrorShieldRayDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY_BEAM, sCallMirrorShieldRayBeamDL);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_mirror_shield3_a", PMM_MODEL_TYPE_SHIELD_MIRROR);
        PlayerModelManager_setDisplayName(h, "Mirror Shield (MM) (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK, &sAdultShieldMtx);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR, sMirrorShieldAdultDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY, sMirrorShieldRayAdultDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY_BEAM, sMirrorShieldRayBeamAdultDL);
    }

    // Kokiri Sword (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_kokiri_sword1_c", PMM_MODEL_TYPE_SWORD_KOKIRI);
        PlayerModelManager_setDisplayName(h, "Kokiri Sword (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSheathedKokiriSwordMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_KOKIRI_HILT, sCallKokiriSwordHiltDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_KOKIRI_BLADE, sCallKokiriSwordBladeDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_KOKIRI_SHEATH, sCallKokiriSwordSheathDL);
    }

    // Razor Sword (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_razor_sword2_c", PMM_MODEL_TYPE_SWORD_RAZOR);
        PlayerModelManager_setDisplayName(h, "Razor Sword");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSheathedRazorSwordMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_RAZOR_HILT, sCallRazorSwordHiltDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_RAZOR_BLADE, sCallRazorSwordBladeDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_RAZOR_SHEATH, sCallRazorSwordSheathDL);
    }

    // Gilded Sword (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_gilded_sword3_c", PMM_MODEL_TYPE_SWORD_GILDED);
        PlayerModelManager_setDisplayName(h, "Gilded Sword");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SWORD_GILDED_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSheathedKokiriSwordMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GILDED_HILT, sCallGildedSwordHiltDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GILDED_BLADE, sCallGildedSwordBladeDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GILDED_SHEATH, sCallGildedSwordSheathDL);
    }

    // Hookshot (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_hookshot_c", PMM_MODEL_TYPE_HOOKSHOT);
        PlayerModelManager_setDisplayName(h, "Hookshot (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK, &gIdentityMtx);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT, sCallHookshotDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_FPS_HOOKSHOT, sCallHookshotFirstPersonDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_CHAIN, sCallHookshotChainDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_HOOK, sCallHookshotHookDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_RETICLE, sCallHookshotReticleDL);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_hookshot_a", PMM_MODEL_TYPE_HOOKSHOT);
        PlayerModelManager_setDisplayName(h, "Hookshot (MM) (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK, &sHookshotHookAndChainAdultMtx);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT, sHookshotAdultDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_FPS_HOOKSHOT, sHookshotFirstPersonAdultDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_CHAIN, sCallHookshotChainDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_HOOK, sHookshotHookAdultDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_RETICLE, sCallHookshotReticleDL);
    }

    // Bow (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_bow_c", PMM_MODEL_TYPE_BOW);
        PlayerModelManager_setDisplayName(h, "Bow (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_BOW, sCallBowDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_FPS_BOW, sCallBowFirstPersonDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_BOW_STRING, sCallBowStringDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_BOW_ARROW, sCallArrowDL);
    }

    // Bottle (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_bottle_c", PMM_MODEL_TYPE_BOTTLE);
        PlayerModelManager_setDisplayName(h, "Bottle (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_BOTTLE_GLASS, sCallBottleGlassDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_BOTTLE_CONTENTS, sCallBottleContentsDL);
    }

    // Ocarina of Time
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_ocarina2_time_c", PMM_MODEL_TYPE_OCARINA_TIME);
        PlayerModelManager_setDisplayName(h, "Ocarina of Time (Child)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_OCARINA_TIME, sCallOcarinaDL);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_ocarina2_time_a", PMM_MODEL_TYPE_OCARINA_TIME);
        PlayerModelManager_setDisplayName(h, "Ocarina of Time (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_OCARINA_TIME, sOcarinaAdultDL);
    }

    // Deku Stick (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_deku_stick_c", PMM_MODEL_TYPE_DEKU_STICK);
        PlayerModelManager_setDisplayName(h, "Deku Stick (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_DEKU_STICK, sCallDekuStickDL);
    }

    // Deku Pipes
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_pipes_c", PMM_MODEL_TYPE_PIPES);
        PlayerModelManager_setDisplayName(h, "Deku Pipes");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_PIPE_MOUTH, sCallPipesMouthDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_PIPE_UP, sCallPipesUpDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_PIPE_DOWN, sCallPipesDownDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_PIPE_LEFT, sCallPipesLeftDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_PIPE_RIGHT, sCallPipesRightDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_PIPE_A, sCallPipesADL);
    }

    // Goron Drums
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_drums_a", PMM_MODEL_TYPE_DRUMS);
        PlayerModelManager_setDisplayName(h, "Goron Drums");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_DRUM_STRAP, sCallDrumStrapDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_DRUM_UP, sCallDrumUpDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_DRUM_DOWN, sCallDrumDownDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_DRUM_LEFT, sCallDrumLeftDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_DRUM_RIGHT, sCallDrumRightDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_DRUM_A, sCallDrumADL);
    }

    // Zora Guitar
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_guitar_a", PMM_MODEL_TYPE_GUITAR);
        PlayerModelManager_setDisplayName(h, "Zora Guitar");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_GUITAR, sCallGuitarDL);
    }
}
