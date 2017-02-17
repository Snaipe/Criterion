#include "i18n.h"
#include "criterion/assert.h"
#include "criterion/internal/asprintf-compat.h"

void init_i18n(void)
{
#if ENABLE_NLS
    bindtextdomain(PACKAGE, LOCALEDIR);
#endif
}

char *cr_translate_assert_msg(int msg_index, ...)
{
    static char *messages[] = {
        [CRITERION_ASSERT_MSG_FAIL] = N_("The conditions for this assertion were not met."),
        [CRITERION_ASSERT_MSG_EXPR_FALSE] = N_("The expression %s is false."),
        [CRITERION_ASSERT_MSG_IS_NULL] = N_("%s is null."),
        [CRITERION_ASSERT_MSG_IS_NOT_NULL] = N_("%s is not null."),
        [CRITERION_ASSERT_MSG_IS_EMPTY] = N_("%s is empty."),
        [CRITERION_ASSERT_MSG_ANY_THROW] = N_("The statement `%s` did not throw any exception."),
        [CRITERION_ASSERT_MSG_NONE_THROW] = N_("The statement `%s` threw some exception."),

#ifdef ENABLE_NLS
        [CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE] = N_("The expression (as strings) %1$s is false: actual=`%2$s` expected=`%3$s`."),
        [CRITERION_ASSERT_MSG_IS_NOT_EMPTY] = N_("%1$s (`%2$s`) is not empty."),
        [CRITERION_ASSERT_MSG_FILE_STR_MATCH] = N_("The file contents of %1$s does not match the string \"%2$s\"."),
        [CRITERION_ASSERT_MSG_FILE_MATCH] = N_("The file contents of %1$s does not match the contents of %2$s."),
        [CRITERION_ASSERT_MSG_THROW] = N_("The statement `%1$s` did throw an instance of the `%2$s` exception."),
        [CRITERION_ASSERT_MSG_NO_THROW] = N_("The statement `%1$s` did not throw an instance of the `%2$s` exception."),
#else
        [CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE] = "The expression (as strings) %s is false: actual=`%s` expected=`%s`.",
        [CRITERION_ASSERT_MSG_IS_NOT_EMPTY] = "%s (`%s`) is not empty.",
        [CRITERION_ASSERT_MSG_FILE_STR_MATCH] = "The file contents of %s does not match the string \"%s\".",
        [CRITERION_ASSERT_MSG_FILE_MATCH] = "The file contents of %s does not match the contents of %s.",
        [CRITERION_ASSERT_MSG_THROW] = "The statement `%s` did throw an instance of the `%s` exception.",
        [CRITERION_ASSERT_MSG_NO_THROW] = "The statement `%s` did not throw an instance of the `%s` exception.",
#endif
    };

    va_list vl;

    va_start(vl, msg_index);
    char *out;
    cr_vasprintf(&out, _(messages[msg_index]), vl);
    va_end(vl);

    return out;
}
