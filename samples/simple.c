#include <criterion/criterion.h>

Test(misc, failing) {
    assert(0);
}

Test(misc, passing) {
    assert(1);
}
