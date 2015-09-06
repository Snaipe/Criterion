#ifndef I18N_H_
# define I18N_H_

# include "config.h"

# if !ENABLE_NLS
#  define _(String) String
#  define _s(String, Plural, Quantity) ((Quantity) == 1 ? String : Plural)
# else
#  include <libintl.h>
#  define _(String) dgettext(PACKAGE, String)
#  define _s(String, Plural, Quantity) \
        dngettext(PACKAGE, String, Plural, (Quantity))
# endif

void init_i18n(void);

#endif /* !I18N_H_ */
