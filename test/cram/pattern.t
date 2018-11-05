Testing normal globs

  $ simple.c.bin --filter='*/passing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='*/pa?sing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/passing?' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/*' --verbose
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

Testing character classes

  $ simple.c.bin --filter='misc/[pf]assing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/[p-s]assing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/[!f]ailing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [SKIP] misc::passing: Test is disabled
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/[p-f]ailing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----] simple.c:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [SKIP] misc::passing: Test is disabled
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

  $ simple.c.bin --filter='misc/[!azerty]assing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

Testing extended globs

  $ simple.c.bin --filter='!(*/passing)' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::failing
  [----] simple.c:4: Assertion Failed
  [----]   
  [----]   The expression 0 is false.
  [----]   
  [FAIL] misc::failing
  [SKIP] misc::passing: Test is disabled
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

  $ simple.c.bin --filter='@(misc)/passing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='*(misc)/pa+(s)ing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/[pf]a@(ss|il)ing' --verbose
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

  $ simple.c.bin --filter='!(*/failing|*/fail)' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/?(passing)' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/?(passing|failing)' --verbose
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

  $ simple.c.bin --filter='misc/*!(passing)' --verbose
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

Testing nested extglob patterns

  $ simple.c.bin --filter='@(+(nest)ed)' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [SKIP] misc::passing: Test is disabled
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='?(*(a|b))' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [SKIP] misc::passing: Test is disabled
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 

  $ simple.c.bin --filter='misc/pa@(s|*(s))ing' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [RUN ] misc::passing
  [PASS] misc::passing
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

Testing malformed pattern

  $ simple.c.bin --filter='?(malformed' --verbose
  pattern error: Mismatching parenthesis
  [3]

  $ simple.c.bin --filter='(malformed' --verbose
  pattern error: Unexpected character '('
  [3]

  $ simple.c.bin --filter='misc/passing)' --verbose
  pattern error: Unexpected character ')'
  [3]

  $ simple.c.bin --filter='misc/[' --verbose
  pattern error: Mismatching brackets.
  [3]

  $ simple.c.bin --filter='misc/[a-' --verbose
  pattern error: Mismatching brackets.
  [3]

  $ simple.c.bin --filter='|pipe' --verbose
  pattern error: Unexpected character '|'
  [3]

Testing special char escaping

  $ simple.c.bin --filter='\!(escaped)' --verbose
  pattern error: Unexpected character '('
  [3]

  $ simple.c.bin --filter='\!escaped' --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [SKIP] misc::failing: Test is disabled
  [SKIP] misc::passing: Test is disabled
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 
