Testing normal globs

  $ simple.c.bin --filter='*/passing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='*/pa?sing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/passing?' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/*' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)




Testing character classes

  $ simple.c.bin --filter='misc/[pf]assing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/[p-s]assing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/[!f]ailing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test was skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/[p-f]ailing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test was skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/[!azerty]assing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)





Testing extended globs

  $ simple.c.bin --filter='!(*/passing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test was skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='@(misc)/passing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --filter='*(misc)/pa+(s)ing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --filter='misc/[pf]a@(ss|il)ing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --filter='!(*/failing|*/fail)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --filter='misc/?(passing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/?(passing|failing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/*!(passing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  \[\\x1b\[0;31mFAIL\\x1b\[0m\] misc::failing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)





Testing nested extglob patterns

  $ simple.c.bin --filter='@(+(nest)ed)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test was skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='?(*(a|b))' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test was skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --filter='misc/pa@(s|*(s))ing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  \[\\x1b\[0;32mPASS\\x1b\[0m\] misc::passing: \(\d\.\d\ds\) \(esc\) (re)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)




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
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test was skipped (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test was skipped (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


