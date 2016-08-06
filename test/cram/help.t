Display the help message

  $ simple.c.bin --help
  Tests compiled with Criterion v2.2.1
  
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
      --pattern [PATTERN]: run tests matching the given pattern
      --tap[=FILE]: writes TAP report in FILE (no file or "-" means stderr)
      --xml[=FILE]: writes XML report in FILE (no file or "-" means stderr)
      --always-succeed: always exit with 0
      --no-early-exit: do not exit the test worker prematurely after the test
      --verbose[=level]: sets verbosity to level (1 by default)
      --crash: crash failing assertions rather than aborting (for debugging purposes)
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider

  $ simple.cc.bin --help
  Tests compiled with Criterion v2.2.1
  
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
      --pattern [PATTERN]: run tests matching the given pattern
      --tap[=FILE]: writes TAP report in FILE (no file or "-" means stderr)
      --xml[=FILE]: writes XML report in FILE (no file or "-" means stderr)
      --always-succeed: always exit with 0
      --no-early-exit: do not exit the test worker prematurely after the test
      --verbose[=level]: sets verbosity to level (1 by default)
      --crash: crash failing assertions rather than aborting (for debugging purposes)
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider


