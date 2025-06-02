#ifndef __MODEL_COMMON_H__
#define __MODEL_COMMON_H__

#include "playermodelmanager.h"

#define GET_PLAYER_FORM_PROXY gLinkFormProxies[GET_PLAYER_FORM]

extern Gfx dfCommand[];
extern Gfx callDfCommand[];
extern Gfx popModelViewMtx[];
extern Mtx zeroMtx;
extern Link_FormProxy gLinkFormProxies[];
extern bool gIsAgePropertyRefreshRequested;

void matchFaceTexturesToProxy(Link_FormProxy *formProxy);
void initFormProxy(Link_FormProxy *formProxy);
void refreshFormProxy(Link_FormProxy *formProxy);
void loadVanillaSkeletonTransforms(Link_ModelInfo *modelInfo, ObjectId objectId, void **limbs);

#endif
