https://github.com/Snaipe/Criterion/issues/463

  $ bug463.c.bin
  [----] bug463.c:16: Assertion Failed
  [----]   eq(type(struct T1), (struct T1){}, (struct T1){}): 
  [----]     @@@ <no message or difference -- this is a user bug in the object stringifier>
  [FAIL] bug463::nl
  [----] bug463.c:31: Assertion Failed
  [----]   eq(type(struct T2), (struct T2){}, (struct T2){}): 
  [----]     @@@ <no difference -- this is a user bug in the object stringifier>
  [FAIL] bug463::nonl
  [====] Synthesis: Tested: 2 | Passing: 0 | Failing: 2 | Crashing: 0 

