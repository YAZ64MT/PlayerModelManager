#include "global.h"
#include "recomputils.h"
#include "modelreplacer_api.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

RECOMP_CALLBACK(YAZMT_Z64_MODEL_REPLACER_MOD_NAME, ZModelReplacer_onReady)
void reserveDekuStick() {
    ZModelReplacer_preregisterDL(GAMEPLAY_KEEP, &gDekuStickDL);
}
