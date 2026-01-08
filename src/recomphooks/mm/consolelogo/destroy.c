#include "global.h"
#include "modding.h"

typedef struct ConsoleLogoState ConsoleLogoState;

static ConsoleLogoState *sConsoleLogo;

RECOMP_HOOK("ConsoleLogo_Destroy") void on_ConsoleLogo_Destroy(GameState *thisx) {
    sConsoleLogo = (ConsoleLogoState *)thisx;
}

void applySavedModels_on_return_ConsoleLogo_Destroy(void);

RECOMP_HOOK_RETURN("ConsoleLogo_Destroy") void on_return_ConsoleLogo_Destroy(void) {
    applySavedModels_on_return_ConsoleLogo_Destroy();
}
