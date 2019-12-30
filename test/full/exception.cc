#include <criterion/criterion.h>
#include <stdexcept>

void raise_std(void)
{
    throw std::invalid_argument("Some exception message");
}

void raise_any(void)
{
    throw 1;
}

Test(exception, raise_std) {
    raise_std();
}
Test(exception, raise_any) {
    raise_any();
}

Test(exception, raise_std_init, .init = raise_std) {
}
Test(exception, raise_any_init, .init = raise_any) {
}
Test(exception, raise_std_fini, .fini = raise_std) {
}
Test(exception, raise_any_fini, .fini = raise_any) {
}
