#include <criterion/parameterized.h>
#include <stdio.h>

// Basic usage

ParameterizedTestParameters(params, str) {
    static const char *strings[] = {
        "foo", "bar", "baz"
    };

    return cr_make_param_array(const char *, strings, sizeof (strings) / sizeof (const char *));
}

ParameterizedTest(const char **str, params, str) {
    cr_assert_fail("Parameter: %s", *str);
}

// Multiple parameters must be coalesced in a single parameter

struct parameter_tuple {
    int i;
    double d;
};

ParameterizedTestParameters(params, multiple) {
    static struct parameter_tuple params[] = {
        {1, 2},
        {3, 4},
        {5, 6},
    };

    return cr_make_param_array(struct parameter_tuple, params, sizeof (params) / sizeof (struct parameter_tuple));
}

ParameterizedTest(struct parameter_tuple *tup, params, multiple) {
    cr_assert_fail("Parameters: (%d, %f)", tup->i, tup->d);
}

// Cleaning up dynamically generated parameters

// Note: Do **NOT** embed dynamically allocated pointers inside of structures
// or this will fail on windows

void free_params(struct criterion_test_params *crp) {
    free(crp->params);
}

ParameterizedTestParameters(params, cleanup) {
    const size_t nb_tuples = 3;

    struct parameter_tuple *params = malloc(sizeof(struct parameter_tuple) * nb_tuples);
    params[0] = (struct parameter_tuple) { 1, 2 };
    params[1] = (struct parameter_tuple) { 3, 4 };
    params[2] = (struct parameter_tuple) { 5, 6 };

    return cr_make_param_array(struct parameter_tuple, params, nb_tuples, free_params);
}

ParameterizedTest(struct parameter_tuple *tup, params, cleanup) {
    cr_assert_fail("Parameters: (%d, %f)", tup->i, tup->d);
}
