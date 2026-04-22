#include "criterion/internal/test.h"
#include "abort.h"
#include "filc-simple.h"

extern const struct criterion_test *criterion_current_test;
extern const struct criterion_suite *criterion_current_suite;

void cri_asserts_passed_incr(void)
{
}

void criterion_internal_run_test(void (*fn)(void))
{
    const struct criterion_suite *suite = criterion_current_suite;
    const struct criterion_test *test = criterion_current_test;

    enum {
        PHASE_SUITE_INIT,
        PHASE_TEST_INIT,
        PHASE_MAIN,
        PHASE_TEST_FINI,
        PHASE_SUITE_FINI,
        PHASE_DONE,
    } phase = PHASE_SUITE_INIT;

    for (;;) {
        if (cri_setjmp(g_pre_test)) {
            if (cri_filc_test_skipped())
                return;
            if (phase < PHASE_TEST_FINI)
                phase = PHASE_TEST_FINI;
            else if (phase < PHASE_SUITE_FINI)
                phase = PHASE_SUITE_FINI;
            else
                return;
        }

        switch (phase) {
            case PHASE_SUITE_INIT:
                if (suite->data && suite->data->init)
                    suite->data->init();
                phase = PHASE_TEST_INIT;
                break;
            case PHASE_TEST_INIT:
                if (test->data->init)
                    test->data->init();
                phase = PHASE_MAIN;
                break;
            case PHASE_MAIN:
                if (test->data->param_)
                    criterion_test_die("Parameterized tests are unsupported in Fil-C simple mode.");
                fn();
                phase = PHASE_TEST_FINI;
                break;
            case PHASE_TEST_FINI:
                if (test->data->fini)
                    test->data->fini();
                phase = PHASE_SUITE_FINI;
                break;
            case PHASE_SUITE_FINI:
                if (suite->data && suite->data->fini)
                    suite->data->fini();
                phase = PHASE_DONE;
                break;
            case PHASE_DONE:
                return;
        }
    }
}
