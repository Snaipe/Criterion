#include <stdlib.h>
#include <string.h>

#include "criterion/internal/ordered-set.h"
#include "criterion/internal/preprocess.h"
#include "criterion/options.h"
#include "criterion/criterion.h"
#include "csptr/smalloc.h"
#include "filc-simple.h"
#include "runner.h"
#include "common.h"
#include "string/extglobmatch.h"

CR_API const struct criterion_test *criterion_current_test;
CR_API const struct criterion_suite *criterion_current_suite;
int cri_is_runner = 1;

static int cmp_suite(void *a, void *b)
{
    struct criterion_suite *s1 = a, *s2 = b;
    return strcmp(s1->name, s2->name);
}

static int cmp_test(void *a, void *b)
{
    struct criterion_test *s1 = a, *s2 = b;
    return strcmp(s1->name, s2->name);
}

static void dtor_suite_set(void *ptr, CR_UNUSED void *meta)
{
    struct criterion_suite_set *s = ptr;
    sfree(s->tests);
}

static void dtor_test_set(void *ptr, CR_UNUSED void *meta)
{
    struct criterion_test_set *t = ptr;
    sfree(t->suites);
}

CR_API void criterion_register_test(struct criterion_test_set *set,
        struct criterion_test *test)
{
    struct criterion_suite_set css = {
        .suite = { .name = test->category },
    };
    struct criterion_suite_set *s = insert_ordered_set(set->suites, &css, sizeof (css));

    if (!s->tests)
        s->tests = new_ordered_set(cmp_test, NULL);

    insert_ordered_set(s->tests, test, sizeof (*test));
    ++set->tests;
}

extern struct criterion_test *__start_cr_tst[] __attribute__((weak));
extern struct criterion_test *__stop_cr_tst[] __attribute__((weak));

struct criterion_test_set *criterion_init(void)
{
    struct criterion_ordered_set *suites = new_ordered_set(cmp_suite, dtor_suite_set);
    struct criterion_test_set *set = smalloc(
        .size = sizeof (struct criterion_test_set),
        .dtor = dtor_test_set);

    *set = (struct criterion_test_set) {
        .suites = suites,
        .tests = 0,
    };

    if (!__start_cr_tst || !__stop_cr_tst)
        return set;

    for (struct criterion_test **test = __start_cr_tst; test < __stop_cr_tst; ++test) {
        if (!*test || !*(*test)->category || !*(*test)->name)
            continue;
        criterion_register_test(set, *test);
    }

    return set;
}

void disable_unmatching(struct criterion_test_set *set)
{
    if (!compile_pattern(criterion_options.pattern))
        exit(3);
    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if ((s->suite.data && s->suite.data->disabled) || !s->tests)
            continue;

        FOREACH_SET(struct criterion_test *test, s->tests) {
            if (match(test->data->identifier_) == 0)
                test->data->disabled = true;
        }
    }
    free_pattern();
}

CR_API struct criterion_test_set *criterion_initialize(void)
{
    return criterion_init();
}

CR_API void criterion_finalize(struct criterion_test_set *set)
{
    sfree(set);
}

static int criterion_run_all_tests_impl(struct criterion_test_set *set)
{
    int failed = 0;

    FOREACH_SET(struct criterion_suite_set *suite_set, set->suites) {
        if ((suite_set->suite.data && suite_set->suite.data->disabled) || !suite_set->tests)
            continue;

        FOREACH_SET(struct criterion_test *test, suite_set->tests) {
            if (test->data->disabled)
                continue;

            criterion_current_test = test;
            criterion_current_suite = &suite_set->suite;
            cri_filc_reset_test_state();
            test->test();

            if (cri_filc_test_skipped())
                continue;
            if (cri_filc_test_failed())
                ++failed;
        }
    }

    return failed == 0;
}

CR_API int criterion_run_all_tests(struct criterion_test_set *set)
{
    cri_is_runner = 1;

    if (criterion_options.pattern)
        disable_unmatching(set);

    return criterion_options.always_succeed || criterion_run_all_tests_impl(set);
}
