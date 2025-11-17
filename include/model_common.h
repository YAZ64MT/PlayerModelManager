#ifndef __MODEL_COMMON_H__
#define __MODEL_COMMON_H__

#include "playermodelmanager.h"
#include "formproxy.h"
#include "modelinfo.h"

#define GET_PLAYER_FORM_PROXY (&gLinkFormProxies[GET_PLAYER_FORM])

extern Gfx gPopModelViewMtx[];
extern Link_FormProxy gLinkFormProxies[];

extern ModelInfo gHumanModelInfo;
extern ModelInfo gDekuModelInfo;
extern ModelInfo gGoronModelInfo;
extern ModelInfo gZoraModelInfo;
extern ModelInfo gFierceDeityModelInfo;

void matchFaceTexturesToProxy(Link_FormProxy *formProxy);
void matchFaceTexturesToProxy(Link_FormProxy *formProxy);
void initFormProxy(Link_FormProxy *formProxy, PlayerTransformation form);
void requestRefreshFormProxy(Link_FormProxy *formProxy);
Gfx *getFormProxyDL(Link_FormProxy *formProxy, Link_DisplayList target);
void setSkeletonDLsOnModelInfo(Link_ModelInfo *info, FlexSkeletonHeader *skel);
void requestRefreshFormProxyDL(Link_FormProxy *formProxy, Link_DisplayList linkDLId);
void requestRefreshFormProxyMtx(Link_FormProxy *formProxy, Link_EquipmentMatrix mtxId);
void setupSharedListenerDL(ObjectId id, Gfx *vanillaDL, Link_DisplayList linkDLId);
Mtx *getFormProxyMatrix(Link_FormProxy *formProxy, Link_EquipmentMatrix mtxId);

#endif
