#include "global.h"
#include "formproxy.h"
#include "proxyactorext.h"
#include "overlays/actors/ovl_En_Test3/z_en_test3.h"

void updatePlayerAssetsCommon(Player *player, FormProxy *fp, TexturePtr eyesTex[], TexturePtr mouthTex[]);

void updateAssets_on_EnTest3_Draw(EnTest3 *enTest3) {
    Player *player = &enTest3->player;

    FormProxy *fp = ProxyActorExt_getFormProxy(&player->actor);

    if (fp) {
        extern TexturePtr sEyeTextures_ovl_En_Test3[];
        extern TexturePtr sMouthTextures_ovl_En_Test3[];
        updatePlayerAssetsCommon(player, fp, sEyeTextures_ovl_En_Test3, sMouthTextures_ovl_En_Test3);
    }
}

void removeBackEquipment_on_return_EnTest3_OverrideLimbDraw(s32 limbIndex, Gfx **dList) {
    if (limbIndex == PLAYER_LIMB_SHEATH) {
        *dList = NULL;
    }
}
