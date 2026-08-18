#include "global.h"
#include "modding.h"
#include "overlays/actors/ovl_En_Test3/z_en_test3.h"

static PlayState *sPlay;
static s32 sLimbIndex;
static Gfx **sDList1;
static Gfx **sDList2;
static Vec3s *sRot;
static EnTest3 *sEnTest3;

void handleTunicColor_on_Player_PostLimbDrawGameplay(PlayState *play);
void handleMaskMtx_on_Player_PostLimbDrawGameplay(PlayState *play, s32 limbIndex, Gfx **dList1, Player *player);
void hookGfx_on_EnTest3_PostLimbDraw(PlayState *play, EnTest3 *enTest3);

RECOMP_HOOK("EnTest3_PostLimbDraw") void on_EnTest3_PostLimbDraw(PlayState *play, s32 limbIndex, Gfx **dList1, Gfx **dList2, Vec3s *rot, Actor *thisx) {
    sPlay = play;
    sLimbIndex = limbIndex;
    sDList1 = dList1;
    sDList2 = dList2;
    sRot = rot;
    sEnTest3 = (EnTest3 *)thisx;

    handleTunicColor_on_Player_PostLimbDrawGameplay(sPlay);
    handleMaskMtx_on_Player_PostLimbDrawGameplay(sPlay, sLimbIndex, sDList1, &sEnTest3->player);
    hookGfx_on_EnTest3_PostLimbDraw(sPlay, sEnTest3);
}

void handleTunicColor_on_return_Player_PostLimbDrawGameplay(PlayState *play);
void handleMaskMtx_on_return_Player_PostLimbDrawGameplay(void);
void hookGfx_on_return_EnTest3_PostLimbDraw(EnTest3 *enTest3);

RECOMP_HOOK_RETURN("EnTest3_PostLimbDraw") void on_return_EnTest3_PostLimbDraw(void) {
    handleTunicColor_on_return_Player_PostLimbDrawGameplay(sPlay);
    handleMaskMtx_on_return_Player_PostLimbDrawGameplay();
    hookGfx_on_return_EnTest3_PostLimbDraw(sEnTest3);
}
