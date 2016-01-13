Test C assertions:

  $ asserts.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m11\x1b[0m: Assertion failed: assert is fatal, expect isn't (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m12\x1b[0m: Assertion failed: This assert runs (esc)
  [\x1b[0;31mFAIL\x1b[0m] asserts::base (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m17\x1b[0m: Assertion failed: You can fail an assertion with a message from anywhere (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m18\x1b[0m: Assertion failed: The conditions for this assertion were not met. (esc)
  [\x1b[0;31mFAIL\x1b[0m] asserts::old_school (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m6\x1b[0;1m | Passing: \x1b[0;32m4\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

Test C++ assertions:

  $ asserts.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m83\x1b[0m: Assertion failed: The expression (&s1)[0 .. 2] == (&s2)[0 .. 2] is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] asserts::array (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m13\x1b[0m: Assertion failed: assert is fatal, expect isn't (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m14\x1b[0m: Assertion failed: This assert runs (esc)
  [\x1b[0;31mFAIL\x1b[0m] asserts::base (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m89\x1b[0m: Assertion failed: The statement `throw std::exception()` did not throw an instance of the `std::bad_alloc` exception. (esc)
  [\x1b[0;31mFAIL\x1b[0m] asserts::exception (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m19\x1b[0m: Assertion failed: You can fail an assertion with a message from anywhere (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m20\x1b[0m: Assertion failed: The conditions for this assertion were not met. (esc)
  [\x1b[0;31mFAIL\x1b[0m] asserts::old_school (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m7\x1b[0;1m | Passing: \x1b[0;32m3\x1b[0;1m | Failing: \x1b[0;31m4\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

