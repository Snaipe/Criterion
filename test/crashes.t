Simple crashes & signal testing

  $ signal.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msignal.c\x1b[0m:\x1b[0;31m16\x1b[0m: Unexpected signal caught below this line! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::uncaught: CRASH! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::wrong_signal (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m3\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0;31m1\x1b[0;1m \x1b[0m (esc)

  $ signal.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msignal.cc\x1b[0m:\x1b[0;31m16\x1b[0m: Unexpected signal caught below this line! (esc)
  [\x1b[0;31mFAIL\x1b[0m] simple::uncaught: CRASH! (esc)
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
