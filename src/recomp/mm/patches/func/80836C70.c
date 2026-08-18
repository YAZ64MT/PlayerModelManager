#include "global.h"
#include "modding.h"
#include "modelmatrixids.h"
#include "formproxy.h"
#include "proxyactorext.h"

// Patch Deku Link petal spawn
RECOMP_PATCH void func_80836C70(PlayState *play, Player *player, PlayerBodyPart bodyPartIndex) {
    static Vec3f acceleration = {0.f, -0.2f, 0.f};

    extern Gfx object_link_nuts_DL_008860[];

    ObjectId objId = OBJECT_LINK_NUTS;
    Gfx *dl = object_link_nuts_DL_008860;
    s16 lifetime = 16;

    FormProxy *fp = ProxyActorExt_getFormProxy(&player->actor);
    if (fp) {
        objId = GAMEPLAY_KEEP;
        dl = FormProxy_getDL(fp, LINK_DL_PETAL_PARTICLE);
        PlayerProxyManager_setMinimumLifeTime(ProxyActorExt_getAppearanceDataHandleRaw(&player->actor), lifetime + 1);
    }

    for (int i = 0; i < 4; i++) {
        Vec3f velocity = {
            Rand_CenteredFloat(4.0f),
            Rand_ZeroFloat(2.0f),
            Rand_CenteredFloat(4.0f),
        };

        EffectSsHahen_Spawn(play, &player->bodyPartsPos[bodyPartIndex], &velocity, &acceleration, 0, 10, objId,
                            lifetime, dl);
    }
}
