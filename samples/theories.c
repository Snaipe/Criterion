#include <criterion/theories.h>

TheoryDataPoints(theory, simple) = {
    DataPoints(int, 1, 2, 3),
    DataPoints(long, 314, 42)
};

Theory((int a, long b), theory, simple) {
    cr_assume(a == 2);
    printf("%d, %ld\n", a, b);
}

TheoryDataPoints(theory, float) = {
    DataPoints(float, 3.14, 42),
};

Theory((float a), theory, float) {
    printf("%f\n", (double) a);
}
