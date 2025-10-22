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

static Gfx sCallHeroShield[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHeroShieldAdult, sHeroShieldResizer, sCallHeroShield);

static Gfx sCallMirrorShield[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sMirrorShieldAdult, sMirrorShieldResizer, sCallMirrorShield);

static Gfx sCallMirrorShieldRay[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sMirrorShieldRayAdult, sMirrorShieldRayResizer, sCallMirrorShieldRay);

static Gfx sCallMirrorShieldRayBeam[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sMirrorShieldRayBeamAdult, sMirrorShieldRayBeamResizer, sCallMirrorShieldRayBeam);

static Gfx sCallKokiriSwordHilt[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallKokiriSwordBlade[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallKokiriSwordSheath[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallRazorSwordHilt[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallRazorSwordBlade[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallRazorSwordSheath[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallGildedSwordHilt[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallGildedSwordBlade[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallGildedSwordSheath[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallHookshot[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHookshotAdult, sHookshotResizer, sCallHookshot);

static Gfx sCallHookshotFirstPerson[] = {
    gsSPBranchList(gLinkHumanFirstPersonHookshotDL),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHookshotFirstPersonAdult, sHookshotFirstPersonResizer, sCallHookshotFirstPerson);

static Gfx sCallHookshotHook[] = {
    gsSPEndDisplayList(),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sHookshotHookAdult, sHookshotHookResizer, sCallHookshotHook);

static Gfx sCallHookshotChain[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallHookshotReticle[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBow[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBowFirstPerson[] = {
    gsSPBranchList(gLinkHumanFirstPersonBowDL),
};

static Gfx sCallBowString[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallArrow[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBottleGlass[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallBottleContents[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallOcarina[] = {
    gsSPBranchList(gLinkHumanOcarinaDL),
};

DECLARE_STATIC_MATRIX_WRAPPED_DL(sOcarinaAdult, sOcarinaResizer, sCallOcarina);

static Gfx sCallDekuStick[] = {
    gsSPEndDisplayList(),
};

#undef DECLARE_STATIC_MATRIX_WRAPPED_DL

static Mtx sHookshotHookAndChainAdultMtx;

static Mtx sAdultShieldMatrix;

static Gfx *getRepointedMMDL(ObjectId id, Gfx *g) {
    return GlobalObjects_getGlobalGfxPtr(id, g);
}

static void *getMMObject(ObjectId id) {
    return GlobalObjects_getGlobalObject(id);
}

static void initCustomDLs() {
    void *human = getMMObject(OBJECT_LINK_CHILD);

    guPosition(&sAdultShieldMatrix, 0.f, 0.f, 180.f, 1.f, 935.f, 94.f, 29.f);

    gSPBranchList(sCallHeroShield, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanHerosShieldDL));
    guPosition(&sHeroShieldResizer, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallMirrorShield, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanMirrorShieldDL));
    guPosition(&sMirrorShieldResizer, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallMirrorShieldRay, getRepointedMMDL(OBJECT_MIR_RAY, object_mir_ray_DL_0004B0));
    guPosition(&sMirrorShieldRayResizer, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallMirrorShieldRayBeam, getRepointedMMDL(OBJECT_MIR_RAY, object_mir_ray_DL_000168));
    guPosition(&sMirrorShieldRayBeamResizer, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallKokiriSwordHilt, getRepointedMMDL(GAMEPLAY_KEEP, gKokiriSwordHandleDL));
    gSPBranchList(sCallKokiriSwordBlade, getRepointedMMDL(GAMEPLAY_KEEP, gKokiriSwordBladeDL));
    gSPBranchList(sCallKokiriSwordSheath, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanKokiriSwordSheathDL));

    gSPBranchList(sCallRazorSwordHilt, getRepointedMMDL(GAMEPLAY_KEEP, gRazorSwordHandleDL));
    gSPBranchList(sCallRazorSwordBlade, getRepointedMMDL(GAMEPLAY_KEEP, gRazorSwordBladeDL));
    gSPBranchList(sCallRazorSwordSheath, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanRazorSwordSheathDL));

    gSPBranchList(sCallGildedSwordHilt, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordHandleDL));
    gSPBranchList(sCallGildedSwordBlade, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordBladeDL));
    gSPBranchList(sCallGildedSwordSheath, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanGildedSwordSheathDL));

    gSPBranchList(sCallHookshot, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanHookshotDL));
    guPosition(&sHookshotResizer, 0.f, 0.f, 0.f, 1.4f, 50.f, 105.f, 0.f);

    guPosition(&sHookshotFirstPersonResizer, 0.f, 0.f, 0.f, 1.4f, 50.f, 105.f, 0.f);

    gSPBranchList(sCallHookshotChain, getRepointedMMDL(GAMEPLAY_KEEP, gHookshotChainDL));

    gSPBranchList(sCallHookshotHook, getRepointedMMDL(OBJECT_LINK_CHILD, object_link_child_DL_01D960));
    guPosition(&sHookshotHookResizer, 0.f, 0.f, 0.f, 1.4f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallHookshotReticle, getRepointedMMDL(GAMEPLAY_KEEP, gHookshotReticleDL));

    guPosition(&sHookshotHookAndChainAdultMtx, 0.f, 0.f, 0.f, 1.f, 50.f, 420.f, 0.f);

    gSPBranchList(sCallBow, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanBowDL));
    gSPBranchList(sCallBowString, getRepointedMMDL(OBJECT_LINK_CHILD, object_link_child_DL_017818));
    gSPBranchList(sCallArrow, getRepointedMMDL(GAMEPLAY_KEEP, gameplay_keep_DL_013FF0));

    gSPBranchList(sCallBottleGlass, getRepointedMMDL(GAMEPLAY_KEEP, gBottleGlassDL));
    gSPBranchList(sCallBottleContents, getRepointedMMDL(GAMEPLAY_KEEP, gBottleContentsDL));

    guPosition(&sOcarinaResizer, 7.f, -8.f, -4.f, 1.f, 72.f, 238.f, -3.f);

    gSPBranchList(sCallDekuStick, getRepointedMMDL(GAMEPLAY_KEEP, gDekuStickDL));
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
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_HERO, sCallHeroShield);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_hero_shield2_a", PMM_MODEL_TYPE_SHIELD_HERO);
        PlayerModelManager_setDisplayName(h, "Hero's Shield (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_HERO_BACK, &sAdultShieldMatrix);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_HERO, sHeroShieldAdult);
    }

    // Mirror Shield (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_mirror_shield3_c", PMM_MODEL_TYPE_SHIELD_MIRROR);
        PlayerModelManager_setDisplayName(h, "Mirror Shield (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanMirrorShieldMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR, sCallMirrorShield);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY, sCallMirrorShieldRay);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY_BEAM, sCallMirrorShieldRayBeam);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_mirror_shield3_a", PMM_MODEL_TYPE_SHIELD_MIRROR);
        PlayerModelManager_setDisplayName(h, "Mirror Shield (MM) (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK, &sAdultShieldMatrix);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR, sMirrorShieldAdult);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY, sMirrorShieldRayAdult);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR_RAY_BEAM, sMirrorShieldRayBeamAdult);
    }

    // Kokiri Sword (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_kokiri_sword1_c", PMM_MODEL_TYPE_SWORD_KOKIRI);
        PlayerModelManager_setDisplayName(h, "Kokiri Sword (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SWORD_KOKIRI_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSheathedKokiriSwordMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_KOKIRI_HILT, sCallKokiriSwordHilt);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_KOKIRI_BLADE, sCallKokiriSwordBlade);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_KOKIRI_SHEATH, sCallKokiriSwordSheath);
    }

    // Razor Sword (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_razor_sword2_c", PMM_MODEL_TYPE_SWORD_RAZOR);
        PlayerModelManager_setDisplayName(h, "Razor Sword");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SWORD_RAZOR_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSheathedRazorSwordMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_RAZOR_HILT, sCallRazorSwordHilt);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_RAZOR_BLADE, sCallRazorSwordBlade);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_RAZOR_SHEATH, sCallRazorSwordSheath);
    }

    // Gilded Sword (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_gilded_sword3_c", PMM_MODEL_TYPE_SWORD_GILDED);
        PlayerModelManager_setDisplayName(h, "Gilded Sword");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SWORD_GILDED_BACK, SEGMENTED_TO_GLOBAL_PTR(human, &gLinkHumanSheathedKokiriSwordMtx));
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GILDED_HILT, sCallGildedSwordHilt);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GILDED_BLADE, sCallGildedSwordBlade);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GILDED_SHEATH, sCallGildedSwordSheath);
    }

    // Hookshot (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_hookshot_c", PMM_MODEL_TYPE_HOOKSHOT);
        PlayerModelManager_setDisplayName(h, "Hookshot (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK, &gIdentityMtx);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT, sCallHookshot);
        PlayerModelManager_setDisplayList(h, LINK_DL_FPS_HOOKSHOT, sCallHookshotFirstPerson);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_CHAIN, sCallHookshotChain);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_HOOK, sCallHookshotHook);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_RETICLE, sCallHookshotReticle);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_hookshot_a", PMM_MODEL_TYPE_HOOKSHOT);
        PlayerModelManager_setDisplayName(h, "Hookshot (MM) (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_HOOKSHOT_CHAIN_AND_HOOK, &sHookshotHookAndChainAdultMtx);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT, sHookshotAdult);
        PlayerModelManager_setDisplayList(h, LINK_DL_FPS_HOOKSHOT, sHookshotFirstPersonAdult);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_CHAIN, sCallHookshotChain);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_HOOK, sHookshotHookAdult);
        PlayerModelManager_setDisplayList(h, LINK_DL_HOOKSHOT_RETICLE, sCallHookshotReticle);
    }

    // Bow (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_bow_c", PMM_MODEL_TYPE_BOW);
        PlayerModelManager_setDisplayName(h, "Bow (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_BOW, sCallBow);
        PlayerModelManager_setDisplayList(h, LINK_DL_FPS_BOW, sCallBowFirstPerson);
        PlayerModelManager_setDisplayList(h, LINK_DL_BOW_STRING, sCallBowString);
        PlayerModelManager_setDisplayList(h, LINK_DL_BOW_ARROW, sCallArrow);
    }

    // Bottle (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_bottle_c", PMM_MODEL_TYPE_BOTTLE);
        PlayerModelManager_setDisplayName(h, "Bottle (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_BOTTLE_GLASS, sCallBottleGlass);
        PlayerModelManager_setDisplayList(h, LINK_DL_BOTTLE_CONTENTS, sCallBottleContents);
    }

    // Ocarina of Time
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_ocarina2_time_c", PMM_MODEL_TYPE_OCARINA_TIME);
        PlayerModelManager_setDisplayName(h, "Ocarina of Time (Child)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_OCARINA_TIME, sCallOcarina);
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_ocarina2_time_a", PMM_MODEL_TYPE_OCARINA_TIME);
        PlayerModelManager_setDisplayName(h, "Ocarina of Time (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_OCARINA_TIME, sOcarinaAdult);
    }

    // Deku Stick (MM)
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_deku_stick_c", PMM_MODEL_TYPE_DEKU_STICK);
        PlayerModelManager_setDisplayName(h, "Deku Stick (MM)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_DEKU_STICK, sCallDekuStick);
    }
}
