Getting started
===============

Adding tests
------------

Adding tests is done using the ``Test`` macro:

.. code-block:: c

    #include <criterion/criterion.h>

    Test(suite_name, test_name) {
        // test contents
    }

``suite_name`` and ``test_name`` are the identifiers of the test suite and
the test, respectively. These identifiers must follow the language
identifier format.

Tests are automatically sorted by suite, then by name using the alphabetical
order.

Asserting things
----------------

Assertions come in two kinds:

* ``cr_assert*`` are assertions that are fatal to the current test if failed;
  in other words, if the condition evaluates to ``false``, the test is
  marked as a failure and the execution of the function is aborted.
* ``cr_expect*`` are, in the other hand, assertions that are not fatal to the
  test. Execution will continue even if the condition evaluates to
  ``false``, but the test will be marked as a failure.

``cr_assert()`` and ``cr_expect()`` are the most simple kinds of assertions
criterion has to offer. They both take a mandatory condition as a first
parameter, and an optional failure message:

.. code-block:: c

    #include <string.h>
    #include <criterion/criterion.h>

    Test(sample, test) {
        cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
        cr_expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
        cr_assert(strlen("") == 0);
    }

On top of those, more assertions are available for common operations:

* ``cr_assert_null(Ptr, [Message])``: passes if Ptr is NULL.
* ``cr_assert_eq(Actual, Expected, [Message])``: passes if Actual == Expected.
* ``cr_assert_lt(Actual, Expected, [Message])``: passes if Actual < Expected.
* ``cr_assert_leq(Actual, Expected, [Message])``: passes if Actual <= Expected.
* ``cr_assert_gt(Actual, Expected, [Message])``: passes if Actual > Expected.
* ``cr_assert_geq(Actual, Expected, [Message])``: passes if Actual >= Expected.
* ``cr_assert_float_eq(Actual, Expected, Epsilon, [Message])``:
  passes if Actual == Expected with an error of Epsilon.
* ``cr_assert_arrays_eq(Actual, Expected, Size, [Message])``:
  passes if all elements of Actual (from 0 to Size - 1) are equals to those
  of Expected.
* ``cr_assert_arrays_eq_cmp(Actual, Expected, Size, Cmp, [Message])``:
  Same as ``arrays_eq`` but equality is defined by the result of the binary
  Cmp function.

Equality and lexical comparison assertions are also available for strings:

* ``cr_assert_strings_eq(Actual, Expected, [Message])``
* ``cr_assert_strings_lt(Actual, Expected, [Message])``
* ``cr_assert_strings_leq(Actual, Expected, [Message])``
* ``cr_assert_strings_gt(Actual, Expected, [Message])``
* ``cr_assert_strings_geq(Actual, Expected, [Message])``

And some assertions have a logical negative counterpart:

* ``cr_assert_not(Condition, [Message])``
* ``cr_assert_not_null(Ptr, [Message])``
* ``cr_assert_neq(Actual, Unexpected, [Message])``
* ``cr_assert_float_neq(Actual, Unexpected, Epsilon, [Message])``
* ``cr_assert_strings_neq(Actual, Unexpected, [Message])``
* ``cr_assert_arrays_neq(Actual, Unexpected, Size, [Message])``
* ``cr_assert_arrays_neq_cmp(Actual, Unexpected, Size, Cmp, [Message])``

Of course, every ``assert`` has an ``expect`` counterpart.

Please note that ``arrays_(n)eq`` assertions should not be used on padded
structures -- please use ``arrays_(n)eq_cmp`` instead.

Configuring tests
-----------------

Tests may receive optional configuration parameters to alter their behaviour
or provide additional metadata.

Fixtures
~~~~~~~~

Tests that need some setup and teardown can register functions that will
run before and after the test function:

.. code-block:: c

    #include <stdio.h>
    #include <criterion/criterion.h>

    void setup(void) {
        puts("Runs before the test");
    }

    void teardown(void) {
        puts("Runs after the test");
    }

    Test(suite_name, test_name, .init = setup, .fini = teardown) {
        // test contents
    }

