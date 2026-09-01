https://github.com/Snaipe/Criterion/issues/463

  $ bug463.c.bin
  [----] bug463.c:16: Assertion Failed
  [----]   eq(type(struct T1), (struct T1){}, (struct T1){}): 
  [----]     @@@ <no difference -- this is a user bug in the object stringifier>
  [FAIL] bug463::nl
  [----] bug463.c:31: Assertion Failed
  [----]   eq(type(struct T2), (struct T2){}, (struct T2){}): 
  [----]     @@@ <no difference -- this is a user bug in the object stringifier>
  [FAIL] bug463::nonl
  [====] Synthesis: Tested: 2 | Passing: 0 | Failing: 2 | Crashing: 0 

Nested assertion combinators must resolve and compose correctly

  $ combinators.c.bin
  [----] combinators.c:26: Assertion Failed
  [----]   any(eq(i32, 1, 0), all(eq(i32, 1, 0), eq(i32, 1, 1))): 
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [----]   all(eq(i32, 1, 0), eq(i32, 1, 1)): 
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [FAIL] combinators::nested_any_fails
  [----] combinators.c:22: Assertion Failed
  [----]   not(all(eq(i32, 1, 1), eq(i32, 2, 2))): 
  [----]   all(eq(i32, 1, 1), eq(i32, 2, 2)): 
  [----]   eq(i32, 1, 1): 
  [----]     @@@ <no difference -- this is a user bug in the object stringifier>
  [----]   eq(i32, 2, 2): 
  [----]     @@@ <no difference -- this is a user bug in the object stringifier>
  [FAIL] combinators::not_all_fails
  [====] Synthesis: Tested: 5 | Passing: 3 | Failing: 2 | Crashing: 0 
