#include "global.h"
#include "modding.h"
#include "modelmatrixids.h"
#include "modelentry.h"
#include "proxyactorext.h"
#include "formproxy.h"
#include "playerproxy.h"
#include "utils.h"
#include "logger.h"
#include "modelentrymanager.h"
#include "apilocal.h"

RECOMP_EXPORT bool PlayerModelManager_Actor_isModelApplied(Actor *actor, PlayerModelManagerHandle h) {
    const ModelEntry *entryToCheck = ModelEntryManager_getEntry(h);

    if (entryToCheck) {
        PlayerModelManagerModelType type = ModelEntry_getType(entryToCheck);

        if (Utils_isFormModelType(type)) {
            FormProxy *fp = ProxyActorExt_getFormProxy(actor);

            if (fp) {
                ModelEntryForm *entryForm = FormProxy_getModelEntryForm(fp);

                return entryForm && ModelEntryForm_getModelEntry(entryForm) == entryToCheck;
            }
        } else if (Utils_isEquipmentModelType(type)) {
            PlayerProxy *pp = ProxyActorExt_getPlayerProxy(actor);

            if (pp) {
                return PlayerProxy_isModelEntryApplied(pp, entryToCheck);
            }
        }
    }

    return false;
}

RECOMP_EXPORT Gfx *PlayerModelManager_Actor_getDisplayList(Actor *actor, Link_DisplayList dlId) {
    if (!Utils_isValidDisplayListId(dlId)) {
        Logger_printError("Requested invalid display list ID %d!", dlId);
        return NULL;
    }

    FormProxy *fp = ProxyActorExt_getFormProxy(actor);

    if (fp) {
        return FormProxy_getDL(fp, dlId);
    }

    return NULL;
}

RECOMP_EXPORT Mtx *PlayerModelManager_Actor_getMatrix(Actor *actor, Link_EquipmentMatrix mtxId) {
    if (!Utils_isValidMatrixId(mtxId)) {
        Logger_printError("Requested invalid matrix ID %d!", mtxId);
        return NULL;
    }

    FormProxy *fp = ProxyActorExt_getFormProxy(actor);

    if (fp) {
        return FormProxy_getMatrix(fp, mtxId);
    }

    return NULL;
}

RECOMP_EXPORT bool PlayerModelManager_Actor_hasAppearanceData(Actor *actor) {
    return ProxyActorExt_isActorHasAppearanceData(actor);
}

RECOMP_EXPORT bool PlayerModelManager_Actor_setFormModelType(Actor *actor, PlayerModelManagerModelType type) {
    if (Utils_isFormModelType(type)) {
        FormProxyId id;

        switch (type) {
            case PMM_MODEL_TYPE_CHILD:
            case PMM_MODEL_TYPE_ADULT:
                id = FORM_PROXY_ID_HUMAN;
                break;

            case PMM_MODEL_TYPE_GORON:
                id = FORM_PROXY_ID_GORON;
                break;

            case PMM_MODEL_TYPE_DEKU:
                id = FORM_PROXY_ID_DEKU;
                break;

            case PMM_MODEL_TYPE_ZORA:
                id = FORM_PROXY_ID_ZORA;
                break;

            case PMM_MODEL_TYPE_FIERCE_DEITY:
                id = FORM_PROXY_ID_FIERCE_DEITY;
                break;

            default:
                id = FORM_PROXY_ID_HUMAN;
                break;
        }

        return ProxyActorExt_setFormProxyId(actor, id);
    }

    return false;
}

RECOMP_EXPORT bool PlayerModelManager_Actor_getTunicColor(Actor *actor, Color_RGBA8 *out) {
    FormProxy *fp = ProxyActorExt_getFormProxy(actor);

    if (out && fp) {
        *out = FormProxy_getCurrentTunicColor(fp);
        return true;
    }

    return false;
}

RECOMP_EXPORT PlayerModelManagerModelType PlayerModelManager_Actor_getFormModelType(Actor *actor) {
    FormProxy *fp = ProxyActorExt_getFormProxy(actor);

    if (fp) {
        return FormProxy_getModelType(fp);
    }

    return PMM_MODEL_TYPE_NONE;
}

RECOMP_EXPORT bool PlayerModelManager_Actor_getModelName(Actor *actor, PlayerModelManagerModelType type, char outBuf[], size_t outBufSize) {
    bool ret = false;

    PlayerProxyHandle h = ProxyActorExt_getAppearanceDataHandleCopy(actor);
    if (h) {
        ret = PlayerModelManager_AppearanceData_getModelName(h, type, outBuf, outBufSize);
        PlayerProxyManager_releaseReference(h);
    }

    return ret;
}

RECOMP_EXPORT ActorAppearanceDataHandle PlayerModelManager_Actor_getAppearanceDataHandle(Actor *actor) {
    return ProxyActorExt_getAppearanceDataHandleCopy(actor);
}
