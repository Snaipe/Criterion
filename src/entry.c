#include "criterion/criterion.h"

CR_API int main(int argc, char *argv[]) {
    if (!criterion_initialize(argc, argv, true))
        return 0;

    return !criterion_run_all_tests();
}
