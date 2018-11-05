Testing tests that call exit()

  $ exit.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 3 tests from exit:
  [RUN ] exit::expected_exit
  [PASS] exit::expected_exit
  [RUN ] exit::normal
  [PASS] exit::normal
  [RUN ] exit::unexpected_exit
  [FAIL] exit::unexpected_exit
  [====] Running 2 tests from exit_with_fixtures:
  [RUN ] exit_with_fixtures::fini_exits
  [PASS] exit_with_fixtures::fini_exits
  [----] Warning! The test `exit_with_fixtures::fini_exits` exited during its setup or teardown.
  [RUN ] exit_with_fixtures::init_exits
  [----] Warning! The test `exit_with_fixtures::init_exits` exited during its setup or teardown.
  [====] Synthesis: Tested: 5 | Passing: 3 | Failing: 2 | Crashing: 1 

  $ exit.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 3 tests from exit:
  [RUN ] exit::expected_exit
  [PASS] exit::expected_exit
  [RUN ] exit::normal
  [PASS] exit::normal
  [RUN ] exit::unexpected_exit
  [FAIL] exit::unexpected_exit
  [====] Running 2 tests from exit_with_fixtures:
  [RUN ] exit_with_fixtures::fini_exits
  [PASS] exit_with_fixtures::fini_exits
  [----] Warning! The test `exit_with_fixtures::fini_exits` exited during its setup or teardown.
  [RUN ] exit_with_fixtures::init_exits
  [----] Warning! The test `exit_with_fixtures::init_exits` exited during its setup or teardown.
  [====] Synthesis: Tested: 5 | Passing: 3 | Failing: 2 | Crashing: 1 
