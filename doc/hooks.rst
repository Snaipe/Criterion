Report Hooks
============

Report hooks are functions that are called at key moments during the testing
process. These are useful to report statistics gathered during the execution.

A report hook can be declared using the ``ReportHook`` macro:

.. code-block:: c

    #include <criterion/criterion.h>
    #include <criterion/hooks.h>

    ReportHook(Phase)() {
    }

The macro takes a Phase parameter that indicates the phase at which the function
shall be run. Valid phases are described below.

**Note**: there are no guarantees regarding the order of execution of report hooks
on the same phase. In other words, all report hooks of a specific phase could
be executed in any order.

Testing Phases
--------------

The flow of the test process goes as follows:

1. ``PRE_ALL``: occurs before running the tests.
#. ``PRE_SUITE``: occurs before a suite is initialized.
#. ``PRE_INIT``: occurs before a test is initialized.
#. ``PRE_TEST``: occurs after the test initialization, but before the test is run.
#. ``ASSERT``: occurs when an assertion is hit
#. ``THEORY_FAIL``: occurs when a theory iteration fails.
#. ``TEST_CRASH``: occurs when a test crashes unexpectedly.
#. ``POST_TEST``: occurs after a test ends, but before the test finalization.
#. ``POST_FINI``: occurs after a test finalization.
#. ``POST_SUITE``: occurs before a suite is finalized.
#. ``POST_ALL``: occurs after all the tests are done.

Hook Parameters
---------------

A report hook may take zero or one parameter. If a parameter is given, it
is undefined behaviour if it is not a pointer type and not of the proper pointed
type for that phase.

Valid types for each phases are:

* ``struct criterion_test_set *`` for ``PRE_ALL``.
* ``struct criterion_suite_set *`` for ``PRE_SUITE``.
* ``struct criterion_test *`` for ``PRE_INIT`` and ``PRE_TEST``.
* ``struct criterion_assert_stats *`` for ``ASSERT``.
* ``struct criterion_theory_stats *`` for ``THEORY_FAIL``.
* ``struct criterion_test_stats *`` for ``POST_TEST``, ``POST_FINI``, and ``TEST_CRASH``.
* ``struct criterion_suite_stats *`` for ``POST_SUITE``.
* ``struct criterion_global_stats *`` for ``POST_ALL``.

For instance, these are valid report hook declarations for the ``PRE_TEST`` phase:

.. code-block:: c

    #include <criterion/criterion.h>
    #include <criterion/hooks.h>

    ReportHook(PRE_TEST)() {
        // not using the parameter
    }

    ReportHook(PRE_TEST)(struct criterion_test *test) {
        // using the parameter
    }
