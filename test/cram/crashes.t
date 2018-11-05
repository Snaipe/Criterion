Simple crashes & signal testing

  $ signal.c.bin
  [----] signal.c:14: Unexpected signal caught below this line!
  [FAIL] simple::uncaught: CRASH!
  [FAIL] simple::wrong_signal
  [====] Synthesis: Tested: 3 | Passing: 1 | Failing: 2 | Crashing: 1 

  $ signal.cc.bin
  [----] signal.cc:14: Unexpected signal caught below this line!
  [FAIL] simple::uncaught: CRASH!
  [FAIL] simple::wrong_signal
  [====] Synthesis: Tested: 3 | Passing: 1 | Failing: 2 | Crashing: 1 

Simple crashes & signal testing (verbose)

  $ signal.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 3 tests from simple:
  [RUN ] simple::caught
  [PASS] simple::caught
  [RUN ] simple::uncaught
  [----] signal.c:14: Unexpected signal caught below this line!
  [FAIL] simple::uncaught: CRASH!
  [RUN ] simple::wrong_signal
  [FAIL] simple::wrong_signal
  [====] Synthesis: Tested: 3 | Passing: 1 | Failing: 2 | Crashing: 1 

  $ signal.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 3 tests from simple:
  [RUN ] simple::caught
  [PASS] simple::caught
  [RUN ] simple::uncaught
  [----] signal.cc:14: Unexpected signal caught below this line!
  [FAIL] simple::uncaught: CRASH!
  [RUN ] simple::wrong_signal
  [FAIL] simple::wrong_signal
  [====] Synthesis: Tested: 3 | Passing: 1 | Failing: 2 | Crashing: 1 

Crashes outside of the test

  $ other-crashes.c.bin
  [----] Warning! The test `misc::setup_crash` crashed during its setup or teardown.
  [----] Warning! The test `misc::teardown_crash` crashed during its setup or teardown.
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 1 

  $ other-crashes.cc.bin
  [----] Warning! The test `misc::setup_crash` crashed during its setup or teardown.
  [----] Warning! The test `misc::teardown_crash` crashed during its setup or teardown.
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 1 

Crashes outside of the test (verbose)

  $ other-crashes.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::setup_crash
  [----] Warning! The test `misc::setup_crash` crashed during its setup or teardown.
  [RUN ] misc::teardown_crash
  [PASS] misc::teardown_crash
  [----] Warning! The test `misc::teardown_crash` crashed during its setup or teardown.
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 1 

  $ other-crashes.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from misc:
  [RUN ] misc::setup_crash
  [----] Warning! The test `misc::setup_crash` crashed during its setup or teardown.
  [RUN ] misc::teardown_crash
  [PASS] misc::teardown_crash
  [----] Warning! The test `misc::teardown_crash` crashed during its setup or teardown.
  [====] Synthesis: Tested: 2 | Passing: 1 | Failing: 1 | Crashing: 1 

C++ Exceptions should be handled

  $ exception.cc.bin
  [----] exception::raise_any: Caught some unexpected exception during the test execution.
  [FAIL] exception::raise_any
  [----] exception::raise_any_fini: Caught some unexpected exception during the test finalization.
  [----] exception::raise_any_init: Caught some unexpected exception during the test initialization.
  [FAIL] exception::raise_any_init
  [----] exception::raise_std: Caught an unexpected exception during the test execution: Some exception message.
  [FAIL] exception::raise_std
  [----] exception::raise_std_fini: Caught an unexpected exception during the test finalization: Some exception message.
  [----] exception::raise_std_init: Caught an unexpected exception during the test initialization: Some exception message.
  [FAIL] exception::raise_std_init
  [====] Synthesis: Tested: 6 | Passing: 2 | Failing: 4 | Crashing: 0 

C++ Exceptions should be handled (verbose)

  $ exception.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 6 tests from exception:
  [RUN ] exception::raise_any
  [----] exception::raise_any: Caught some unexpected exception during the test execution.
  [FAIL] exception::raise_any
  [RUN ] exception::raise_any_fini
  [PASS] exception::raise_any_fini
  [----] exception::raise_any_fini: Caught some unexpected exception during the test finalization.
  [RUN ] exception::raise_any_init
  [----] exception::raise_any_init: Caught some unexpected exception during the test initialization.
  [FAIL] exception::raise_any_init
  [RUN ] exception::raise_std
  [----] exception::raise_std: Caught an unexpected exception during the test execution: Some exception message.
  [FAIL] exception::raise_std
  [RUN ] exception::raise_std_fini
  [PASS] exception::raise_std_fini
  [----] exception::raise_std_fini: Caught an unexpected exception during the test finalization: Some exception message.
  [RUN ] exception::raise_std_init
  [----] exception::raise_std_init: Caught an unexpected exception during the test initialization: Some exception message.
  [FAIL] exception::raise_std_init
  [====] Synthesis: Tested: 6 | Passing: 2 | Failing: 4 | Crashing: 0 
