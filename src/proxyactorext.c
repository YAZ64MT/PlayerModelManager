#include "global.h"
#include "modding.h"
#include "z64recomp_api.h"
#include "playerproxy.h"
#include "formproxy.h"
#include "proxyactorext.h"
#include "playermodelconfig.h"
#include "logger.h"

static ActorExtensionId sActorExtIdPlayerProxy;
static ActorExtensionId sActorExtIdFormProxyId;

PlayerProxy *ProxyActorExt_getPlayerProxy(Actor *actor) {
    PlayerProxy **actorPp = z64recomp_get_extended_actor_data(actor, sActorExtIdPlayerProxy);

    if (actorPp) {
        return *actorPp;
    }

    return NULL;
}

bool ProxyActorExt_getFormProxyId(Actor *actor, FormProxyId *out) {
    FormProxyId *fpIdPtr = z64recomp_get_extended_actor_data(actor, sActorExtIdFormProxyId);

    if (fpIdPtr) {
        *out = *fpIdPtr;
        return true;
    }

    return false;
}

FormProxy *ProxyActorExt_getFormProxy(Actor *actor) {
    PlayerProxy *actorPp = ProxyActorExt_getPlayerProxy(actor);

    if (actorPp) {
        FormProxyId fpId;

        if (ProxyActorExt_getFormProxyId(actor, &fpId)) {
            return PlayerProxy_getFormProxy(actorPp, fpId);
        }
    }

    return NULL;
}

bool ProxyActorExt_setFormProxyId(Actor *actor, FormProxyId id) {
    if (id < PlayerModelConfig_getNumFormIds()) {
        FormProxyId *fpIdPtr = z64recomp_get_extended_actor_data(actor, sActorExtIdFormProxyId);

        if (fpIdPtr) {
            *fpIdPtr = id;
            return true;
        }
    }

    return false;
}

bool ProxyActorExt_setPlayerProxy(Actor *actor, PlayerProxy *pp) {
    PlayerProxy **actorPp = z64recomp_get_extended_actor_data(actor, sActorExtIdPlayerProxy);

    if (actorPp) {
        *actorPp = pp;
        return true;
    }

    return false;
}

bool ProxyActorExt_copyProxyInformation(Actor *dest, Actor *src) {
    if (dest && src) {
        FormProxyId fpId;

        if (ProxyActorExt_getFormProxyId(src, &fpId)) {
            PlayerProxy *pp = ProxyActorExt_getPlayerProxy(src);

            if (pp) {
                ProxyActorExt_setPlayerProxy(dest, pp);
                ProxyActorExt_setFormProxyId(dest, fpId);
                return true;
            }
        }
    }

    return false;
}

RECOMP_CALLBACK(".", _internal_preInitHashObjects)
void handleFormProxyExtensionInits() {
    sActorExtIdPlayerProxy = z64recomp_extend_actor_all(sizeof(PlayerProxy *));
    sActorExtIdFormProxyId = z64recomp_extend_actor_all(sizeof(FormProxyId));
}

RECOMP_HOOK("Actor_Init")
void initFormProxyExtension_on_Actor_Init(Actor *actor, PlayState *play) {
    ProxyActorExt_setPlayerProxy(actor, NULL);
    ProxyActorExt_setFormProxyId(actor, PlayerModelConfig_getNumFormIds());
}

RECOMP_HOOK("Player_Init")
void setupPlayerFormProxy_on_Player_Init(Actor *thisx, PlayState *play) {
    Player *player = (Player *)thisx;

    PlayerTransformation form;
    if (player->actor.shape.rot.x != 0) {
        form = player->actor.shape.rot.x - 1;
    } else {
        form = GET_PLAYER_FORM;
    }

    if (form < PLAYER_FORM_MAX) {
        // TODO: CREATE AN EXPORT FOR OVERRIDING PLAYER PROXY
        FormProxyId id;

        if (PlayerProxy_getProxyIdFromForm(form, &id)) {
            ProxyActorExt_setPlayerProxy(thisx, gPlayer1Proxy);
            ProxyActorExt_setFormProxyId(thisx, id);
        }
        FormProxy *fp = ProxyActorExt_getFormProxy(thisx);

        // Replace player skeleton before init
        if (fp) {
            FlexSkeletonHeader *skel = FormProxy_getSkeleton(fp);
            if (skel) {
                gPlayerSkeletons[form] = skel;
            }
        }
    }
}

static Player *sPlayerInitCommonGoron;

RECOMP_HOOK("Player_InitCommon")
void replaceGoronShieldingSkel_on_Player_InitCommon(Player *this, PlayState *play, FlexSkeletonHeader *skelHeader) {
    sPlayerInitCommonGoron = this->transformation == PLAYER_FORM_GORON ? this : NULL;
}

RECOMP_HOOK_RETURN("Player_InitCommon")
void replaceGoronShieldingSkel_on_return_Player_InitCommon() {
    if (sPlayerInitCommonGoron) {
        FormProxy *fp = ProxyActorExt_getFormProxy(&sPlayerInitCommonGoron->actor);

        if (fp) {
            FlexSkeletonHeader *shieldingSkel = FormProxy_getShieldingSkeleton(fp);

            if (shieldingSkel && shieldingSkel->sh.segment) {
                sPlayerInitCommonGoron->unk_2C8.skeleton = shieldingSkel->sh.segment;
            }
        }
    }
}
