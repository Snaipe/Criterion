#ifndef CRITERION_HOOKS_H_
# define CRITERION_HOOKS_H_

#include "common.h"

typedef enum {
    PRE_INIT,
    PRE_TEST,
    POST_TEST,
    POST_FINI,
} e_report_status;

typedef void (*f_report_hook)(void *);

# define HOOK_IDENTIFIER_(Suffix) HOOK_IDENTIFIER__(__LINE__, Suffix)
# define HOOK_IDENTIFIER__(Line, Suffix) HOOK_IDENTIFIER___(Line, Suffix)
# define HOOK_IDENTIFIER___(Line, Suffix) hook_l ## Line ## _ ## Suffix

# define HOOK_PROTOTYPE_                                                       \
    void HOOK_IDENTIFIER_(impl)(UNUSED void *data)

# define ReportHook(Kind)                                                      \
    HOOK_PROTOTYPE_;                                                           \
    SECTION_("criterion_hooks_" #Kind)                                         \
    const f_report_hook HOOK_IDENTIFIER_(func) = HOOK_IDENTIFIER_(impl);      \
    HOOK_PROTOTYPE_

extern __attribute__ ((weak)) void criterion_init(void);
extern __attribute__ ((weak)) void criterion_fini(void);

#endif /* !CRITERION_HOOKS_H_ */
