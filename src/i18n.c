#include "i18n.h"

void init_i18n(void) {
#if ENABLE_NLS
    bindtextdomain (PACKAGE, LOCALEDIR);
#endif
}
