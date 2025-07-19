#ifndef __Z64RECOMP_INTERPOLATION__
#define __Z64RECOMP_INTERPOLATION__

#include "global.h"
#include "modding.h"

RECOMP_IMPORT("*", u32 actor_get_interpolation_skipped(Actor *actor));
RECOMP_IMPORT("*", void actor_set_interpolation_skipped(Actor *actor));
RECOMP_IMPORT("*", void actor_clear_interpolation_skipped(Actor *actor));

#endif
