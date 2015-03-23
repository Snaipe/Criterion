Environment and CLI
===================

Tests built with Criterion expose by default various command line switchs
and environment variables to alter their runtime behaviour.

Command line arguments
----------------------

* ``-h or --help``: Show a help message with the available switches.
* ``-v or --version``: Prints the version of criterion that has been
  linked against.
* ``-l or --list``: Print all the tests in a list.
* ``-f or --fail-fast``: Exit after the first test failure.
* ``--ascii``: Don't use fancy unicode symbols or colors in the output.
* ``--pattern [PATTERN]``: Run tests whose string identifier matches
  the given shell wildcard pattern (see dedicated section below).
* ``--no-early-exit``: The test workers shall not prematurely exit when done and
  will properly return from the main, cleaning up their process space.
  This is useful when tracking memory leaks with ``valgrind --tool=memcheck``.
* ``--always-succeed``: The process shall exit with a status of ``0``.
* ``--tap``: Enables the TAP (Test Anything Protocol) output format.
* ``--verbose[=level]``: Makes the output verbose. When provided with an integer,
  sets the verbosity level to that integer.

Shell Wildcard Pattern
----------------------

Patterns in criterion are matched against a test's string identifier with
``fnmatch``.

Special characters used in shell-style wildcard patterns are:

=========== ===================================
Pattern     Meaning
=========== ===================================
``*``       matches everything
----------- -----------------------------------
``?``       matches any character
----------- -----------------------------------
``[seq]``   matches any character in *seq*
----------- -----------------------------------
``[!seq]``  matches any character not in *seq*
=========== ===================================

A test string identifier is of the form ``suite-name/test-name``, so a pattern
of ``simple/*`` matches every tests in the ``simple`` suite, ``*/passing``
matches all tests named ``passing`` regardless of the suite, and ``*`` matches
every possible test.

Environment Variables
---------------------

Environment variables are alternatives to command line switches when set to 1.

* ``CRITERION_ALWAYS_SUCCEED``:  Same as ``--always-succeed``.
* ``CRITERION_NO_EARLY_EXIT``:   Same as ``--no-early-exit``.
* ``CRITERION_ENABLE_TAP``:      Same as ``--tap``.
* ``CRITERION_FAIL_FAST``:       Same as ``--fail-fast``.
* ``CRITERION_USE_ASCII``:       Same as ``--ascii``.
* ``CRITERION_VERBOSITY_LEVEL``: Same as ``--verbose``. Sets the verbosity level
  to its value.
* ``CRITERION_TEST_PATTERN``:    Same as ``--pattern``. Sets the test pattern
  to its value.
