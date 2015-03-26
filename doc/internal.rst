Changing the internals
======================

Providing your own main
-----------------------

If you are not satisfied with the default CLI or environment variables, you
can define your own main function.

Configuring the test runner
~~~~~~~~~~~~~~~~~~~~~~~~~~~

You'd usually want to configure the test runner before calling it.
Configuration is done by setting fields in a global variable named
``criterion_options`` (include criterion/options.h).

Here is an exhaustive list of these fields:

=================== ================================== ==============================================================
Field               Type                               Description
=================== ================================== ==============================================================
logging_threshold   enum criterion_logging_level       The logging level
------------------- ---------------------------------- --------------------------------------------------------------
output_provider     struct criterion_output_provider * The output provider (see below)
------------------- ---------------------------------- --------------------------------------------------------------
no_early_exit       bool                               True iff the test worker should exit early
------------------- ---------------------------------- --------------------------------------------------------------
always_succeed      bool                               True iff criterion_run_all_tests should always returns 1
------------------- ---------------------------------- --------------------------------------------------------------
use_ascii           bool                               True iff the outputs should use the ASCII charset
------------------- ---------------------------------- --------------------------------------------------------------
fail_fast           bool                               True iff the test runner should abort after the first failure
------------------- ---------------------------------- --------------------------------------------------------------
pattern             const char *                       The pattern of the tests that should be executed
=================== ================================== ==============================================================

Starting the test runner
~~~~~~~~~~~~~~~~~~~~~~~~

The test runner can be called with ``criterion_run_all_tests``. The function
returns 0 if one test or more failed, 1 otherwise.
