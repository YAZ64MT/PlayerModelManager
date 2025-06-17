#ifndef __MODEL_COMMON_H__
#define __MODEL_COMMON_H__

#include "playermodelmanager.h"

#define GET_PLAYER_FORM_PROXY gLinkFormProxies[GET_PLAYER_FORM]

extern Gfx gEmptyDisplayList[];
extern Gfx gCallEmptyDisplayList[];
extern Gfx gPopModelViewMtx[];
extern Mtx gZeroMtx;
extern Link_FormProxy gLinkFormProxies[];
extern bool gIsAgePropertyRefreshRequested;

void matchFaceTexturesToProxy(Link_FormProxy *formProxy);
void refreshFaceTextures();
void initFormProxy(Link_FormProxy *formProxy);
void requestRefreshFormProxy(PlayerTransformation form);
Gfx *getFormProxyDL(Link_FormProxy *formProxy, Link_DisplayList target);

#endif
