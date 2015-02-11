Environment and CLI
===================

Tests built with Criterion support environment variables to alter
their runtime behaviour.

Environment Variables
---------------------

* `CRITERION_ALWAYS_SUCCEED`: when set to `1`, the exit status of the test
  process will be 0, regardless if the tests failed or not.
* `CRITERION_NO_EARLY_EXIT`: when set to `1`, the test workers shall not
  call `_exit` when done and will properly return from the main and
  clean up their process space. This is useful when tracking memory leaks with
  `valgrind --tool=memcheck`.
