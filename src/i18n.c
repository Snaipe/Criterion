#include "i18n.h"

#if ENABLE_NLS
__attribute__ ((constructor))
void init_i18n(void) {
    bindtextdomain (PACKAGE, LOCALEDIR);
}
#endif
