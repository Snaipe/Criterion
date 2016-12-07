#include <criterion/parameterized.h>
#include <stdio.h>

/* Basic usage */

ParameterizedTestParameters(params, simple) {
    static int vals[] = { 1, 2, 3 };

    return cr_make_param_array(int, vals, sizeof (vals) / sizeof (int));
}

ParameterizedTest(int *val, params, simple) {
    cr_assert_fail("Parameter: %d", *val);
}

/* Multiple parameters must be coalesced in a single parameter */

struct parameter_tuple {
    int i;
    double d;
};

ParameterizedTestParameters(params, multiple) {
    static struct parameter_tuple params[] = {
        { 1, 2 },
        { 3, 4 },
        { 5, 6 },
    };

    return cr_make_param_array(struct parameter_tuple, params, sizeof (params) / sizeof (struct parameter_tuple));
}

ParameterizedTest(struct parameter_tuple *tup, params, multiple) {
    cr_assert_fail("Parameters: (%d, %f)", tup->i, tup->d);
}

/* Cleaning up dynamically generated parameters */

/* you **MUST** use cr_malloc, cr_free, cr_realloc, and cr_calloc instead of their
   unprefixed counterparts to allocate dynamic memory in parameters, otherwise
   this will crash. */

struct parameter_tuple_dyn {
    int i;
    double *d;
};

void free_params(struct criterion_test_params *crp)
{
    for (size_t i = 0; i < crp->length; ++i) {
        struct parameter_tuple_dyn *tup = (struct parameter_tuple_dyn *) crp->params + i;
        cr_free(tup->d);
    }
    cr_free(crp->params);
}

double *gen_double(double val)
{
    double *ptr = cr_malloc(sizeof (double));

    *ptr = val;
    return ptr;
}

ParameterizedTestParameters(params, cleanup) {
    const size_t nb_tuples = 3;

    struct parameter_tuple_dyn *params = cr_malloc(sizeof (struct parameter_tuple_dyn) * nb_tuples);

    params[0] = (struct parameter_tuple_dyn) { 1, gen_double(2) };
    params[1] = (struct parameter_tuple_dyn) { 3, gen_double(4) };
    params[2] = (struct parameter_tuple_dyn) { 5, gen_double(6) };

    return cr_make_param_array(struct parameter_tuple_dyn, params, nb_tuples, free_params);
}

ParameterizedTest(struct parameter_tuple_dyn *tup, params, cleanup) {
    cr_assert_fail("Parameters: (%d, %f)", tup->i, *tup->d);
}
