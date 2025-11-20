#ifndef MODELREPLACER_COMPAT_H
#define MODELREPLACER_COMPAT_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "z64object.h"
#include "z64player.h"
#include "modelmatrixids.h"
#include "stdbool.h"

#define MRC_PLAYER_FORM_EVERY PLAYER_FORM_MAX + 1

typedef struct {
    PlayerTransformation form;
    Link_DisplayList linkDLId;
    ObjectId objId;
    Gfx *vanillaDL;
} MRC_ListenerInfo;

void MRC_setupListenerDL(ObjectId id, Gfx *vanillaDL, PlayerTransformation form, Link_DisplayList linkDLId);
MRC_ListenerInfo *MRC_getListenerFromFormAndDL(PlayerTransformation form, Link_DisplayList entryDLId);
bool MRC_isMRCEnabled();
Gfx *MRC_getListenerDL(PlayerTransformation form, Link_DisplayList entryDLId);
void MRC_addExcludedDL(Gfx *dl);

#endif
