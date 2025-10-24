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
#include "maskdls.h"
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

static Gfx sCallGreatFairySwordDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallFierceDeitySwordDL[] = {
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

static Gfx sCallMaskTruthDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskKafeiDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskAllNightDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskBunnyHoodDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskKeatonDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskGaroDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskRomaniDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskCircusLeaderDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskPostmanDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskCoupleDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskGreatFairyDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskGibdoDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskDonGeroDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskKamaroDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskCaptainDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskStoneDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskBremenDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskBlastDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskBlastCooldownDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskScentsDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskGiantDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskFierceDeityDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskFierceDeityScreamDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskGoronDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskGoronScreamDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskZoraDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskZoraScreamDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskDekuDL[] = {
    gsSPEndDisplayList(),
};

static Gfx sCallMaskDekuScreamDL[] = {
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

    gSPBranchList(sCallGreatFairySwordDL, getRepointedMMDL(OBJECT_LINK_CHILD, gLinkHumanGreatFairysSwordDL));

    gSPBranchList(sCallFierceDeitySwordDL, getRepointedMMDL(OBJECT_LINK_BOY, gLinkFierceDeitySwordDL));

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

    gSPBranchList(sCallMaskTruthDL, getRepointedMMDL(OBJECT_MASK_TRUTH, object_mask_truth_DL_0001A0));
    gSPBranchList(sCallMaskKafeiDL, getRepointedMMDL(OBJECT_MASK_KERFAY, gKafeisMaskDL));
    gSPBranchList(sCallMaskAllNightDL, getRepointedMMDL(OBJECT_MASK_YOFUKASI, object_mask_yofukasi_DL_000490));
    gSPBranchList(sCallMaskBunnyHoodDL, getRepointedMMDL(OBJECT_MASK_RABIT, object_mask_rabit_DL_000610));
    gSPBranchList(sCallMaskKeatonDL, getRepointedMMDL(OBJECT_MASK_KI_TAN, object_mask_ki_tan_DL_0004A0));
    gSPBranchList(sCallMaskGaroDL, getRepointedMMDL(OBJECT_MASK_JSON, object_mask_json_DL_0004C0));
    gSPBranchList(sCallMaskRomaniDL, getRepointedMMDL(OBJECT_MASK_ROMERNY, object_mask_romerny_DL_0007A0));
    gSPBranchList(sCallMaskCircusLeaderDL, getRepointedMMDL(OBJECT_MASK_ZACHO, object_mask_zacho_DL_000700));
    gSPBranchList(sCallMaskPostmanDL, getRepointedMMDL(OBJECT_MASK_POSTHAT, object_mask_posthat_DL_000290));
    gSPBranchList(sCallMaskCoupleDL, getRepointedMMDL(OBJECT_MASK_MEOTO, object_mask_meoto_DL_0005A0));
    gSPBranchList(sCallMaskGreatFairyDL, getRepointedMMDL(OBJECT_MASK_BIGELF, object_mask_bigelf_DL_0016F0));
    gSPBranchList(sCallMaskGibdoDL, getRepointedMMDL(OBJECT_MASK_GIBUDO, object_mask_gibudo_DL_000250));
    gSPBranchList(sCallMaskDonGeroDL, getRepointedMMDL(OBJECT_MASK_GERO, gDonGeroMaskDL));
    gSPBranchList(sCallMaskKamaroDL, getRepointedMMDL(OBJECT_MASK_DANCER, object_mask_dancer_DL_000EF0));
    gSPBranchList(sCallMaskCaptainDL, getRepointedMMDL(OBJECT_MASK_SKJ, object_mask_skj_DL_0009F0));
    gSPBranchList(sCallMaskBremenDL, getRepointedMMDL(OBJECT_MASK_BREE, object_mask_bree_DL_0003C0));
    gSPBranchList(sCallMaskBlastDL, getRepointedMMDL(OBJECT_MASK_BAKURETU, object_mask_bakuretu_DL_0005C0));
    gSPBranchList(sCallMaskBlastCooldownDL, getRepointedMMDL(OBJECT_MASK_BAKURETU, object_mask_bakuretu_DL_000440));
    gSPBranchList(sCallMaskScentsDL, getRepointedMMDL(OBJECT_MASK_BU_SAN, object_mask_bu_san_DL_000710));
    gSPBranchList(sCallMaskGiantDL, getRepointedMMDL(OBJECT_MASK_KYOJIN, object_mask_kyojin_DL_000380));
    gSPBranchList(sCallMaskFierceDeityDL, getRepointedMMDL(GAMEPLAY_KEEP, gFierceDeityMaskDL));
    gSPBranchList(sCallMaskFierceDeityScreamDL, getRepointedMMDL(OBJECT_MASK_BOY, object_mask_boy_DL_000900));
    gSPBranchList(sCallMaskGoronDL, getRepointedMMDL(GAMEPLAY_KEEP, gGoronMaskDL));
    gSPBranchList(sCallMaskGoronScreamDL, getRepointedMMDL(OBJECT_MASK_GORON, object_mask_goron_DL_0014A0));
    gSPBranchList(sCallMaskZoraDL, getRepointedMMDL(GAMEPLAY_KEEP, gZoraMaskDL));
    gSPBranchList(sCallMaskZoraScreamDL, getRepointedMMDL(OBJECT_MASK_ZORA, object_mask_zora_DL_000DB0));
    gSPBranchList(sCallMaskDekuDL, getRepointedMMDL(GAMEPLAY_KEEP, gDekuMaskDL));
    gSPBranchList(sCallMaskDekuScreamDL, getRepointedMMDL(OBJECT_MASK_NUTS, object_mask_nuts_DL_001D90));
}

static void registerMaskDL(const char *internalName, const char *displayName, PlayerModelManagerModelType modelType, Link_DisplayList dlId, Gfx *dl) {
    PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, internalName, modelType);
    PlayerModelManager_setDisplayName(h, displayName);
    PlayerModelManager_setAuthor(h, "Nintendo");
    PlayerModelManager_setDisplayList(h, dlId, dl);
}

