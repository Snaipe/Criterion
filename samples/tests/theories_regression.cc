#ifdef _MSC_VER
#pragma warning(disable : 4090)
#endif

#include <criterion/theories.h>

// Testing for various parameters

char test_str[] = {'t', 'e', 's', 't', '\0'};

TheoryDataPoints(theory, misc) = {
    DataPoints(char, 'a'),
    DataPoints(bool, true),
    DataPoints(short, 1),
    DataPoints(int, 1),
    DataPoints(long, 1),
    DataPoints(long long, 1),
    DataPoints(float, 3.14f),
    DataPoints(double, 3.14),
    DataPoints(char *, test_str),
    DataPoints(const char *, "other test"),
};

Theory((char c, bool b, short s, int i, long l, long long ll, float f, double d, char *str, const char *cstr), theory, misc) {
    cr_assert(b);
    cr_assert_eq(c, 'a');
    cr_assert_eq(s, 1);
    cr_assert_eq(i, 1);
    cr_assert_eq(l, 1);
    cr_assert_eq(ll, 1);
    cr_assert_eq(f, 3.14f);
    cr_assert_eq(d, 3.14);
    cr_assert_str_eq(str, "test");
    cr_assert_str_eq(cstr, "other test");

    // abort to see the formatted string of all parameters
    cr_assert_fail();
}

// Manually generate datapoints

TheoryDataPoints(theory, gen) = {
    DataPoints(int, 0), // placeholder
};

static void generate_datapoints(void) {
    static int arr[] = {1, 2, 3, 4, 5};
    TheoryDataPoint(theory, gen)[0].len = 5;
    TheoryDataPoint(theory, gen)[0].arr = &arr;
}

Theory((int i), theory, gen, .init = generate_datapoints) {
    (void) i;
    cr_assert_fail(); // we fail to display the parameter
}
