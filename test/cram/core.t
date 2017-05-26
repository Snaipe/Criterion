Checking the version

  $ simple.c.bin --version
  Tests compiled with Criterion v2.3.0-rc2

  $ simple.cc.bin --version
  Tests compiled with Criterion v2.3.0-rc2

Running the simplest test

  $ simple.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.cc\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Running with verbose output

  $ simple.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.3.0-rc2 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.3.0-rc2 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.cc\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d.\d\ds\) \(esc\) (re)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

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
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ fixtures.cc.bin
  Runs before the test
  Runs after the test
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing descriptions

  $ description.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.3.0-rc2 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m]   Just a failing test (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mdescription.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d.\d\ds\) \(esc\) (re)
  [\x1b[0;33mSKIP\x1b[0m] misc::skipped: Test is disabled (esc)
  [\x1b[0;34m----\x1b[0m]   This one is skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ description.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.3.0-rc2 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m]   Just a failing test (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mdescription.cc\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d.\d\ds\) \(esc\) (re)
  [\x1b[0;33mSKIP\x1b[0m] misc::skipped: Test is disabled (esc)
  [\x1b[0;34m----\x1b[0m]   This one is skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing report hooks

  $ report.c.bin
  criterion_init
  testing test in category sample
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mreport.c\x1b[0m:\x1b[0;31m5\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  Asserts: [1 passed, 1 failed, 2 total]
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] sample::test: \(\d\.\d\ds\) \(esc\) (re)
  criterion_fini
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ report.cc.bin
  criterion_init
  testing test in category sample
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mreport.cc\x1b[0m:\x1b[0;31m5\x1b[0m: Assertion Failed (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  [\x1b[0;34m----\x1b[0m]   The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m]    (esc)
  Asserts: [1 passed, 1 failed, 2 total]
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] sample::test: \(\d\.\d\ds\) \(esc\) (re)
  criterion_fini
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)