static void registerMaskDL2(const char *internalName, const char *displayName, PlayerModelManagerModelType modelType, Link_DisplayList dlId1, Gfx *dl1, Link_DisplayList dlId2, Gfx *dl2) {
    PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, internalName, modelType);
    PlayerModelManager_setDisplayName(h, displayName);
    PlayerModelManager_setAuthor(h, "Nintendo");
    PlayerModelManager_setDisplayList(h, dlId1, dl1);
    PlayerModelManager_setDisplayList(h, dlId2, dl2);
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

    // Fierce Deity Sword
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_great_fairy_sword4_a", PMM_MODEL_TYPE_SWORD_FIERCE_DIETY);
        PlayerModelManager_setDisplayName(h, "Fierce Deity's Sword");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_FIERCE_DEITY_BLADE, sCallFierceDeitySwordDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_FIERCE_DEITY_HILT, gEmptyDL);
    }

    // Great Fairy's Sword
    {
        PlayerModelManagerHandle h = PlayerModelManager_registerModel(PMM_API_VERSION, "mm_great_fairy_sword5_c", PMM_MODEL_TYPE_SWORD_GREAT_FAIRY);
        PlayerModelManager_setDisplayName(h, "Great Fairy's Sword");
        PlayerModelManager_setAuthor(h, "Nintendo");
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GREAT_FAIRY_BLADE, sCallGreatFairySwordDL);
        PlayerModelManager_setDisplayList(h, LINK_DL_SWORD_GREAT_FAIRY_HILT, gEmptyDL);
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

    // Masks
    registerMaskDL("mm_mask_truth_c", "Mask of Truth", PMM_MODEL_TYPE_MASK_TRUTH, LINK_DL_MASK_TRUTH, sCallMaskTruthDL);
    registerMaskDL("mm_mask_kafei_c", "Kafei's Mask", PMM_MODEL_TYPE_MASK_KAFEIS_MASK, LINK_DL_MASK_KAFEIS_MASK, sCallMaskKafeiDL);
    registerMaskDL("mm_mask_all_night_c", "All Night Mask", PMM_MODEL_TYPE_MASK_ALL_NIGHT, LINK_DL_MASK_ALL_NIGHT, sCallMaskAllNightDL);
    registerMaskDL("mm_mask_bunny_hood_c", "Bunny Hood", PMM_MODEL_TYPE_MASK_BUNNY, LINK_DL_MASK_BUNNY, sCallMaskBunnyHoodDL);
    registerMaskDL("mm_mask_keaton_c", "Keaton Mask", PMM_MODEL_TYPE_MASK_KEATON, LINK_DL_MASK_KEATON, sCallMaskKeatonDL);
    registerMaskDL("mm_mask_garo_c", "Garo's Mask", PMM_MODEL_TYPE_MASK_GARO, LINK_DL_MASK_GARO, sCallMaskGaroDL);
    registerMaskDL("mm_mask_romani_c", "Romani's Mask", PMM_MODEL_TYPE_MASK_ROMANI, LINK_DL_MASK_ROMANI, sCallMaskRomaniDL);
    registerMaskDL("mm_mask_circus_leader_c", "Circus Leader's Mask", PMM_MODEL_TYPE_MASK_CIRCUS_LEADER, LINK_DL_MASK_CIRCUS_LEADER, sCallMaskCircusLeaderDL);
    registerMaskDL("mm_mask_postman_c", "Postman's Hat", PMM_MODEL_TYPE_MASK_POSTMAN, LINK_DL_MASK_POSTMAN, sCallMaskPostmanDL);
    registerMaskDL("mm_mask_couple_c", "Couple's Mask", PMM_MODEL_TYPE_MASK_COUPLE, LINK_DL_MASK_COUPLE, sCallMaskCoupleDL);
    registerMaskDL("mm_mask_great_fairy_c", "Great Fairy Mask", PMM_MODEL_TYPE_MASK_GREAT_FAIRY, LINK_DL_MASK_GREAT_FAIRY, sCallMaskGreatFairyDL);
    registerMaskDL("mm_mask_gibdo_c", "Gibdo Mask", PMM_MODEL_TYPE_MASK_GIBDO, LINK_DL_MASK_GIBDO, sCallMaskGibdoDL);
    registerMaskDL("mm_mask_don_gero_c", "Don Gero's Mask", PMM_MODEL_TYPE_MASK_DON_GERO, LINK_DL_MASK_DON_GERO, sCallMaskDonGeroDL);
    registerMaskDL("mm_mask_kamaro_c", "Kamaro's Mask", PMM_MODEL_TYPE_MASK_KAMARO, LINK_DL_MASK_KAMARO, sCallMaskKamaroDL);
    registerMaskDL("mm_mask_captain_c", "Captain's Hat", PMM_MODEL_TYPE_MASK_CAPTAIN, LINK_DL_MASK_CAPTAIN, sCallMaskCaptainDL);
    registerMaskDL("mm_mask_stone_c", "Stone Mask", PMM_MODEL_TYPE_MASK_STONE, LINK_DL_MASK_STONE, sCallMaskStoneDL);
    registerMaskDL("mm_mask_bremen_c", "Bremen Mask", PMM_MODEL_TYPE_MASK_BREMEN, LINK_DL_MASK_BREMEN, sCallMaskBremenDL);
    registerMaskDL("mm_mask_scents_c", "Mask of Scents", PMM_MODEL_TYPE_MASK_SCENTS, LINK_DL_MASK_SCENTS, sCallMaskScentsDL);
    registerMaskDL2("mm_mask_blast", "Blast Mask", PMM_MODEL_TYPE_MASK_BLAST, LINK_DL_MASK_BLAST, sCallMaskBlastDL, LINK_DL_MASK_BLAST_COOLING_DOWN, sCallMaskBlastCooldownDL);
    registerMaskDL("mm_mask_giant_c", "Giant's Mask", PMM_MODEL_TYPE_MASK_GIANT, LINK_DL_MASK_GIANT, sCallMaskGiantDL);
    registerMaskDL2("mm_mask_fierce_deity_c", "Fierce Deity's Mask", PMM_MODEL_TYPE_MASK_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY, sCallMaskFierceDeityDL, LINK_DL_MASK_FIERCE_DEITY_SCREAM, sCallMaskFierceDeityScreamDL);
    registerMaskDL2("mm_mask_goron_c", "Goron Mask", PMM_MODEL_TYPE_MASK_GORON, LINK_DL_MASK_GORON, sCallMaskGoronDL, LINK_DL_MASK_GORON_SCREAM, sCallMaskGoronScreamDL);
    registerMaskDL2("mm_mask_zora_c", "Zora Mask", PMM_MODEL_TYPE_MASK_ZORA, LINK_DL_MASK_ZORA, sCallMaskZoraDL, LINK_DL_MASK_ZORA_SCREAM, sCallMaskZoraScreamDL);
    registerMaskDL2("mm_mask_deku_c", "Deku Mask", PMM_MODEL_TYPE_MASK_DEKU, LINK_DL_MASK_DEKU, sCallMaskDekuDL, LINK_DL_MASK_DEKU_SCREAM, sCallMaskDekuScreamDL);
}
