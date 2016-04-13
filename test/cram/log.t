Testing log messages

  $ log.c.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m2\x1b[0m tests from \x1b[0;33mlogging\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] logging::format (esc)
  [\x1b[0;34m----\x1b[0m] Log messages are printf-formatted strings. (esc)
  [\x1b[0;32mPASS\x1b[0m] logging::format (esc)
  [\x1b[0;34mRUN \x1b[0m] logging::simple (esc)
  [\x1b[0;34m----\x1b[0m] This is an informational message. They are not displayed by default. (esc)
  [\x1b[0;33mWARN\x1b[0m] This is a warning. They indicate some possible malfunction or misconfiguration in the test. (esc)
  [\x1b[0;31mERR \x1b[0m] \x1b[0;31m\x1b[0;1mThis is an error. They indicate serious problems and are usually shown before the test is aborted.\x1b[0m (esc)
  [\x1b[0;32mPASS\x1b[0m] logging::simple (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0;31m0\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

  $ log.cc.bin --verbose
  [\x1b[0;34m----\x1b[0m] Criterion v2.2.1 (esc)
  [\x1b[0;34m====\x1b[0m] Running \x1b[0;34m1\x1b[0m test from \x1b[0;33mlogging\x1b[0m: (esc)
  [\x1b[0;34mRUN \x1b[0m] logging::stream (esc)
  [\x1b[0;34m----\x1b[0m] This is an informational message. They are not displayed by default. (esc)
  [\x1b[0;33mWARN\x1b[0m] This is a warning. They indicate some possible malfunction or misconfiguration in the test. (esc)
  [\x1b[0;31mERR \x1b[0m] \x1b[0;31m\x1b[0;1mThis is an error. They indicate serious problems and are usually shown before the test is aborted.\x1b[0m (esc)
  [\x1b[0;32mPASS\x1b[0m] logging::stream (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m1\x1b[0;1m | Passing: \x1b[0;32m1\x1b[0;1m | Failing: \x1b[0;31m0\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)

Testing log severity

  $ log.c.bin
  [\x1b[0;33mWARN\x1b[0m] This is a warning. They indicate some possible malfunction or misconfiguration in the test. (esc)
  [\x1b[0;31mERR \x1b[0m] \x1b[0;31m\x1b[0;1mThis is an error. They indicate serious problems and are usually shown before the test is aborted.\x1b[0m (esc)
  [\x1b[0;34m====\x1b[0m] \x1b[0;1mSynthesis: Tested: \x1b[0;34m2\x1b[0;1m | Passing: \x1b[0;32m2\x1b[0;1m | Failing: \x1b[0;31m0\x1b[0;1m | Crashing: \x1b[0;31m0\x1b[0;1m \x1b[0m (esc)
