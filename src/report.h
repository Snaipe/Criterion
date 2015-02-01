#ifndef REPORT_H_
# define REPORT_H_

# include "hooks.h"

# define report(Kind, Data) call_report_hooks_##Kind(Data)

# define DECL_CALL_REPORT_HOOKS(Kind)                       \
    extern f_report_hook __start_criterion_hooks_##Kind;    \
    extern f_report_hook __stop_criterion_hooks_##Kind;     \
    void call_report_hooks_##Kind(void *data)

DECL_CALL_REPORT_HOOKS(PRE_INIT);
DECL_CALL_REPORT_HOOKS(PRE_TEST);
DECL_CALL_REPORT_HOOKS(POST_TEST);
DECL_CALL_REPORT_HOOKS(POST_FINI);

#endif /* !REPORT_H_ */
