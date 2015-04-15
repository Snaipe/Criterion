#include <criterion/criterion.h>

Test(misc, failing, .description = "Just a failing test") {
    assert(0);
}

Test(misc, skipped, .description = "This one is skipped", .disabled = true) {
}
