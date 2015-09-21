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
        struct my_params *params = cr_malloc(sizeof (struct my_params) * nb_params);

        // generate parameter set

        params[0] = ...
        params[1] = ...

        ...

        return cr_make_param_array(struct my_params, params, nb_params);
    }

    ParameterizedTest(struct my_params *param, suite_name, test_name) {
        // access param.param0, param.param1, ...
    }

Dynamically allocating fields
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Any dynamic memory allocation done from a ParameterizedTestParameter function
**must** be done with ``cr_malloc``, ``cr_calloc``, or ``cr_realloc``.

Any pointer returned by those 3 functions must be passed to ``cr_free`` after
you have no more use of it.

It is undefined behaviour to use any other allocation function (such as ``malloc``)
from the scope of a ParameterizedTestParameter function.

In C++, these methods should not be called explicitely -- instead, you should
use:

* ``criterion::new_obj<Type>(params...)`` to allocate an object of type ``Type``
  and call its constructor taking ``params...``.
  The function possess the exact same semantics as ``new Type(params...)``.
* ``criterion::delete_obj(obj)`` to destroy an object previously allocated by
  ``criterion::new_obj``.
  The function possess the exact same semantics as ``delete obj``.
* ``criterion::new_arr<Type>(size)`` to allocate an array of objects of type ``Type``
  and length ``size``. ``Type`` is initialized by calling its default constructor.
  The function possess the exact same semantics as ``new Type[size]``.
* ``criterion::delete_arr(array)`` to destroy an array previously allocated by
  ``criterion::new_arr``.
  The function possess the exact same semantics as ``delete[] array``.

Freeing dynamically allocated parameter fields
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

One can pass an extra parameter to ``cr_make_param_array`` to specify
the cleanup function that should be called on the generated parameter context:

.. code-block:: c

    #include <criterion/parameterized.h>

    struct my_params {
        int *some_int_ptr;
    };

    void cleanup_params(struct criterion_test_params *ctp) {
        cr_free(((struct my_params *) ctp->params)->some_int_ptr);
    }

    ParameterizedTestParameter(suite_name, test_name) = {
        static my_params param = {
            .some_int_ptr = cr_malloc(sizeof (int));
        };
        *param.some_int_ptr = 42;

        return cr_make_param_array(struct my_params, &param, 1, cleanup_params);
    }

Configuring parameterized tests
-------------------------------

Parameterized tests can optionally recieve configuration parameters to alter
their own behaviour, and are applied to each iteration of the parameterized
test individually (this means that the initialization and finalization runs once
per iteration).
Those parameters are the same ones as the ones of the ``Test`` macro function
(c.f. :ref:`test-config-ref`).


