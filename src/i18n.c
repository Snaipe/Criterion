#include "i18n.h"

#ifdef I18N
__attribute__ ((constructor))
void init_i18n(void) {
    bindtextdomain (PACKAGE, LOCALEDIR);
}
#endif
