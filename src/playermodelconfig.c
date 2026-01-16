#include "playermodelconfig.h"

FormProxyId PlayerModelConfig_getNumFormIds(void) {
    // this is the only place this define should be used
    // the define is kept in the header for convenience
    return FORM_PROXY_ID_MAX_DO_NOT_DIRECTLY_USE;
}
