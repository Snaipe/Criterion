Testing log messages

  $ log.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 2 tests from logging:
  [RUN ] logging::format
  [----] Log messages are printf-formatted strings.
  [PASS] logging::format
  [RUN ] logging::simple
  [----] This is an informational message. They are not displayed by default.
  [WARN] This is a warning. They indicate some possible malfunction or misconfiguration in the test.
  [ERR ] This is an error. They indicate serious problems and are usually shown before the test is aborted.
  [PASS] logging::simple
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ log.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 1 test from logging:
  [RUN ] logging::stream
  [----] This is an informational message. They are not displayed by default.
  [WARN] This is a warning. They indicate some possible malfunction or misconfiguration in the test.
  [ERR ] This is an error. They indicate serious problems and are usually shown before the test is aborted.
  [PASS] logging::stream
  [====] Synthesis: Tested: 1 | Passing: 1 | Failing: 0 | Crashing: 0 

Testing log severity

  $ log.c.bin
  [WARN] This is a warning. They indicate some possible malfunction or misconfiguration in the test.
  [ERR ] This is an error. They indicate serious problems and are usually shown before the test is aborted.
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 
