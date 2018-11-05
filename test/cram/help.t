Display the help message

  $ simple.c.bin --help
  Tests compiled with Criterion v2.3.3
  
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
      --always-succeed: always exit with 0
      --verbose[=level]: sets verbosity to level (1 by default)
      --crash: crash failing assertions rather than aborting (for debugging purposes)
      --debug[=TYPE]: run tests with a debugging server, listening on localhost:1234 by default. TYPE may be gdb, lldb, or wingbd.
      --debug-transport=VAL: the transport to use by the debugging server. `tcp:1234` by default
      --full-stats: Tests must fully report statistics (causes massive slowdown for large number of assertions but is more accurate).
      --ignore-warnings: Ignore warnings, do not exit with a non-zero exit status.
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider

  $ simple.cc.bin --help
  Tests compiled with Criterion v2.3.3
  
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
      --always-succeed: always exit with 0
      --verbose[=level]: sets verbosity to level (1 by default)
      --crash: crash failing assertions rather than aborting (for debugging purposes)
      --debug[=TYPE]: run tests with a debugging server, listening on localhost:1234 by default. TYPE may be gdb, lldb, or wingbd.
      --debug-transport=VAL: the transport to use by the debugging server. `tcp:1234` by default
      --full-stats: Tests must fully report statistics (causes massive slowdown for large number of assertions but is more accurate).
      --ignore-warnings: Ignore warnings, do not exit with a non-zero exit status.
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider

Display usage on invalid CLI flags

  $ simple.c.bin --an-invalid-option
  .*simple\.c\.bin(\.exe)?: (unknown|unrecognized) option [`'"]?-- ?an-invalid-option['"]? (re)
  Tests compiled with Criterion v2.3.3
  
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
      --always-succeed: always exit with 0
      --verbose[=level]: sets verbosity to level (1 by default)
      --crash: crash failing assertions rather than aborting (for debugging purposes)
      --debug[=TYPE]: run tests with a debugging server, listening on localhost:1234 by default. TYPE may be gdb, lldb, or wingbd.
      --debug-transport=VAL: the transport to use by the debugging server. `tcp:1234` by default
      --full-stats: Tests must fully report statistics (causes massive slowdown for large number of assertions but is more accurate).
      --ignore-warnings: Ignore warnings, do not exit with a non-zero exit status.
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider
