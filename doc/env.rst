Environment and CLI
===================

Tests built with Criterion expose by default various command line switchs
and environment variables to alter their runtime behaviour.

Command line arguments
----------------------

* `-h or --help`: Show a help message with the available switches
* `--no-early-exit`: The test workers shall not prematurely exit when done and
  will properly return from the main, cleaning up their process space.
  This is useful when tracking memory leaks with `valgrind --tool=memcheck`.
* `--always-succeed`: The process shall exit with a status of `0`.
* `--tap`: Enables the TAP (Test Anything Protocol) output format.
* `--verbose[=level]`: Makes the output verbose. When provided with an integer,
  sets the verbosity level to that integer.

Environment Variables
---------------------

Environment variables are alternatives to command line switches when set to 1.

* `CRITERION_ALWAYS_SUCCEED`:  Same as `--always-succeed`.
* `CRITERION_NO_EARLY_EXIT`:   Same as `--no-early-exit`.
* `CRITERION_ENABLE_TAP`:      Same as `--tap`.
* `CRITERION_VERBOSITY_LEVEL`: Same as `--verbose`. Sets the verbosity level
  to its value.
