#include <criterion/theories.h>
#include <limits.h>

# define INT_DATAPOINTS DataPoints(int, 0, 1, 2, -1, -2, INT_MAX, INT_MIN)

// Let's test the multiplicative properties of 32-bit integers:

int bad_mul(int a, int b) {
    return a * b;
}

int bad_div(int a, int b) {
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

// The above implementation of mul & div fails the test because of overflows,
// let's try again:

long long good_mul(long long a, long long b) {
    return a * b;
}

long long good_div(long long a, long long b) {
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

// For triangulation

Test(algebra, multiplication_by_integer) {
    cr_assert_eq(10, good_mul(5, 2));
}

// Another property test

TheoryDataPoints(algebra, zero_is_absorbing) = {
    INT_DATAPOINTS,
    INT_DATAPOINTS,
};

Theory((int a, int b), algebra, zero_is_absorbing) {
    cr_assume(a == 0 || b == 0);
    cr_assert_eq(0, good_mul(a, b));
}
