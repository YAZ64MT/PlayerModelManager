#include "global.h"
#include "recomputils.h"
#include "modelreplacer_api.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

RECOMP_CALLBACK(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, ZModelReplacer_onReady)
void reserveGlassBottle() {
    ZModelReplacer_preregisterDL(GAMEPLAY_KEEP, gBottleGlassDL);
    ZModelReplacer_preregisterDL(GAMEPLAY_KEEP, gBottleContentsDL);
}
