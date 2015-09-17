#include <criterion/parameterized.h>

ParameterizedTestParameters(params, str) {
    static const char *strings[] = {
        "foo", "bar", "baz"
    };

    return cr_make_param_array(const char *, strings, sizeof (strings) / sizeof (const char *));
}

ParameterizedTest(const char *str, params, str) {
    cr_assert(0, "Parameter: %s", str);
}
