Test C assertions:

  $ asserts.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m11\x1b[0m: Assertion failed: assert is fatal, expect isn't (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m12\x1b[0m: Assertion failed: This assert runs (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] asserts::base: \(\d.\d\ds\) \(esc\) (re)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m17\x1b[0m: Assertion failed: You can fail an assertion with a message from anywhere (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.c\x1b[0m:\x1b[0;31m18\x1b[0m: Assertion failed: The conditions for this assertion were not met. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] asserts::old_school: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m6\x1b[0;1m | Passing: \x1b[0;32m4\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Test C++ assertions:

  $ asserts.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m105\x1b[0m: Assertion failed: The expression (&s1)[0 .. 2] == (&s2)[0 .. 2] is false. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] asserts::array: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m14\x1b[0m: Assertion failed: assert is fatal, expect isn't (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m15\x1b[0m: Assertion failed: This assert runs (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] asserts::base: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m110\x1b[0m: Assertion failed: The statement `throw std::exception()` did not throw an instance of the `std::bad_alloc` exception. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] asserts::exception: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m20\x1b[0m: Assertion failed: You can fail an assertion with a message from anywhere (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1masserts.cc\x1b[0m:\x1b[0;31m21\x1b[0m: Assertion failed: The conditions for this assertion were not met. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] asserts::old_school: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m7\x1b[0;1m | Passing: \x1b[0;32m3\x1b[0;1m | Failing: \x1b[0;31m4\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Using redirections

  $ redirect.c.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ redirect.cc.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing all assert messages

  $ failmessages.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m8\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m9\x1b[0m: Assertion failed: The expression (0) == (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m10\x1b[0m: Assertion failed: The expression (1) != (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m11\x1b[0m: Assertion failed: The expression (2) < (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m12\x1b[0m: Assertion failed: The expression (2) <= (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m13\x1b[0m: Assertion failed: The expression (1) > (2) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m14\x1b[0m: Assertion failed: The expression (1) >= (2) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m15\x1b[0m: Assertion failed: "" is not null. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m16\x1b[0m: Assertion failed: ((void *)0) is null. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m18\x1b[0m: Assertion failed: The expression (2) - (1) <= (0.1) && (1) - (2) <= (0.1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m19\x1b[0m: Assertion failed: The expression (2) - (2) > (0.1) || (2) - (2) > (0.1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m21\x1b[0m: Assertion failed: "foo" (`foo`) is not empty. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m22\x1b[0m: Assertion failed: "" is empty. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m23\x1b[0m: Assertion failed: The expression (as strings) ("abc") == ("abd") is false: actual=`abc` expected=`abd`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m24\x1b[0m: Assertion failed: The expression (as strings) ("abc") != ("abc") is false: actual=`abc` expected=`abc`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m25\x1b[0m: Assertion failed: The expression (as strings) ("abc") < ("aba") is false: actual=`abc` expected=`aba`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m26\x1b[0m: Assertion failed: The expression (as strings) ("abc") <= ("aba") is false: actual=`abc` expected=`aba`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m27\x1b[0m: Assertion failed: The expression (as strings) ("abc") > ("abd") is false: actual=`abc` expected=`abd`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m28\x1b[0m: Assertion failed: The expression (as strings) ("abc") >= ("abd") is false: actual=`abc` expected=`abd`. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] messages::default: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m32\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m33\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m34\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m35\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m36\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m37\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m38\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m39\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m40\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m42\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m43\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m45\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m46\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m47\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m48\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m49\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m50\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m51\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.c\x1b[0m:\x1b[0;31m52\x1b[0m: Assertion failed: foo bar (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] messages::user: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ failmessages.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m5\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m6\x1b[0m: Assertion failed: The expression (0) == (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m7\x1b[0m: Assertion failed: The expression (1) != (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m8\x1b[0m: Assertion failed: The expression (2) < (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m9\x1b[0m: Assertion failed: The expression (2) <= (1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m10\x1b[0m: Assertion failed: The expression (1) > (2) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m11\x1b[0m: Assertion failed: The expression (1) >= (2) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m12\x1b[0m: Assertion failed: "" is not null. (esc)
  \[\\x1b\[0;34m----\\x1b\[0m\] \\x1b\[0;1mfailmessages\.cc\\x1b\[0m:\\x1b\[0;31m13\\x1b\[0m: Assertion failed: (nullptr|__null) is null\. \(esc\) (re)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m15\x1b[0m: Assertion failed: The expression (2) - (1) <= (0.1) && (1) - (2) <= (0.1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m16\x1b[0m: Assertion failed: The expression (2) - (2) > (0.1) || (2) - (2) > (0.1) is false. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m18\x1b[0m: Assertion failed: "foo" (`foo`) is not empty. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m19\x1b[0m: Assertion failed: "" is empty. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m20\x1b[0m: Assertion failed: The expression (as strings) ("abc") == ("abd") is false: actual=`abc` expected=`abd`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m21\x1b[0m: Assertion failed: The expression (as strings) ("abc") != ("abc") is false: actual=`abc` expected=`abc`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m22\x1b[0m: Assertion failed: The expression (as strings) ("abc") < ("aba") is false: actual=`abc` expected=`aba`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m23\x1b[0m: Assertion failed: The expression (as strings) ("abc") <= ("aba") is false: actual=`abc` expected=`aba`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m24\x1b[0m: Assertion failed: The expression (as strings) ("abc") > ("abd") is false: actual=`abc` expected=`abd`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m25\x1b[0m: Assertion failed: The expression (as strings) ("abc") >= ("abd") is false: actual=`abc` expected=`abd`. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m27\x1b[0m: Assertion failed: The statement `throw std::exception()` did not throw an instance of the `std::bad_alloc` exception. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m28\x1b[0m: Assertion failed: The statement `throw std::exception()` did throw an instance of the `std::exception` exception. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m29\x1b[0m: Assertion failed: The statement `{}` did not throw any exception. (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m30\x1b[0m: Assertion failed: The statement `throw std::exception()` threw some exception. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] messages::default: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m34\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m35\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m36\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m37\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m38\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m39\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m40\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m41\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m42\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m44\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m45\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m47\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m48\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m49\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m50\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m51\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m52\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m53\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m54\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m56\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m57\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m58\x1b[0m: Assertion failed: foo bar (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mfailmessages.cc\x1b[0m:\x1b[0;31m59\x1b[0m: Assertion failed: foo bar (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] messages::user: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m2\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Long assert messages should wrap

  $ long-messages.c.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mlong-messages.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: This is (esc)
  [\x1b[0;34m----\x1b[0m]   A long message (esc)
  [\x1b[0;34m----\x1b[0m]   Spawning multiple lines. (esc)
  [\x1b[0;34m----\x1b[0m]   Formatting is respected. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] sample::long_msg: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ long-messages.cc.bin
  [\x1b[0;34m----\x1b[0m] \x1b[0;1mlong-messages.cc\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: This is (esc)
  [\x1b[0;34m----\x1b[0m]   A long message (esc)
  [\x1b[0;34m----\x1b[0m]   Spawning multiple lines. (esc)
  [\x1b[0;34m----\x1b[0m]   Formatting is respected. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] sample::long_msg: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Using redirections

  $ redirect.c.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ redirect.cc.bin
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)
