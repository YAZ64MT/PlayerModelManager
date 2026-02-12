#include "global.h"
#include "formproxy.h"
#include "proxyactorext.h"
#include "overlays/actors/ovl_En_Test3/z_en_test3.h"

void updateAssets_on_EnTest3_Draw(EnTest3 *enTest3) {
    Player *player = &enTest3->player;

    FormProxy *fp = ProxyActorExt_getFormProxy(&player->actor);

    if (fp) {
        extern TexturePtr sEyeTextures_ovl_En_Test3[];
        extern TexturePtr sMouthTextures_ovl_En_Test3[];
        FormProxy_repointPlayerFaceTexturePtrs(fp, sEyeTextures_ovl_En_Test3, sMouthTextures_ovl_En_Test3);
        FlexSkeletonHeader *skel = FormProxy_getSkeleton(fp);
        if (skel) {
            player->skelAnime.dListCount = player->skelAnimeUpper.dListCount = skel->dListCount;
            player->skelAnime.skeleton = player->skelAnimeUpper.skeleton = skel->sh.segment;
        }

        if (player->transformation == PLAYER_FORM_GORON) {
            FlexSkeletonHeader *shieldingSkel = FormProxy_getShieldingSkeleton(fp);

            if (shieldingSkel) {
                player->unk_2C8.dListCount = shieldingSkel->dListCount;
                player->unk_2C8.skeleton = shieldingSkel->sh.segment;
            }
        }
    }
}

void removeBackEquipment_on_return_EnTest3_OverrideLimbDraw(s32 limbIndex, Gfx **dList) {
    if (limbIndex == PLAYER_LIMB_SHEATH) {
        *dList = NULL;
    }
}
