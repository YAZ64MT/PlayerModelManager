#include "playermodelconfig.h"

#define FORM_PROXY_ID_MAX ((FormProxyId)5)

FormProxyId PlayerModelConfig_getNumFormIds(void) {
    return FORM_PROXY_ID_MAX;
}
