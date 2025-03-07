Display the help message

  $ simple.c.bin --help
  Tests compiled with Criterion v.* (re)
  
  usage: .*simple\.c\.bin(\.exe)? OPTIONS (re)
  options: 
      -h or --help: prints this message
      -q or --quiet: disables all logging
      -v or --version: prints the version of criterion these tests have been linked against
      -l or --list: prints all the tests in a list
      -jN or --jobs N: use N concurrent jobs
      -f or --fail-fast: exit after the first failure
      --color=<auto|always|never>: colorize the output
      --encoding=<ENCODING>: use the specified encoding for the output (default: locale-deduced)
      --ascii: don't use fancy unicode symbols or colors in the output
      -S or --short-filename: only display the base name of the source file on a failure
      --filter [PATTERN]: run tests matching the given pattern
      --timeout [TIMEOUT]: set a timeout (in seconds) for all tests
      --tap[=FILE]: writes TAP report in FILE (no file or "-" means stderr)
      --xml[=FILE]: writes XML report in FILE (no file or "-" means stderr)
      --json[=FILE]: writes JSON report in FILE (no file or "-" means stderr)
      --always-succeed: always exit with 0
      --verbose[=level]: sets verbosity to level (1 by default)
      --crash: crash failing assertions rather than aborting (for debugging purposes)
      --debug[=TYPE]: run tests with a debugging server, listening on localhost:1234 by default. TYPE may be gdb, lldb, or windbg.
      --debug-transport=VAL: the transport to use by the debugging server. `tcp:1234` by default
      --full-stats: Tests must fully report statistics (causes massive slowdown for large number of assertions but is more accurate).
      --ignore-warnings: Ignore warnings, do not exit with a non-zero exit status.
      -OPROVIDER:PATH or --output=PROVIDER=PATH: write test report to PATH using the specified provider. If PATH is an existing directory, the report will be created in that directory and will be named after the binary.

C++ equivalents

  $ [ "$CXX_SUPPORT" = 1 ] || exit 80

  $ simple.cc.bin --help
  Tests compiled with Criterion v.* (re)
  
  usage: .*simple\.cc\.bin(\.exe)? OPTIONS (re)
  options: 
      -h or --help: prints this message
      -q or --quiet: disables all logging
      -v or --version: prints the version of criterion these tests have been linked against
      -l or --list: prints all the tests in a list
      -jN or --jobs N: use N concurrent jobs
      -f or --fail-fast: exit after the first failure
      --color=<auto|always|never>: colorize the output
      --encoding=<ENCODING>: use the specified encoding for the output (default: locale-deduced)
      --ascii: don't use fancy unicode symbols or colors in the output
      -S or --short-filename: only display the base name of the source file on a failure
      --filter [PATTERN]: run tests matching the given pattern
      --timeout [TIMEOUT]: set a timeout (in seconds) for all tests
      --tap[=FILE]: writes TAP report in FILE (no file or "-" means stderr)
      --xml[=FILE]: writes XML report in FILE (no file or "-" means stderr)
      --json[=FILE]: writes JSON report in FILE (no file or "-" means stderr)
      --always-succeed: always exit with 0
      --verbose[=level]: sets verbosity to level (1 by default)
      --crash: crash failing assertions rather than aborting (for debugging purposes)
      --debug[=TYPE]: run tests with a debugging server, listening on localhost:1234 by default. TYPE may be gdb, lldb, or windbg.
      --debug-transport=VAL: the transport to use by the debugging server. `tcp:1234` by default
      --full-stats: Tests must fully report statistics (causes massive slowdown for large number of assertions but is more accurate).
      --ignore-warnings: Ignore warnings, do not exit with a non-zero exit status.
      -OPROVIDER:PATH or --output=PROVIDER=PATH: write test report to PATH using the specified provider. If PATH is an existing directory, the report will be created in that directory and will be named after the binary.


