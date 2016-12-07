#ifdef _MSC_VER
# pragma warning(disable : 4090)
#endif

#include <criterion/theories.h>

/* Testing for various parameters */

char test_str[] = { 't', 'e', 's', 't', '\0' };

TheoryDataPoints(theory, misc) = {
    DataPoints(char,         'a'),
    DataPoints(bool,         true),
    DataPoints(short,        1),
    DataPoints(int,          1),
    DataPoints(float,        3.14f),
    DataPoints(double,       3.14),
    DataPoints(char *,       test_str),
    DataPoints(const char *, "other test"),
};

Theory((char c, bool b, short s, int i, float f, double d, char *str, const char *cstr), theory, misc) {
    float reff = 3.14f;
    double refd = 3.14;

    cr_assert(b);
    cr_assert_eq(c, 'a');
    cr_assert_eq(s, 1);
    cr_assert_eq(i, 1);
    cr_assert_eq(f, reff);
    cr_assert_eq(d, refd);
    cr_assert_str_eq(str, "test");
    cr_assert_str_eq(cstr, "other test");

    /* abort to see the formatted string of all parameters */
    cr_assert_fail();
}
