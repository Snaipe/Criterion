Environment and CLI
===================

Tests built with Criterion expose by default various command line switchs
and environment variables to alter their runtime behaviour.

Command line arguments
----------------------

* ``-h or --help``: Show a help message with the available switches.
* ``-q or --quiet``: Disables all logging.
* ``-v or --version``: Prints the version of criterion that has been
  linked against.
* ``-l or --list``: Print all the tests in a list.
* ``-f or --fail-fast``: Exit after the first test failure.
* ``--ascii``: Don't use fancy unicode symbols or colors in the output.
* ``-jN or --jobs N``: Use ``N`` parallel jobs to run the tests. ``0`` picks
  a number of jobs ideal for your hardware configuration.
* ``--filter [PATTERN]``: Run tests whose string identifier matches
  the given shell wildcard pattern (see dedicated section below). (\*nix only)
* ``--debug[=debugger]``: Run tests with a debugging server attached.
  ``debugger`` can be 'gdb', 'lldb', or 'windbg' (windows only).
* ``--debug-transport [TRANSPORT]``: Make the debugging server use the
  specified remote transport. Only transports of the form ``tcp:port`` are
  currently supported. ``tcp:1234`` is the default.
* ``--no-early-exit``: This flag is deprecated and no longer does anything.
* ``-S or --short-filename``: The filenames are displayed in their short form.
* ``--always-succeed``: The process shall exit with a status of ``0``.
* ``--tap[=FILE]``: Writes a TAP (Test Anything Protocol) report to FILE.
  No file or ``"-"`` means ``stderr`` and implies ``--quiet``. This option is
  equivalent to ``--output=tap:FILE``.
* ``--xml[=FILE]``: Writes JUnit4 XML report to FILE.
  No file or ``"-"`` means ``stderr`` and implies ``--quiet``. This option is
  equivalent to ``--output=tap:FILE``.
* ``--json[=FILE]``: Writes a JSON report to FILE.
  No file or ``"-"`` means ``stderr`` and implies ``--quiet``. This option is
  equivalent to ``--output=tap:FILE``.
* ``--verbose[=level]``: Makes the output verbose. When provided with an integer,
  sets the verbosity level to that integer.
* ``--full-stats``: Forces tests to fully report statistics. By default,
  tests do not report details for passing assertions, so this option forces
  them to do so.
  Activating this causes massive slowdowns for large number of assertions, but
  provides more accurate reports.
* ``-OPROVIDER:FILE or --output=PROVIDER:FILE``: Write a test report to FILE
  using the output provider named by PROVIDER.
  If FILE is ``"-"``, it implies ``--quiet``, and the report shall be written
  to ``stderr``.

Shell Wildcard Pattern
----------------------

Extglob patterns in criterion are matched against a test's string identifier.

In the table below, a ``pattern-list`` is a list of patterns separated by ``|``.
Any extglob pattern can be constructed by combining any of the following
sub-patterns:

==================== ======================================================
Pattern              Meaning
==================== ======================================================
``*``                matches everything
-------------------- ------------------------------------------------------
``?``                matches any character
-------------------- ------------------------------------------------------
``[seq]``            matches any character in *seq*
-------------------- ------------------------------------------------------
``[!seq]``           matches any character not in *seq*
-------------------- ------------------------------------------------------
``?(pattern-list)``  Matches zero or one occurrence of the given patterns
-------------------- ------------------------------------------------------
``*(pattern-list)``  Matches zero or more occurrences of the given patterns
-------------------- ------------------------------------------------------
``+(pattern-list)``  Matches one or more occurrences of the given patterns
-------------------- ------------------------------------------------------
``@(pattern-list)``  Matches one of the given patterns
-------------------- ------------------------------------------------------
``!(pattern-list)``  Matches anything except one of the given patterns
==================== ======================================================

A test string identifier is of the form ``suite-name/test-name``, so a pattern
of ``simple/*`` matches every tests in the ``simple`` suite, ``*/passing``
matches all tests named ``passing`` regardless of the suite, and ``*`` matches
every possible test.

Environment Variables
---------------------

Environment variables are alternatives to command line switches when set to 1.

* ``CRITERION_ALWAYS_SUCCEED``:  Same as ``--always-succeed``.
* ``CRITERION_FAIL_FAST``:       Same as ``--fail-fast``.
* ``CRITERION_USE_ASCII``:       Same as ``--ascii``.
* ``CRITERION_JOBS``:            Same as ``--jobs``. Sets the number of jobs to
  its value.
* ``CRITERION_SHORT_FILENAME``:  Same as ``--short-filename``.
* ``CRITERION_VERBOSITY_LEVEL``: Same as ``--verbose``. Sets the verbosity level
  to its value.
* ``CRITERION_TEST_PATTERN``:    Same as ``--pattern``. Sets the test pattern
  to its value. (\*nix only)
* ``CRITERION_DISABLE_TIME_MEASUREMENTS``: Disables any time measurements on
  the tests.
* ``CRITERION_OUTPUTS``:         Can be set to a comma-separated list of
  ``PROVIDER:FILE`` entries. For instance, setting the variable to
  ``tap:foo.tap,xml:bar.xml`` has the same effect as specifying ``--tap=foo.tap``
  and ``--xml=bar.xml`` at once.
* ``CRITERION_ENABLE_TAP``:      (Deprecated, use CRITERION_OUTPUTS) Same as ``--tap``.
