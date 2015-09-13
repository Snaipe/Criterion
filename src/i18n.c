#include "i18n.h"
#include "criterion/assert.h"
#include "criterion/asprintf-compat.h"

void init_i18n(void) {
#if ENABLE_NLS
    bindtextdomain (PACKAGE, LOCALEDIR);
#endif
}

char *translate_assert_msg(int msg_index, ...) {
    static char *messages[] = {
        [CRITERION_ASSERT_MSG_FAIL] = N_("The conditions for this assertion were not met."),
        [CRITERION_ASSERT_MSG_EXPR_FALSE] = N_("The expression %s is false."),
        [CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE] = N_("The expression (as strings) %s is false."),
        [CRITERION_ASSERT_MSG_IS_NULL] = N_("%s is null."),
        [CRITERION_ASSERT_MSG_IS_NOT_NULL] = N_("%s is not null."),
        [CRITERION_ASSERT_MSG_IS_EMPTY] = N_("%s is empty."),
        [CRITERION_ASSERT_MSG_IS_NOT_EMPTY] = N_("%s is not empty."),

#ifdef ENABLE_NLS
        [CRITERION_ASSERT_MSG_FILE_STR_MATCH] = N_("The file contents of %1$s does not match the string \"%2$s\"."),
#else
        [CRITERION_ASSERT_MSG_FILE_STR_MATCH] = "The file contents of %s does not match the string \"%s\".",
#endif
    };

    va_list vl;
    va_start(vl, msg_index);
    char *out;
    cr_vasprintf(&out, _(messages[msg_index]), vl);
    va_end(vl);

    return out;
}
