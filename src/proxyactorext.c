#include "global.h"
#include "modding.h"
#include "z64recomp_api.h"
#include "playerproxy.h"
#include "formproxy.h"
#include "proxyactorext.h"
#include "playermodelconfig.h"
#include "logger.h"

typedef struct PlayerProxyInfo {
    PlayerProxy *pp;
    FormProxyId id;
} PlayerProxyInfo;

static ActorExtensionId sActorExtIdPlayerProxyInfo;

static PlayerProxyInfo *getPlayerProxyInfo(Actor *actor) {
    if (!actor) {
        return NULL;
    }

    return z64recomp_get_extended_actor_data(actor, sActorExtIdPlayerProxyInfo);
}

PlayerProxy *ProxyActorExt_getPlayerProxy(Actor *actor) {
    PlayerProxyInfo *proxyInfo = getPlayerProxyInfo(actor);

    if (proxyInfo) {
        return proxyInfo->pp;
    }

    return NULL;
}

bool ProxyActorExt_getFormProxyId(Actor *actor, FormProxyId *out) {
    PlayerProxyInfo *proxyInfo = getPlayerProxyInfo(actor);

    if (proxyInfo) {
        *out = proxyInfo->id;
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
    PlayerProxyInfo *proxyInfo = getPlayerProxyInfo(actor);

    if (proxyInfo) {
        proxyInfo->id = id;
        return true;
    }

    return false;
}

bool ProxyActorExt_setPlayerProxy(Actor *actor, PlayerProxy *pp) {
    PlayerProxyInfo *proxyInfo = getPlayerProxyInfo(actor);

    if (proxyInfo) {
        proxyInfo->pp = pp;
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
    sActorExtIdPlayerProxyInfo = z64recomp_extend_actor_all(sizeof(PlayerProxyInfo));
}

static void resetProxyInfo(PlayerProxyInfo *proxyInfo) {
    proxyInfo->id = FORM_PROXY_ID_NONE;
    proxyInfo->pp = NULL;
}

void PlayerProxyInfo_init(PlayerProxyInfo *proxyInfo) {
    resetProxyInfo(proxyInfo);
}

void PlayerProxyInfo_destroy(PlayerProxyInfo *proxyInfo) {
    resetProxyInfo(proxyInfo);
}

void initFormProxyExtension_on_Actor_Init(Actor *actor) {
    PlayerProxyInfo *proxyInfo = getPlayerProxyInfo(actor);

    if (proxyInfo) {
        PlayerProxyInfo_init(proxyInfo);
    }
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

void clearProxyExt_on_Actor_Destroy(Actor *actor) {
    PlayerProxyInfo *proxyInfo = getPlayerProxyInfo(actor);

    if (proxyInfo) {
        PlayerProxyInfo_destroy(proxyInfo);
    }
}
