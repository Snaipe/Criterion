Testing normal globs

  $ simple.c.bin --pattern '*/passing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern '*/pa?sing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/passing?' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/*' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)




Testing character classes

  $ simple.c.bin --pattern 'misc/[pf]assing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/[p-s]assing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/[!f]ailing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/[p-f]ailing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/[!azerty]assing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)





Testing extended globs

  $ simple.c.bin --pattern '!(*/passing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern '@(misc)/passing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --pattern '*(misc)/pa+(s)ing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --pattern 'misc/[pf]a@(ss|il)ing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --pattern '!(*/failing|*/fail)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


  $ simple.c.bin --pattern 'misc/?(passing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/?(passing|failing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/*!(passing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)





Testing nested extglob patterns

  $ simple.c.bin --pattern '@(+(nest)ed)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern '?(*(a|b))' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

  $ simple.c.bin --pattern 'misc/pa@(s|*(s))ing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)




Testing malformed pattern

  $ simple.c.bin --pattern '?(malformed' --verbose
  pattern error: Mismatching parenthesis
  [3]

  $ simple.c.bin --pattern '(malformed' --verbose
  pattern error: Unexpected character '('
  [3]

  $ simple.c.bin --pattern 'misc/passing)' --verbose
  pattern error: Unexpected character ')'
  [3]

  $ simple.c.bin --pattern 'misc/[' --verbose
  pattern error: Mismatching brackets.
  [3]

  $ simple.c.bin --pattern 'misc/[a-' --verbose
  pattern error: Mismatching brackets.
  [3]

  $ simple.c.bin --pattern '|pipe' --verbose
  pattern error: Unexpected character '|'
  [3]


Testing special char escaping

  $ simple.c.bin --pattern '\!(escaped)' --verbose
  pattern error: Unexpected character '('
  [3]

  $ simple.c.bin --pattern '\!escaped' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)


