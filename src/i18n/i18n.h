#ifndef I18N_H_
# define I18N_H_

# include <libintl.h>
# include "config.h"

# define _(String) (dgettext ? dgettext(PACKAGE, String) : String)
# define _s(String, Plural, Quantity)                           \
    (dgettext ? dngettext(PACKAGE, String, Plural, Quantity)    \
              : (Quantity == 1 ? String : Plural))

#endif /* !I18N_H_ */
