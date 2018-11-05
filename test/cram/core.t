Checking the version

  $ simple.c.bin --version
  Tests compiled with Criterion v2.3.3

  $ simple.cc.bin --version
  Tests compiled with Criterion v2.3.3

Running the simplest test

  $ simple.c.bin
  [----] simple.c:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 0 

  $ simple.cc.bin
  [----] simple.cc:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 0 

Running with verbose output

  $ simple.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----] simple.c:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 0 

  $ simple.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----] simple.cc:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 0 

Listing the tests

  $ simple.c.bin --list
  misc: 2 tests
  |-- failing
  `-- passing

  $ simple.cc.bin --list
  misc: 2 tests
  |-- failing
  `-- passing

Testing the fixtures

  $ fixtures.c.bin
  Runs before the test
  Runs after the test
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ fixtures.cc.bin
  Runs before the test
  Runs after the test
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

Testing descriptions

  $ description.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----]   Just a failing test
  [----] description.c:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [SKIP] misc::skipped: Test is disabled
  [----]   This one is skipped
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

  $ description.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----]   Just a failing test
  [----] description.cc:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [SKIP] misc::skipped: Test is disabled
  [----]   This one is skipped
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

Testing report hooks

  $ report.c.bin
  criterion_init
  testing test in category sample
  [----] report.c:5: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  Asserts: [1 passed, 1 failed, 2 total]
  [FAIL] sample::test
  criterion_fini
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

  $ report.cc.bin
  criterion_init
  testing test in category sample
  [----] report.cc:5: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  Asserts: [1 passed, 1 failed, 2 total]
  [FAIL] sample::test
  criterion_fini
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 
