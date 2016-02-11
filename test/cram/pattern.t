Only run on linux

  $ if [ "$CRITERION_TESTS_PATTERN" = "off" ]; then
  >   exit 80
  > fi

Selecting misc::passing

  $ simple.c.bin --pattern '*/passing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Selecting misc::failing

  $ simple.c.bin --pattern '!(*/passing)' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Selecting both misc::passing and misc::failing

  $ simple.c.bin --pattern 'misc/[pf]a@(ss|il)ing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::failing (esc)
  [\x1b[0;34m----\x1b[0m] \x1b[0;1msimple.c\x1b[0m:\x1b[0;31m4\x1b[0m: Assertion failed: The expression 0 is false. (esc)
  [\x1b[0;31mFAIL\x1b[0m] misc::failing (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m1\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing nested extglob patterns

  $ simple.c.bin --pattern '@(+(nest)ed))' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing one or more

  $ simple.c.bin --pattern '?(*(a|b))' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing malformed pattern

  $ simple.c.bin --pattern '?(malformed' --verbose
  pattern error: mismatching parenthesis
  [1]

Testing range negation

  $ simple.c.bin --pattern 'misc/[!azerty]assing' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] misc::passing (esc)
  [\x1b[0;32mPASS\x1b[0m] misc::passing (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing unparenthesized pipe

  $ simple.c.bin --pattern '|pipe' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)

Testing special char escaping

  $ simple.c.bin --pattern '\!(escaped' --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.0 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mmisc\x1b[0m: (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::passing: Test is disabled (esc)
  [\x1b[0;33mSKIP\x1b[0m] misc::failing: Test is disabled (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m0\x1b[0;1m | Passing: \x1b[0;32m0\x1b[0;1m | Failing: \x1b[0m0\x1b[0;1m | Crashing: \x1b[0m0\x1b[0;1m \x1b[0m (esc)
