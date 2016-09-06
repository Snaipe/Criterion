Display the help message

  $ simple.c.bin --help
  Tests compiled with Criterion v2.3.0-dev
  
  usage: .*simple\.c\.bin(\.exe)? OPTIONS (re)
  options: 
      -h or --help: prints this message
      -q or --quiet: disables all logging
      -v or --version: prints the version of criterion these tests have been linked against
      -l or --list: prints all the tests in a list
      -jN or --jobs N: use N concurrent jobs
      -f or --fail-fast: exit after the first failure
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
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider

  $ simple.cc.bin --help
  Tests compiled with Criterion v2.3.0-dev
  
  usage: .*simple\.cc\.bin(\.exe)? OPTIONS (re)
  options: 
      -h or --help: prints this message
      -q or --quiet: disables all logging
      -v or --version: prints the version of criterion these tests have been linked against
      -l or --list: prints all the tests in a list
      -jN or --jobs N: use N concurrent jobs
      -f or --fail-fast: exit after the first failure
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
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider


