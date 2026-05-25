--default-timeout applies to tests with no explicit timeout

  $ default-timeout.c.bin --default-timeout 1 --filter='timeout/no_timeout_set'
  \[FAIL\] timeout::no_timeout_set: Timed out. \([0-9.]*s\) (re)
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

--default-timeout does not override an explicit per-test timeout

  $ default-timeout.c.bin --default-timeout 1 --filter='timeout/explicit_not_overridden'
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 
