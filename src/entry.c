#include "criterion/criterion.h"

CR_API int main(int argc, char *argv[]) {
    struct criterion_test_set *tests = criterion_initialize();

    if (!criterion_handle_args(argc, argv, true))
        return 0;

    int result = !criterion_run_all_tests(tests);

    criterion_finalize(tests);

    return result;
}
