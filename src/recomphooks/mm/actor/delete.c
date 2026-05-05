#include "global.h"
#include "modding.h"

static ActorContext *sActorCtx;
static Actor *sActor;
static PlayState *sPlay;

void clearProxyExt_on_Actor_Delete(Actor *actor);

RECOMP_HOOK("Actor_Delete") void on_Actor_Delete(ActorContext *actorCtx, Actor *actor, PlayState *play) {
    sActorCtx = actorCtx;
    sActor = actor;
    sPlay = play;

    clearProxyExt_on_Actor_Delete(sActor);
}

RECOMP_HOOK_RETURN("Actor_Delete") void on_return_Actor_Delete(void) {

}
