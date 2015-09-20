Using parameterized tests
=========================

Parameterized tests are useful to repeat a specific test logic over a finite
set of parameters.

Due to limitations on how generated parameters are passed, parameterized tests
can only accept one pointer parameter; however, this is not that much of a
problem since you can just pass a structure containing the context you need.

Adding parameterized tests
--------------------------

Adding parameterized tests is done by defining the parameterized test function,
and the parameter generator function:

.. code-block:: c

    #include <criterion/parameterized.h>

    ParameterizedTestParameter(suite_name, test_name) = {
        void *params;
        size_t nb_params;

        // generate parameter set
        return cr_make_param_array(Type, params, nb_params);
    }

    ParameterizedTest(Type *param, suite_name, test_name) {
        // contents of the test
    }

``suite_name`` and ``test_name`` are the identifiers of the test suite and
the test, respectively. These identifiers must follow the language
identifier format.

``Type`` is the compound type of the generated array. ``params`` and ``nb_params``
are the pointer and the length of the generated array, respectively.

Passing multiple parameters
---------------------------

As said earlier, parameterized tests only take one parameter, so passing
multiple parameters is, in the strict sense, not possible. However, one can
easily use a struct to hold the context as a workaround:

.. code-block:: c

    #include <criterion/parameterized.h>

    struct my_params {
        int param0;
        double param1;
        ...
    };

    ParameterizedTestParameter(suite_name, test_name) = {
        size_t nb_params = 32;
        struct my_params *params = malloc(sizeof (struct my_params) * nb_params);

        // generate parameter set

        params[0] = ...
        params[1] = ...

        ...

        return cr_make_param_array(struct my_params, params, nb_params);
    }

    ParameterizedTest(struct my_params *param, suite_name, test_name) {
        // access param.param0, param.param1, ...
    }

There is, however, one absolute rule that must be respected, unless you don't
want your tests to run on windows, ever: parameters must not contain any
pointer to dynamically allocated data.

Hence, this is not permitted:

.. code-block:: c

    #include <criterion/parameterized.h>

    struct my_params {
        int *some_int_ptr;
    };

    ParameterizedTestParameter(suite_name, test_name) = {
        static my_params param = {
            .some_int_ptr = malloc(sizeof (int)); // Don't do this.
        };
        *param.some_int_ptr = 42;

        return cr_make_param_array(struct my_params, &param, 1);
    }

and **will crash the test** on Windows.

Configuring parameterized tests
-------------------------------

Parameterized tests can optionally recieve configuration parameters to alter
their own behaviour, and are applied to each iteration of the parameterized
test individually (this means that the initialization and finalization runs once
per iteration).
Those parameters are the same ones as the ones of the ``Test`` macro function
(c.f. :ref:`test-config-ref`).


