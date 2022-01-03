#ifdef _MSC_VER
# pragma warning(disable : 4090)
#endif

#include <criterion/theories.h>

/* Testing for various parameters */

TheoryDataPoints(theory, misc) = {
    DataPoints(char,         'a'),
    DataPoints(bool,         true),
    DataPoints(short,        1),
    DataPoints(int,          1),
    DataPoints(float,        3.14f),
    DataPoints(double,       3.14),
    DataPoints(long double,  3.14l),
    DataPoints(char *,       "test"),
    DataPoints(const char *, "other test"),
};

Theory((char c, bool b, short s, int i, float f, double d, long double ld, char *str, const char *cstr), theory, misc) {
    float reff = 3.14f;
    double refd = 3.14;
    long double refld = 3.14l;

    cr_expect(b);
    cr_expect_eq(c, 'a');
    cr_expect_eq(s, 1);
    cr_expect_eq(i, 1);
    cr_expect_eq(f, reff);
    cr_expect_eq(d, refd);
    cr_expect_eq(ld, refld);
    cr_expect_str_eq(str, "test");
    cr_expect_str_eq(cstr, "other test");

    /* abort to see the formatted string of all parameters */
    cr_assert_fail();
}
