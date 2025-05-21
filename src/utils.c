#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "libc/stdarg.h"
#include "playermodelmanager.h"
#include "playermodelmanager_utils.h"
#include "libc/string.h"

void clearLinkModelInfo(Link_ModelInfo* modelInfo) {
    memset(modelInfo, 0, sizeof(Link_ModelInfo));
}

Gfx *createShimDisplayList(u32 displayListCount, ...) {
    Gfx *shimDl = recomp_alloc(sizeof(Gfx) * displayListCount);

    va_list args;
    va_start(args, displayListCount);

    for (u32 i = 0; i < displayListCount - 1; ++i) {
        Gfx *dlToCall = va_arg(args, Gfx *);

        gSPDisplayList(&shimDl[i], dlToCall);
    }

    Gfx *lastDl = va_arg(args, Gfx *);

    gSPBranchList(&shimDl[displayListCount - 1], lastDl);

    va_end(args);

    return shimDl;
}

Gfx *createShimWithMatrix(Mtx* matrix, u32 displayListCount, ...) {
    u32 shimSize = displayListCount + 3;

    Gfx *shimDl = recomp_alloc(sizeof(Gfx) * (shimSize));

    gSPMatrix(shimDl, matrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    va_list args;
    va_start(args, displayListCount);

    Gfx *calledLists = shimDl + 1;
    for (u32 i = 0; i < displayListCount; ++i) {
        Gfx *dlToCall = va_arg(args, Gfx *);

        gSPDisplayList(&calledLists[i], dlToCall);
    }

    va_end(args);

    gSPPopMatrix(&shimDl[shimSize - 2], G_MTX_MODELVIEW);

    gSPEndDisplayList(&shimDl[shimSize - 1]);

    return shimDl;
}
