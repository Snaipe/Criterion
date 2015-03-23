#include <criterion/criterion.h>

Test(sample, long_msg) {
    assert(0, "This is\nA long message\nSpawning multiple lines.\n\nFormatting is respected.");
}
