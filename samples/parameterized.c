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
    struct parameter_tuple_dyn *tuples = crp->params;
    for (size_t i = 0; i < crp->length; ++i) {
        struct parameter_tuple_dyn *tup = &tuples[i];
        free(tup->d);
    }
    free(crp->params);
}

double *gen_double(double d) {
    double *ptr = malloc(sizeof(double));
    printf("%p\n", ptr);
    fflush(NULL);
    *ptr = d;
    return ptr;
}

ParameterizedTestParameters(params, cleanup) {
    const size_t nb_tuples = 3;

    struct parameter_tuple_dyn *params = malloc(sizeof(struct parameter_tuple_dyn) * nb_tuples);
    params[0] = (struct parameter_tuple_dyn) { 1, gen_double(2) };
    params[1] = (struct parameter_tuple_dyn) { 3, gen_double(4) };
    params[2] = (struct parameter_tuple_dyn) { 5, gen_double(6) };

    return cr_make_param_array(struct parameter_tuple_dyn, params, nb_tuples, free_params);
}

ParameterizedTest(struct parameter_tuple_dyn *tup, params, cleanup) {
    cr_assert_fail("Parameters: (%d, %p)", tup->i, tup->d);
}
