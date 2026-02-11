#include "global.h"
#include "playerproxy.h"
#include "modelinfo.h"
#include "formproxy.h"
#include "logger.h"
#include "playermodelconfig.h"

PlayerProxy *gPlayer1Proxy;
PlayerProxy *gPlayer2Proxy;

ModelInfo gHumanModelInfoFallbackOverride;
ModelInfo gDekuModelInfoFallbackOverride;
ModelInfo gGoronModelInfoFallbackOverride;
ModelInfo gZoraModelInfoFallbackOverride;
ModelInfo gFierceDeityModelInfoFallbackOverride;
ModelInfo gHumanModelInfo;
ModelInfo gDekuModelInfo;
ModelInfo gGoronModelInfo;
ModelInfo gZoraModelInfo;
ModelInfo gFierceDeityModelInfo;

static U32HashsetHandle sValidPlayerProxies;

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid PlayerProxy pointer.")

static bool isValidPlayerProxy(const PlayerProxy *pp) {
    return recomputil_u32_hashset_contains(sValidPlayerProxies, (uintptr_t)pp);
}

FormProxy *PlayerProxy_getFormProxy(PlayerProxy *pp, FormProxyId formId) {
    if (!isValidPlayerProxy(pp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return recomputil_u32_memory_hashmap_get(pp->formProxies, formId);
}

void PlayerProxy_createFormProxy(PlayerProxy *pp, FormProxyId proxyId, PlayerTransformation form, ModelInfo *fallback, ModelInfo *fallbackOverride) {
    if (!isValidPlayerProxy(pp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    if (recomputil_u32_memory_hashmap_create(pp->formProxies, proxyId)) {
        FormProxy *fp = PlayerProxy_getFormProxy(pp, proxyId);
        FormProxy_init(fp, pp, form, proxyId, fallback, fallbackOverride);
    } else {
        Logger_printWarning("Tried to create a FormProxy, but key %d already existed!", proxyId);
    }
}

void PlayerProxy_init(PlayerProxy *pp) {
    if (!pp) {
        Logger_printWarning("Passed in a NULL pointer!");
        return;
    }

    if (isValidPlayerProxy(pp)) {
        Logger_printWarning("Tried to initialize a PlayerProxy twice!");
        return; // don't init twice
    }

    recomputil_u32_hashset_insert(sValidPlayerProxies, (uintptr_t)pp);

    pp->formProxies = recomputil_create_u32_memory_hashmap(sizeof(FormProxy));

    PlayerProxy_createFormProxy(pp, FORM_PROXY_ID_HUMAN, PLAYER_FORM_HUMAN, &gHumanModelInfo, &gHumanModelInfoFallbackOverride);
    PlayerProxy_createFormProxy(pp, FORM_PROXY_ID_DEKU, PLAYER_FORM_DEKU, &gDekuModelInfo, &gDekuModelInfoFallbackOverride);
    PlayerProxy_createFormProxy(pp, FORM_PROXY_ID_GORON, PLAYER_FORM_GORON, &gGoronModelInfo, &gGoronModelInfoFallbackOverride);
    PlayerProxy_createFormProxy(pp, FORM_PROXY_ID_ZORA, PLAYER_FORM_ZORA, &gZoraModelInfo, &gZoraModelInfoFallbackOverride);
    PlayerProxy_createFormProxy(pp, FORM_PROXY_ID_FIERCE_DEITY, PLAYER_FORM_FIERCE_DEITY, &gFierceDeityModelInfo, &gFierceDeityModelInfoFallbackOverride);

    FormProxy *human = PlayerProxy_getFormProxy(pp, FORM_PROXY_ID_HUMAN);
    FormProxy *deku = PlayerProxy_getFormProxy(pp, FORM_PROXY_ID_DEKU);
    FormProxy *goron = PlayerProxy_getFormProxy(pp, FORM_PROXY_ID_GORON);
    FormProxy *zora = PlayerProxy_getFormProxy(pp, FORM_PROXY_ID_ZORA);
    FormProxy *fd = PlayerProxy_getFormProxy(pp, FORM_PROXY_ID_FIERCE_DEITY);

    FormProxy *formProxies[] = {human, deku, goron, zora, fd};

    typedef struct FormAlternateDL {
        FormProxyId formProxyId;
        Link_DisplayList dlId;
    } FormAlternateDL;

#define DECLARE_FORM_ALT_DL(proxyId, linkDlId)   \
    {                                            \
        .formProxyId = proxyId, .dlId = linkDlId \
    }

    const FormAlternateDL formAlternates[] = {
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_DEKU, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_DEKU, LINK_DL_MASK_DEKU),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_DEKU, LINK_DL_MASK_DEKU_SCREAM),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_GORON, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_GORON),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_GORON, LINK_DL_MASK_GORON),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_GORON, LINK_DL_MASK_GORON_SCREAM),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_MASK_ZORA),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_MASK_ZORA_SCREAM),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_FIERCE_DEITY, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_FIERCE_DEITY),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY),
        DECLARE_FORM_ALT_DL(FORM_PROXY_ID_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY_SCREAM),
    };

    for (int i = 0; i < ARRAY_COUNT(formProxies); ++i) {
        FormProxy *currFp = formProxies[i];

        if (currFp) {
            for (int j = 0; j < ARRAY_COUNT(formAlternates); ++j) {
                const FormAlternateDL *currAlt = &formAlternates[j];

                if (FormProxy_getFormProxyId(currFp) != currAlt->formProxyId) {
                    FormProxy_setAlternateFormProxyDL(currFp, currAlt->dlId, PlayerProxy_getFormProxy(pp, currAlt->formProxyId));
                }
            }
        }
    }
}

