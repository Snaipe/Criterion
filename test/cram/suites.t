Using test suites

  $ suites.c.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ suites.cc.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ more-suites.c.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ more-suites.cc.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Listing tests in multiple suites

  $ suites.c.bin --list
  first_suite: 1 test
  └── test
  second_suite: 1 test
  └── test

Running the tests with verbose output

  $ suites.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.3.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m1\x1b[0m test from \x1b[0;33mfirst_suite\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] first_suite::test (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] first_suite::test: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m1\x1b[0m test from \x1b[0;33msecond_suite\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] second_suite::test (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] second_suite::test: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ more-suites.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.3.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m1\x1b[0m test from \x1b[0;33mdisabled\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] disabled::test: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m1\x1b[0m test from \x1b[0;33msuite1\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] suite1::test (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] suite1::test: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m1\x1b[0m test from \x1b[0;33msuite2\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] suite2::test (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] suite2::test: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)
