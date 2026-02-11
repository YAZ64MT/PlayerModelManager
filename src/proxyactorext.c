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
    FormProxyId *fpIdPtr = z64recomp_get_extended_actor_data(actor, sActorExtIdFormProxyId);

    if (fpIdPtr) {
        *fpIdPtr = id;
        return true;
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

FormProxy *ProxyActorExt_getFormProxyOrFallback(Actor *actor, FormProxyId fallbackId, PlayState *play) {
    FormProxy *fp = ProxyActorExt_getFormProxy(actor);

    if (!fp) {
        fp = ProxyActorExt_getFormProxy(&(GET_PLAYER(play)->actor));
    }

    if (!fp) {
        fp = PlayerProxy_getFormProxy(gPlayer1Proxy, fallbackId);
    }

    return fp;
}

RECOMP_CALLBACK(".", _internal_preInitHashObjects) void handleFormProxyExtensionInits(void) {
    sActorExtIdPlayerProxy = z64recomp_extend_actor_all(sizeof(PlayerProxy *));
    sActorExtIdFormProxyId = z64recomp_extend_actor_all(sizeof(FormProxyId));
}

void initFormProxyExtension_on_Actor_Init(Actor *actor) {
    ProxyActorExt_setPlayerProxy(actor, NULL);
    ProxyActorExt_setFormProxyId(actor, FORM_PROXY_ID_NONE);
}

void setupPlayerFormProxy_on_Player_InitCommon(Player *player) {
    if (player->transformation < PLAYER_FORM_MAX) {
        // TODO: CREATE AN EXPORT FOR OVERRIDING PLAYER PROXY
        FormProxyId id;

        if (PlayerProxy_getProxyIdFromForm(player->transformation, &id)) {
            PlayerProxy *proxy = NULL;
            if (player->actor.id == ACTOR_PLAYER) {
                proxy = gPlayer1Proxy;
            } else if (player->actor.id == ACTOR_EN_TEST3) {
                proxy = gPlayer2Proxy;
            }

            if (proxy) {
                ProxyActorExt_setPlayerProxy(&player->actor, proxy);
                ProxyActorExt_setFormProxyId(&player->actor, id);
            }
        }
    }
}
