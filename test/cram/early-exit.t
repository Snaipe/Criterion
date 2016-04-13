Testing --no-early-exit

  $ simple.c.bin --no-early-exit
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing CRITERION_NO_EARLY_EXIT

  $ CRITERION_NO_EARLY_EXIT=0 simple.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)
