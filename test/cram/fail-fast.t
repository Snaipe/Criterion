Testing --fail-fast

  $ simple.c.bin --fail-fast --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----] simple.c:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 
  [1]

Testing CRITERION_FAIL_FAST

  $ CRITERION_FAIL_FAST=1 simple.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----] simple.c:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 
  [1]
