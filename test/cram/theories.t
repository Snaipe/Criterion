Using theories

  $ theories_regression.c.bin
  [----] theories_regression.c:34: Assertion Failed
  [----]   
  [----]   The conditions for this assertion were not met.
  [----]   
  [----]   Theory theory::misc failed with the following parameters: ('a', true, 1, 1, 3.14f, 3.14, "test", "other test")
  [FAIL] theory::misc
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

  $ theories_regression.cc.bin
  [----] theories_regression.cc:36: Assertion Failed
  [----]   
  [----]   The conditions for this assertion were not met.
  [----]   
  [----]   Theory theory::misc failed with the following parameters: ('a', true, 1, 1, 3.14f, 3.14, "test", "other test")
  [FAIL] theory::misc
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

Theories should be listed as only one test

  $ theories_regression.c.bin --list
  theory: 1 test
  `-- misc
