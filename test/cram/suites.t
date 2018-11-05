Using test suites

  $ suites.c.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ suites.cc.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ more-suites.c.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ more-suites.cc.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

Listing tests in multiple suites

  $ suites.c.bin --list
  first_suite: 1 test
  `-- test
  second_suite: 1 test
  `-- test

Running the tests with verbose output

  $ suites.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 1 test from first_suite:
  [RUN ] first_suite::test
  [PASS] first_suite::test
  [====] Running 1 test from second_suite:
  [RUN ] second_suite::test
  [PASS] second_suite::test
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ more-suites.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 1 test from disabled:
  [SKIP] disabled::test: Test is disabled
  [====] Running 1 test from suite1:
  [RUN ] suite1::test
  [PASS] suite1::test
  [====] Running 1 test from suite2:
  [RUN ] suite2::test
  [PASS] suite2::test
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 
