#ifdef _MSC_VER
# pragma warning(disable : 4090)
#endif

#include <criterion/theories.h>
#ifdef __cplusplus
# include <climits>
#else
# include <limits.h>
#endif

#define INT_DATAPOINTS    DataPoints(int, 0, 1, 2, -1, -2, INT_MAX, INT_MIN)

/* Let's test the multiplicative properties of 32-bit integers: */

int bad_mul(int a, int b)
{
    return a * b;
}

int bad_div(int a, int b)
{
    return a / b;
}

TheoryDataPoints(algebra, bad_divide_is_inverse_of_multiply) = {
    INT_DATAPOINTS,
    INT_DATAPOINTS,
};

Theory((int a, int b), algebra, bad_divide_is_inverse_of_multiply) {
    cr_assume(b != 0);
    cr_assert_eq(a, bad_div(bad_mul(a, b), b));
}

/* The above implementation of mul & div fails the test because of overflows,
   let's try again: */
long long good_mul(long long a, long long b)
{
    return a * b;
}

long long good_div(long long a, long long b)
{
    return a / b;
}

TheoryDataPoints(algebra, good_divide_is_inverse_of_multiply) = {
    INT_DATAPOINTS,
    INT_DATAPOINTS,
};

Theory((int a, int b), algebra, good_divide_is_inverse_of_multiply) {
    cr_assume(b != 0);
    cr_assert_eq(a, good_div(good_mul(a, b), b));
}

/* For triangulation */

Test(algebra, multiplication_by_integer) {
    cr_assert_eq(10, good_mul(5, 2));
}

/* Another property test */

TheoryDataPoints(algebra, zero_is_absorbing) = {
    INT_DATAPOINTS,
    INT_DATAPOINTS,
};

Theory((int a, int b), algebra, zero_is_absorbing) {
    cr_assume(a == 0 || b == 0);
    cr_assert_eq(0, good_mul(a, b));
}

/* Testing for various parameters */

struct my_object {
    int foo;
};

struct my_object o = { 42 };

char test_str[] = { 't', 'e', 's', 't', '\0' };

TheoryDataPoints(theory, misc) = {
    DataPoints(char,               'a'),
    DataPoints(bool,               true),
    DataPoints(short,              1),
    DataPoints(int,                1),
    DataPoints(long,               1),
    DataPoints(long long,          1),
    DataPoints(float,              3.14f),
    DataPoints(double,             3.14),
    DataPoints(char *,             test_str),
    DataPoints(const char *,       "other test"),
    DataPoints(struct my_object *, &o),
};

Theory((char c, bool b, short s, int i, long l, long long ll, float f, double d, char *str, const char *cstr, struct my_object *obj), theory, misc) {
    float reff = 3.14f;
    double refd = 3.14;

    cr_assert(b);
    cr_assert_eq(c, 'a');
    cr_assert_eq(s, 1);
    cr_assert_eq(i, 1);
    cr_assert_eq(l, 1);
    cr_assert_eq(ll, 1);
    cr_assert_eq(f, reff);
    cr_assert_eq(d, refd);
    cr_assert_str_eq(str, "test");
    cr_assert_str_eq(cstr, "other test");
    cr_assert_eq(obj->foo, 42);

    /* abort to see the formatted string of all parameters */
    cr_assert_fail();
}
