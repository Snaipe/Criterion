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
  [----]   all(eq(i32, 1, 1), eq(i32, 2, 2)): passed, but was expected to fail
  [----]   eq(i32, 1, 1): passed, but was expected to fail
  [----]     actual: 1
  [----]     expected: 1
  [----]   eq(i32, 2, 2): passed, but was expected to fail
  [----]     actual: 2
  [----]     expected: 2
  [FAIL] combinators::not_all_fails
  [====] Synthesis: Tested: 5 | Passing: 3 | Failing: 2 | Crashing: 0 

https://github.com/Snaipe/Criterion/issues/594

  $ bug594.c.bin
  [----] bug594.c:19: Assertion Failed
  [----]   all(eq(int, 1, 1), eq(int, 1, 2)): 
  [----]   eq(int, 1, 2): 
  [----]     diff: [-1-]{+2+}
  [FAIL] bug594::all
  [----] bug594.c:23: Assertion Failed
  [----]   any(eq(int, 1, 2), eq(int, 3, 4)): 
  [----]   eq(int, 1, 2): 
  [----]     diff: [-1-]{+2+}
  [----]   eq(int, 3, 4): 
  [----]     diff: [-3-]{+4+}
  [FAIL] bug594::any
  [----] bug594.c:27: Assertion Failed
  [----]   not(all(eq(int, 1, 1))): 
  [----]   all(eq(int, 1, 1)): passed, but was expected to fail
  [----]   eq(int, 1, 1): passed, but was expected to fail
  [----]     actual: 1
  [----]     expected: 1
  [FAIL] bug594::negated_combinator
  [----] bug594.c:5: Assertion Failed
  [----]   none(eq(int, 1, 1), eq(int, 3, 3), eq(int, 3, 3)): 
  [----]   eq(int, 1, 1): passed, but was expected to fail
  [----]     actual: 1
  [----]     expected: 1
  [----]   eq(int, 3, 3): passed, but was expected to fail
  [----]     actual: 3
  [----]     expected: 3
  [----]   eq(int, 3, 3): passed, but was expected to fail
  [----]     actual: 3
  [----]     expected: 3
  [FAIL] bug594::none
  [----] bug594.c:9: Assertion Failed
  [----]   not(eq(int, 1, 1)): 
  [----]   eq(int, 1, 1): passed, but was expected to fail
  [----]     actual: 1
  [----]     expected: 1
  [FAIL] bug594::not
  [----] bug594.c:15: Assertion Failed
  [----]   not(eq(str, s, s)): 
  [----]   eq(str, s, s): passed, but was expected to fail
  [----]     actual: "a string well over the forty character display threshold"
  [----]     expected: "a string well over the forty character display threshold"
  [FAIL] bug594::not_long
  [====] Synthesis: Tested: 7 | Passing: 1 | Failing: 6 | Crashing: 0 

https://github.com/Snaipe/Criterion/issues/550

  $ bug550.c.bin
  [----] bug550.c:35: Assertion Failed
  [FAIL] bug550::leading_paren_fails
  [====] Synthesis: Tested: 3 | Passing: 2 | Failing: 1 | Crashing: 0 

https://github.com/Snaipe/Criterion/issues/342

  $ bug342.c.bin --verbose
  \[----\] Criterion v.* (re)
  [====] Running 2 tests from bug342_body:
  [RUN ] bug342_body::exit_code
  [SKIP] bug342_body::exit_code: skipped from body
  [RUN ] bug342_body::signal
  [SKIP] bug342_body::signal: skipped from body
  [====] Running 3 tests from bug342_init:
  [RUN ] bug342_init::exit_code
  [SKIP] bug342_init::exit_code: skipped from init
  [RUN ] bug342_init::plain
  [SKIP] bug342_init::plain: skipped from init
  [RUN ] bug342_init::signal
  [SKIP] bug342_init::signal: skipped from init
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 
