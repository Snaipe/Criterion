#include <criterion/criterion.h>

Test(misc, failing, .description = "Just a failing test") {
    assert(0);
}
