#ifndef CRITERION_H_
# define CRITERION_H_

# include "common.h"
# include "assert.h"
# include "types.h"

# define IDENTIFIER_(Category, Name, Suffix) \
    Category ## _ ## Name ## _ ## Suffix
# define TEST_PROTOTYPE_(Category, Name) \
    void IDENTIFIER_(Category, Name, impl)(void)

# define SUITE_IDENTIFIER_(Name, Suffix) \
    suite_ ## Name ## _ ## Suffix

# define Test(...) Test_(__VA_ARGS__, .sentinel_ = 0)
# define Test_(Category, Name, ...)                                            \
    TEST_PROTOTYPE_(Category, Name);                                           \
    struct criterion_test_extra_data IDENTIFIER_(Category, Name, extra) = {    \
        .identifier_ = #Category "/" #Name,                                    \
        .file_    = __FILE__,                                                  \
        .line_    = __LINE__,                                                  \
        __VA_ARGS__                                                            \
    };                                                                         \
    SECTION_("criterion_tests")                                                \
    const struct criterion_test IDENTIFIER_(Category, Name, meta) = {          \
        .name     = #Name,                                                     \
        .category = #Category,                                                 \
        .test     = IDENTIFIER_(Category, Name, impl),                         \
        .data     = &IDENTIFIER_(Category, Name, extra)                        \
    };                                                                         \
    TEST_PROTOTYPE_(Category, Name)

# define TestSuite(...) TestSuite_(__VA_ARGS__, .sentinel_ = 0)
# define TestSuite_(Name, ...)                                                 \
    struct criterion_test_extra_data SUITE_IDENTIFIER_(Name, extra) = {        \
        .file_    = __FILE__,                                                  \
        .line_    = 0,                                                         \
        __VA_ARGS__                                                            \
    };                                                                         \
    SECTION_("crit_suites")                                                    \
    const struct criterion_suite SUITE_IDENTIFIER_(Name, meta) = {             \
        .name     = #Name,                                                     \
        .data     = &SUITE_IDENTIFIER_(Name, extra),                           \
    }

int criterion_run_all_tests(void);

#endif /* !CRITERION_H_ */
