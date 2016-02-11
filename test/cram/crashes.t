Simple crashes & signal testing

  $ signal.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msignal.c\x1b[0m:\x1b[0;31m14\x1b[0m: Unexpected signal caught below this line! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::uncaught: CRASH! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::wrong_signal (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m3\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

  $ signal.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msignal.cc\x1b[0m:\x1b[0;31m14\x1b[0m: Unexpected signal caught below this line! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::uncaught: CRASH! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::wrong_signal (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m3\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

Simple crashes & signal testing (verbose)

  $ signal.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m3\x1b[0m tests from \x1b[0;33msimple\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] simple::caught (esc)
  [\x1b[0;32mPASS\x1b[0m] simple::caught (esc)
  [\x1b[0;34mRUN \x1b[0m] simple::uncaught (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msignal.c\x1b[0m:\x1b[0;31m14\x1b[0m: Unexpected signal caught below this line! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::uncaught: CRASH! (esc)
  [\x1b[0;34mRUN \x1b[0m] simple::wrong_signal (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::wrong_signal (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m3\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

  $ signal.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m3\x1b[0m tests from \x1b[0;33msimple\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] simple::caught (esc)
  [\x1b[0;32mPASS\x1b[0m] simple::caught (esc)
  [\x1b[0;34mRUN \x1b[0m] simple::uncaught (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msignal.cc\x1b[0m:\x1b[0;31m14\x1b[0m: Unexpected signal caught below this line! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::uncaught: CRASH! (esc)
  [\x1b[0;34mRUN \x1b[0m] simple::wrong_signal (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::wrong_signal (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m3\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

Crashes outside of the test

  $ other-crashes.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::setup_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::teardown_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

  $ other-crashes.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::setup_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::teardown_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

Crashes outside of the test (verbose)

  $ other-crashes.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::setup_crash (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::setup_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::teardown_crash (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::teardown_crash (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::teardown_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

  $ other-crashes.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::setup_crash (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::setup_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::teardown_crash (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::teardown_crash (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mWarning! The test `misc::teardown_crash` crashed during its setup or teardown.\x1b[0m (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

C++ Exceptions should be handled

  $ exception.cc.bin
  [\x1b[0;34m----\x1b[0m] exception::raise_any: Caught some unexpected exception during the test execution. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_any (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_any_fini: Caught some unexpected exception during the test finalization. (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_any_init: Caught some unexpected exception during the test initialization. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_any_init (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_std: Caught an unexpected exception during the test execution: Some exception message. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_std (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_std_fini: Caught an unexpected exception during the test finalization: Some exception message. (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_std_init: Caught an unexpected exception during the test initialization: Some exception message. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_std_init (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m6\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0;31m4\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

C++ Exceptions should be handled (verbose)

  $ exception.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m6\x1b[0m tests from \x1b[0;33mexception\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] exception::raise_any (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_any: Caught some unexpected exception during the test execution. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_any (esc)
  [\x1b[0;34mRUN \x1b[0m] exception::raise_any_fini (esc)
  [\x1b[0;32mPASS\x1b[0m] exception::raise_any_fini (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_any_fini: Caught some unexpected exception during the test finalization. (esc)
  [\x1b[0;34mRUN \x1b[0m] exception::raise_any_init (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_any_init: Caught some unexpected exception during the test initialization. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_any_init (esc)
  [\x1b[0;34mRUN \x1b[0m] exception::raise_std (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_std: Caught an unexpected exception during the test execution: Some exception message. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_std (esc)
  [\x1b[0;34mRUN \x1b[0m] exception::raise_std_fini (esc)
  [\x1b[0;32mPASS\x1b[0m] exception::raise_std_fini (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_std_fini: Caught an unexpected exception during the test finalization: Some exception message. (esc)
  [\x1b[0;34mRUN \x1b[0m] exception::raise_std_init (esc)
  [\x1b[0;34m----\x1b[0m] exception::raise_std_init: Caught an unexpected exception during the test initialization: Some exception message. (esc)
  [\x1b[0;31mFAIL\x1b[0m] exception::raise_std_init (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m6\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0;31m4\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)
