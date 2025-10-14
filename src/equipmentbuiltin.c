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
#include "apilocal.h"

extern Gfx gLinkHumanFirstPersonHookshotDL[];
extern Gfx gLinkHumanFirstPersonBowDL[];

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

static Mtx sHookshotHookAndChainAdultMtx;

#undef DECLARE_STATIC_MATRIX_WRAPPED_DL

static Mtx sAdultShieldMatrix;

static void initCustomDLs() {
    void *human = GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD);

    guPosition(&sAdultShieldMatrix, 0.f, 0.f, 180.f, 1.f, 935.f, 94.f, 29.f);

    gSPBranchList(sCallHeroShield, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanHerosShieldDL));
    guPosition(&sHeroShieldResizer, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallMirrorShield, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanMirrorShieldDL));
    guPosition(&sMirrorShieldResizer, 0.f, 0.f, 0.f, 1.3f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallKokiriSwordHilt, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gKokiriSwordHandleDL));
    gSPBranchList(sCallKokiriSwordBlade, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gKokiriSwordBladeDL));
    gSPBranchList(sCallKokiriSwordSheath, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanKokiriSwordSheathDL));

    gSPBranchList(sCallRazorSwordHilt, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gRazorSwordHandleDL));
    gSPBranchList(sCallRazorSwordBlade, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gRazorSwordBladeDL));
    gSPBranchList(sCallRazorSwordSheath, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanRazorSwordSheathDL));

    gSPBranchList(sCallGildedSwordHilt, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanGildedSwordHandleDL));
    gSPBranchList(sCallGildedSwordBlade, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanGildedSwordBladeDL));
    gSPBranchList(sCallGildedSwordSheath, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanGildedSwordSheathDL));

    gSPBranchList(sCallHookshot, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, gLinkHumanHookshotDL));
    guPosition(&sHookshotResizer, 0.f, 0.f, 0.f, 1.4f, 50.f, 105.f, 0.f);

    guPosition(&sHookshotFirstPersonResizer, 0.f, 0.f, 0.f, 1.4f, 50.f, 105.f, 0.f);

    gSPBranchList(sCallHookshotChain, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gHookshotChainDL));

    gSPBranchList(sCallHookshotHook, GlobalObjects_getGlobalGfxPtr(OBJECT_LINK_CHILD, object_link_child_DL_01D960));
    guPosition(&sHookshotHookResizer, 0.f, 0.f, 0.f, 1.4f, 0.f, 0.f, 0.f);

    gSPBranchList(sCallHookshotReticle, GlobalObjects_getGlobalGfxPtr(GAMEPLAY_KEEP, gHookshotReticleDL));

    guPosition(&sHookshotHookAndChainAdultMtx, 0.f, 0.f, 0.f, 1.f, 50.f, 420.f, 0.f);
}

RECOMP_CALLBACK(".", onRegisterModels)
void registerMMEquipment() {
    initCustomDLs();

    void *human = GlobalObjects_getGlobalObject(OBJECT_LINK_CHILD);
    void *deku = GlobalObjects_getGlobalObject(OBJECT_LINK_NUTS);
    void *goron = GlobalObjects_getGlobalObject(OBJECT_LINK_GORON);
    void *zora = GlobalObjects_getGlobalObject(OBJECT_LINK_ZORA);
    void *deity = GlobalObjects_getGlobalObject(OBJECT_LINK_BOY);

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
    }

    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_mirror_shield3_a", PMM_MODEL_TYPE_SHIELD_MIRROR);
        PlayerModelManager_setDisplayName(h, "Mirror Shield (MM) (Adult)");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setMatrix(h, LINK_EQUIP_MATRIX_SHIELD_MIRROR_BACK, &sAdultShieldMatrix);
        PlayerModelManager_setDisplayList(h, LINK_DL_SHIELD_MIRROR, sMirrorShieldAdult);
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
}