If a setup crashes, you will get a warning message, and the test will be aborted
and marked as a failure.
Is a teardown crashes, you will get a warning message, and the test will keep
its result.

Testing signals
~~~~~~~~~~~~~~~

If a test receives a signal, it will by default be marked as a failure.
You can, however, expect a test to only pass if a special kind of signal
is received:

.. code-block:: c

    #include <stddef.h>
    #include <signal.h>
    #include <criterion/criterion.h>

    // This test will fail
    Test(sample, failing) {
        int *ptr = NULL;
        *ptr = 42;
    }

    // This test will pass
    Test(sample, passing, .signal = SIGSEGV) {
        int *ptr = NULL;
        *ptr = 42;
    }

This feature will also work (to some extent) on Windows for the
following signals on some exceptions:

======== =====================================================================
Signal   Triggered by
======== =====================================================================
SIGSEGV  STATUS_ACCESS_VIOLATION, STATUS_DATATYPE_MISALIGNMENT,
         STATUS_ARRAY_BOUNDS_EXCEEDED, STATUS_GUARD_PAGE_VIOLATION,
         STATUS_IN_PAGE_ERROR, STATUS_NO_MEMORY, STATUS_INVALID_DISPOSITION,
         STATUS_STACK_OVERFLOW
-------- ---------------------------------------------------------------------
SIGILL   STATUS_ILLEGAL_INSTRUCTION, STATUS_PRIVILEGED_INSTRUCTION,
         STATUS_NONCONTINUABLE_EXCEPTION
-------- ---------------------------------------------------------------------
SIGINT   STATUS_CONTROL_C_EXIT
-------- ---------------------------------------------------------------------
SIGFPE   STATUS_FLOAT_DENORMAL_OPERAND, STATUS_FLOAT_DIVIDE_BY_ZERO,
         STATUS_FLOAT_INEXACT_RESULT, STATUS_FLOAT_INVALID_OPERATION,
         STATUS_FLOAT_OVERFLOW, STATUS_FLOAT_STACK_CHECK,
         STATUS_FLOAT_UNDERFLOW, STATUS_INTEGER_DIVIDE_BY_ZERO,
         STATUS_INTEGER_OVERFLOW
-------- ---------------------------------------------------------------------
SIGALRM  STATUS_TIMEOUT
======== =====================================================================

See the `windows exception reference`_ for more details on each exception.

.. _windows exception reference: https://msdn.microsoft.com/en-us/library/windows/desktop/ms679356(v=vs.85).aspx

Configuration reference
~~~~~~~~~~~~~~~~~~~~~~~

Here is an exhaustive list of all possible configuration parameters you can
pass:

============= =============== ==============================================================
Parameter     Type            Description
============= =============== ==============================================================
.description  const char *    Adds a description. Cannot be ``NULL``.
------------- --------------- --------------------------------------------------------------
.init         void (*)(void)  Adds a setup function the be executed before the test.
------------- --------------- --------------------------------------------------------------
.fini         void (*)(void)  Adds a teardown function the be executed after the test.
------------- --------------- --------------------------------------------------------------
.disabled     bool            Disables the test.
------------- --------------- --------------------------------------------------------------
.signal       int             Expect the test to raise the specified signal.
------------- --------------- --------------------------------------------------------------
.exit_code    int             Expect the test to exit with the specified status.
============= =============== ==============================================================

Setting up suite-wise configuration
-----------------------------------

Tests under the same suite can have a suite-wise configuration -- this is done
using the ``TestSuite`` macro:

.. code-block:: c

    #include <criterion/criterion.h>

    TestSuite(suite_name, [params...]);

    Test(suite_name, test_1) {
    }

    Test(suite_name, test_2) {
    }

Configuration parameters are the same as above, but applied to the suite itself.

Suite fixtures are run *along with* test fixtures.
