Changing the internals
======================

Providing your own main
-----------------------

If you are not satisfied with the default CLI or environment variables, you
can define your own main function.

Configuring the test runner
~~~~~~~~~~~~~~~~~~~~~~~~~~~

First and foremost, you need to generate the test set; this is done by calling
``criterion_initialize()``. The function returns a ``struct criterion_test_set *``,
that you need to pass to ``criterion_run_all_tests`` later on.

At the very end of your main, you also need to call ``criterion_finalize`` with
the test set as parameter to free any ressources initialized by criterion earlier.

You'd usually want to configure the test runner before calling it.
Configuration is done by setting fields in a global variable named
``criterion_options`` (include criterion/options.h).

Here is an exhaustive list of these fields:

=================== ================================== ==============================================================
Field               Type                               Description
=================== ================================== ==============================================================
logging_threshold   enum criterion_logging_level       The logging level
------------------- ---------------------------------- --------------------------------------------------------------
logger              struct criterion_logger *          The logger (see below)
------------------- ---------------------------------- --------------------------------------------------------------
always_succeed      bool                               True iff criterion_run_all_tests should always returns 1
------------------- ---------------------------------- --------------------------------------------------------------
use_ascii           bool                               True iff the outputs should use the ASCII charset
------------------- ---------------------------------- --------------------------------------------------------------
fail_fast           bool                               True iff the test runner should abort after the first failure
------------------- ---------------------------------- --------------------------------------------------------------
pattern             const char *                       The pattern of the tests that should be executed
=================== ================================== ==============================================================

if you want criterion to provide its own default CLI parameters and environment
variables handling, you can also call ``criterion_handle_args(int argc, char *argv[], bool handle_unknown_arg)``
with the proper ``argc/argv``. ``handle_unknown_arg``, if set to true, is here
to tell criterion to print its usage when an unknown CLI parameter is encountered.
If you want to add your own parameters, you should set it to false.

The function returns 0 if the main should exit immediately, and 1 if it should
continue.

Starting the test runner
~~~~~~~~~~~~~~~~~~~~~~~~

The test runner can be called with ``criterion_run_all_tests``. The function
returns 0 if one test or more failed, 1 otherwise.

Example main
~~~~~~~~~~~~

.. code-block:: c

    #include <criterion/criterion.h>

    /* This is necessary on windows, as BoxFort needs the main to be exported
       in order to find it. */
    #if defined (_WIN32) || defined (__CYGWIN__)
    # if defined (_MSC_VER)
    #  define DLLEXPORT __declspec(dllexport)
    # elif defined (__GNUC__)
    #  define DLLEXPORT __attribute__((dllexport))
    # else
    #  error No dllexport attribute
    # endif
    #else
    # define DLLEXPORT
    #endif

    DLLEXPORT int main(int argc, char *argv[]) {
        struct criterion_test_set *tests = criterion_initialize();

        int result = 0;
        if (criterion_handle_args(argc, argv, true))
            result = !criterion_run_all_tests(tests);

        criterion_finalize(tests);
        return result;
    }

Implementing your own logger
----------------------------

In case you are not satisfied by the default logger, you can implement
yours. To do so, simply set the ``logger`` option to your custom
logger.

Each function contained in the structure is called during one of the standard
phase of the criterion runner.

For more insight on how to implement this, see other existing loggers
in ``src/log/``.
