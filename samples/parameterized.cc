#include <criterion/parameterized.h>

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

    return criterion_test_params(params);
}

ParameterizedTest(struct parameter_tuple *tup, params, multiple) {
    cr_assert_fail("Parameters: (%d, %f)", tup->i, tup->d);
}

// Using dynamically generated parameters

// you **MUST** use new_obj, new_arr, delete_obj, delete_arr instead of
// the new, new[], delete and delete[] operators (respectively) to allocate and
// deallocate dynamic memory in parameters, otherwise this will crash.

// the criterion::allocator<T> allocator may be used with STL containers to
// allocate objects with the functions described above.

using criterion::new_obj;
using criterion::new_arr;
using criterion::delete_obj;
using criterion::delete_arr;

struct parameter_tuple_dyn {
    int i;
    std::unique_ptr<double, decltype(criterion::free)> d;

    parameter_tuple_dyn() : i(0), d(nullptr, criterion::free) {}
    parameter_tuple_dyn(int i, double *d) : i(i), d(d, criterion::free) {}
};

ParameterizedTestParameters(params, cleanup) {
    static criterion::parameters<parameter_tuple_dyn> params;

    params.push_back(parameter_tuple_dyn(1, new_obj<double>(2)));
    params.push_back(parameter_tuple_dyn(3, new_obj<double>(4)));
    params.push_back(parameter_tuple_dyn(5, new_obj<double>(6)));

    // A criterion::parameters<T> can be returned in place of a
    // criterion_test_params.
    return params;
}

ParameterizedTest(parameter_tuple_dyn *tup, params, cleanup) {
    cr_assert_fail("Parameters: (%d, %f)", tup->i, *tup->d);
}
