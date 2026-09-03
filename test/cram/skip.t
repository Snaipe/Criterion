Testing skipping

  $ skip.c.bin --verbose
  \[----\] Criterion v.* (re)
  [====] Running 2 tests from misc:
  [RUN ] misc::message
  [SKIP] misc::message: Skips may take printf-like messages
  [RUN ] misc::skipping
  [SKIP] misc::skipping: Test was skipped
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 

  $ skip.c.bin --show-skipped
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 | Skipped: 2 

  $ CRITERION_SHOW_SKIPPED=1 skip.c.bin
  [====] Synthesis: Tested: 0 | Passing: 0 | Failing: 0 | Crashing: 0 | Skipped: 2 
