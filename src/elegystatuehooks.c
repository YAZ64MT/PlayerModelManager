#include "global.h"
#include "proxyactorext.h"
#include "playerproxy.h"
#include "formproxy.h"
#include "overlays/actors/ovl_En_Torch2/z_en_torch2.h"

void setupElegyStatueProxy_on_return_func_80848640(PlayState *play, Player *player) {
    EnTorch2 *elegyStatue = play->actorCtx.elegyShells[player->transformation];

    if (elegyStatue) {
        ProxyActorExt_copyProxyInformation(&elegyStatue->actor, &player->actor);
    }
}

void replaceElegyStatue_on_EnTorch2_Draw(Actor *actor, PlayState *play) {
    static Link_DisplayList formToDLid[] = {
        [PLAYER_FORM_HUMAN] = LINK_DL_ELEGY_OF_EMPTINESS_SHELL_HUMAN,
        [PLAYER_FORM_DEKU] = LINK_DL_ELEGY_OF_EMPTINESS_SHELL_DEKU,
        [PLAYER_FORM_GORON] = LINK_DL_ELEGY_OF_EMPTINESS_SHELL_GORON,
        [PLAYER_FORM_ZORA] = LINK_DL_ELEGY_OF_EMPTINESS_SHELL_ZORA,
        [PLAYER_FORM_FIERCE_DEITY] = LINK_DL_ELEGY_OF_EMPTINESS_SHELL_FIERCE_DEITY,
    };

    PlayerTransformation form = actor->params;

    FormProxyId formId;

    if (!PlayerProxy_getProxyIdFromForm(form, &formId)) {
        formId = FORM_PROXY_ID_HUMAN;
    }

    FormProxy *fp = ProxyActorExt_getFormProxyOrFallback(actor, formId, play);

    if (fp) {
        extern Gfx *sShellDLists[];

        Gfx *dl = FormProxy_getDL(fp, formToDLid[form]);

        if (dl) {
            sShellDLists[form] = dl;
        }
    }
}
