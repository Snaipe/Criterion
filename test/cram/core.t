Checking the version

  $ simple.c.bin --version
  Tests compiled with Criterion v2.2.0

  $ simple.cc.bin --version
  Tests compiled with Criterion v2.2.0

Running the simplest test

  $ simple.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

  $ simple.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.cc\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

Running with verbose output

  $ simple.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

  $ simple.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.cc\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

Display the help message

  $ simple.c.bin --help
  Tests compiled with Criterion v2.2.0
  
  usage: simple.c.bin OPTIONS
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
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider

  $ simple.cc.bin --help
  Tests compiled with Criterion v2.2.0
  
  usage: simple.cc.bin OPTIONS
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
      -OP:F or --output=PROVIDER=FILE: write test report to FILE using the specified provider

Listing the tests

  $ simple.c.bin --list
  misc: 2 tests
  ├── failing
  └── passing

  $ simple.cc.bin --list
  misc: 2 tests
  ├── failing
  └── passing

Testing the fixtures

  $ fixtures.c.bin
  Runs before the test
  Runs after the test
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m0\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

  $ fixtures.cc.bin
  Runs before the test
  Runs after the test
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m0\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

Testing descriptions

  $ description.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m]   Just a failing test (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mdescription.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::skipped: Test is disabled (esc)
  [\x1b[0;34m----\x1b[0m]   This one is skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

  $ description.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m]   Just a failing test (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mdescription.cc\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::skipped: Test is disabled (esc)
  [\x1b[0;34m----\x1b[0m]   This one is skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

Testing report hooks

  $ report.c.bin
  criterion_init
  testing test in category sample
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mreport.c\x1b[0m:\x1b[0;31m5\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  Asserts: [1 passed, 1 failed, 2 total]
  [\x1b[0;31mFAIL\x1b[0m] sample::test (esc)
  criterion_fini
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

  $ report.cc.bin
  criterion_init
  testing test in category sample
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mreport.cc\x1b[0m:\x1b[0;31m5\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  Asserts: [1 passed, 1 failed, 2 total]
  [\x1b[0;31mFAIL\x1b[0m] sample::test (esc)
  criterion_fini
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)