void PlayerProxy_refresh(PlayerProxy *pp) {
    for (size_t i = 0; i < gFormProxyIds->size; ++i) {
        FormProxy *currFp = PlayerProxy_getFormProxy(pp, gFormProxyIds->ids[i]);

        if (currFp) {
            FormProxy_requestRefresh(currFp);
        }
    }
}

void PlayerProxy_setOverrideDL(PlayerProxy *pp, Link_DisplayList dlId, Gfx *dl) {
    for (size_t i = 0; i < gFormProxyIds->size; ++i) {
        FormProxy *currFp = PlayerProxy_getFormProxy(pp, gFormProxyIds->ids[i]);

        if (currFp) {
            FormProxy_setCurrentOverrideDL(currFp, dlId, dl);
        }
    }
}

void PlayerProxy_setOverrideMtx(PlayerProxy *pp, Link_EquipmentMatrix mtxId, Mtx *mtx) {
    for (size_t i = 0; i < gFormProxyIds->size; ++i) {
        FormProxy *currFp = PlayerProxy_getFormProxy(pp, gFormProxyIds->ids[i]);

        if (currFp) {
            FormProxy_setCurrentOverrideMtx(currFp, mtxId, mtx);
        }
    }
}

bool PlayerProxy_getProxyIdFromForm(PlayerTransformation form, FormProxyId *out) {
    if (!out) {
        return false;
    }

    switch (form) {
        case PLAYER_FORM_FIERCE_DEITY:
            *out = FORM_PROXY_ID_FIERCE_DEITY;
            break;

        case PLAYER_FORM_GORON:
            *out = FORM_PROXY_ID_GORON;
            break;

        case PLAYER_FORM_ZORA:
            *out = FORM_PROXY_ID_ZORA;
            break;

        case PLAYER_FORM_DEKU:
            *out = FORM_PROXY_ID_DEKU;
            break;

        case PLAYER_FORM_HUMAN:
            *out = FORM_PROXY_ID_HUMAN;
            break;

        default:
            return false;
            break;
    }

    return true;
}

void PlayerProxy_requestTunicColorOverride(PlayerProxy *pp, Color_RGBA8 color) {
    for (size_t i = 0; i < gFormProxyIds->size; ++i) {
        FormProxy *currFp = PlayerProxy_getFormProxy(pp, gFormProxyIds->ids[i]);

        if (currFp) {
            FormProxy_requestTunicColorOverride(currFp, color);
        }
    }
}

RECOMP_CALLBACK(".", _internal_initHashObjects) void initPlayerProxyHash(void) {
    sValidPlayerProxies = recomputil_create_u32_hashset();
}

RECOMP_CALLBACK(".", _internal_postInitHashObjects) void initPlayerProxyFallbacks(void) {
    ModelInfo_init(&gHumanModelInfoFallbackOverride);
    ModelInfo_init(&gDekuModelInfoFallbackOverride);
    ModelInfo_init(&gGoronModelInfoFallbackOverride);
    ModelInfo_init(&gZoraModelInfoFallbackOverride);
    ModelInfo_init(&gFierceDeityModelInfoFallbackOverride);
    ModelInfo_init(&gHumanModelInfo);
    ModelInfo_init(&gDekuModelInfo);
    ModelInfo_init(&gGoronModelInfo);
    ModelInfo_init(&gZoraModelInfo);
    ModelInfo_init(&gFierceDeityModelInfo);
}
