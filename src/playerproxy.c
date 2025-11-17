#include "global.h"
#include "playerproxy.h"
#include "model_common.h"
#include "logger.h"

ModelInfo gHumanModelInfoFallbackOverride;
ModelInfo gDekuModelInfoFallbackOverride;
ModelInfo gGoronModelInfoFallbackOverride;
ModelInfo gZoraModelInfoFallbackOverride;
ModelInfo gFierceDeityModelInfoFallbackOverride;

static U32HashsetHandle sValidPlayerProxies;

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid PlayerProxy pointer.")

static bool isValidPlayerProxy(const PlayerProxy *pp) {
    return recomputil_u32_hashset_contains(sValidPlayerProxies, (uintptr_t)pp);
}

FormProxy *PlayerProxy_getFormProxy(const PlayerProxy *pp, FormProxyId formId) {
    if (!isValidPlayerProxy(pp)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return recomputil_u32_memory_hashmap_get(pp->formProxies, formId);
}

typedef struct FormAlternateDL {
    FormProxyId formProxyId;
    Link_DisplayList dlId;
} FormAlternateDL;

#define DECLARE_FORM_ALT_DL(proxyForm, linkDlId) {.formProxyId = proxyForm, .dlId = linkDlId}

static const FormAlternateDL sFormAlternates[] = {
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_DEKU, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_DEKU, LINK_DL_MASK_DEKU),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_DEKU, LINK_DL_MASK_DEKU_SCREAM),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_GORON, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_GORON),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_GORON, LINK_DL_MASK_GORON),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_GORON, LINK_DL_MASK_GORON_SCREAM),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_MASK_ZORA),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_MASK_ZORA_SCREAM),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_LFIN_BOOMERANG),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_ZORA, LINK_DL_RFIN_BOOMERANG),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_FIERCE_DEITY, LINK_DL_ELEGY_OF_EMPTINESS_SHELL_FIERCE_DEITY),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY),
    DECLARE_FORM_ALT_DL(FORM_PROXY_ID_FIERCE_DEITY, LINK_DL_MASK_FIERCE_DEITY_SCREAM),
};

void PlayerProxy_createFormProxy(PlayerProxy *pp, FormProxyId proxyId, PlayerTransformation form, ModelInfo *fallback, ModelInfo *fallbackOverride) {
    if (!isValidPlayerProxy(pp)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    if (!recomputil_u32_memory_hashmap_create(pp->formProxies, proxyId)) {
        Logger_printWarning("Tried to create a FormProxy, but key %d already existed!", form);

        FormProxy *fp = PlayerProxy_getFormProxy(pp, form);

        FormProxy_init(fp, form, proxyId, fallback, fallbackOverride);
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

    for (int i = 0; i < ARRAY_COUNT(formProxies); ++i) {
        FormProxy *currFp = formProxies[i];

        if (currFp) {
            for (int j = 0; j < ARRAY_COUNT(sFormAlternates); ++j) {
                const FormAlternateDL *currAlt = &sFormAlternates[j];

                if (FormProxy_getFormProxyId(currFp) != currAlt->formProxyId) {
                    FormProxy_setAlternateFormProxyDL(currFp, currAlt->dlId, PlayerProxy_getFormProxy(pp, currAlt->formProxyId));
                }
            }
        }
    }
}

RECOMP_CALLBACK(".", _internal_initHashObjects)
void initPlayerProxyHash() {
    sValidPlayerProxies = recomputil_create_u32_hashset();
}

RECOMP_CALLBACK(".", _internal_postInitHashObjects)
void initPlayerProxyFallbacks() {
    ModelInfo_init(&gHumanModelInfoFallbackOverride);
    ModelInfo_init(&gDekuModelInfoFallbackOverride);
    ModelInfo_init(&gGoronModelInfoFallbackOverride);
    ModelInfo_init(&gZoraModelInfoFallbackOverride);
    ModelInfo_init(&gFierceDeityModelInfoFallbackOverride);
}
