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

* ``assert*`` are assertions that are fatal to the current test if failed;
  in other words, if the condition evaluates to ``false``, the test is
  marked as a failure and the execution of the function is aborted.
* ``expect*`` are, in the other hand, assertions that are not fatal to the
  test. Execution will continue even if the condition evaluates to
  ``false``, but the test will be marked as a failure.

``assert()`` and ``expect()`` are the most simple kinds of assertions
criterion has to offer. They both take a mandatory condition as a first
parameter, and an optional failure message:

.. code-block:: c

    #include <string.h>
    #include <criterion/criterion.h>

    Test(sample, test) {
        expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
        expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
        assert(strlen("") == 0);
    }

On top of those, more assertions are available for common operations:

* ``{assert,expect}_not(Actual, Expected, [Message])``
* ``{assert,expect}_equal(Actual, Expected, [Message])``
* ``{assert,expect}_not_equal(Actual, Unexpected, [Message])``
* ``{assert,expect}_lt(Actual, Expected, [Message])``
* ``{assert,expect}_leq(Actual, Expected, [Message])``
* ``{assert,expect}_gt(Actual, Expected, [Message])``
* ``{assert,expect}_geq(Actual, Expected, [Message])``
* ``{assert,expect}_float_equal(Actual, Expected, Epsilon, [Message])``
* ``{assert,expect}_float_not_equal(Actual, Unexpected, Epsilon, [Message])``
* ``{assert,expect}_strings_equal(Actual, Expected, [Message])``
* ``{assert,expect}_strings_not_equal(Actual, Unexpected, [Message])``
* ``{assert,expect}_strings_lt(Actual, Expected, [Message])``
* ``{assert,expect}_strings_leq(Actual, Expected, [Message])``
* ``{assert,expect}_strings_gt(Actual, Expected, [Message])``
* ``{assert,expect}_strings_geq(Actual, Expected, [Message])``
* ``{assert,expect}_arrays_equal(Actual, Expected, Size, [Message])``
* ``{assert,expect}_arrays_not_equal(Actual, Unexpected, Size, [Message])``

Fixtures
--------

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

Testing signals
---------------

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
