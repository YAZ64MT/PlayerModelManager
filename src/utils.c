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

void clearModelInfoKeepEyes(Link_ModelInfo *info) {

    TexturePtr eyesTex[PLAYER_EYES_MAX];
    TexturePtr mouthTex[PLAYER_MOUTH_MAX];

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        eyesTex[i] = info->eyesTextures[i];
    }

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        mouthTex[i] = info->mouthTextures[i];
    }

    clearLinkModelInfo(info);

    for (int i = 0; i < PLAYER_EYES_MAX; ++i) {
        info->eyesTextures[i] = eyesTex[i];
    }

    for (int i = 0; i < PLAYER_MOUTH_MAX; ++i) {
        info->mouthTextures[i] = mouthTex[i];
    }
}

Gfx *createShimDisplayList(int displayListCount, ...) {
    if (displayListCount < 1) {
        return NULL;
    }

    Gfx *shimDl = recomp_alloc(sizeof(Gfx) * displayListCount);

    va_list args;
    va_start(args, displayListCount);

    for (int i = 0; i < displayListCount - 1; ++i) {
        Gfx *dlToCall = va_arg(args, Gfx *);

        gSPDisplayList(&shimDl[i], dlToCall);
    }

    Gfx *lastDl = va_arg(args, Gfx *);

    gSPBranchList(&shimDl[displayListCount - 1], lastDl);

    va_end(args);

    return shimDl;
}

Gfx *createShimWithMatrix(Mtx* matrix, int displayListCount, ...) {
    if (displayListCount < 1) {
        return NULL;
    }

    u32 shimSize = displayListCount + 3;

    Gfx *shimDl = recomp_alloc(sizeof(Gfx) * (shimSize));

    gSPMatrix(shimDl, matrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    va_list args;
    va_start(args, displayListCount);

    Gfx *calledLists = shimDl + 1;
    for (int i = 0; i < displayListCount; ++i) {
        Gfx *dlToCall = va_arg(args, Gfx *);

        gSPDisplayList(&calledLists[i], dlToCall);
    }

    va_end(args);

    gSPPopMatrix(&shimDl[shimSize - 2], G_MTX_MODELVIEW);

    gSPEndDisplayList(&shimDl[shimSize - 1]);

    return shimDl;
}

u32 readU32(const u8 array[], u32 offset) {
    return (u32)(array[offset + 0]) << 24 |
           (u32)(array[offset + 1]) << 16 |
           (u32)(array[offset + 2]) << 8 |
           (u32)(array[offset + 3]);
}

void writeU32(u8 array[], u32 offset, u32 value) {
    array[offset + 0] = (value & 0xFF000000) >> 24;
    array[offset + 1] = (value & 0x00FF0000) >> 16;
    array[offset + 2] = (value & 0x0000FF00) >> 8;
    array[offset + 3] = (value & 0x000000FF);
}

bool isSegmentedPtr(void *p) {
    return (uintptr_t)p >> 24 <= 0xF;
}
