Testing multiple samples with --tap

  $ simple.c.bin --tap
  TAP version 13
  1..2
  # Criterion v.* (re)
  
  # Running 2 tests from misc
  ok - misc::passing 
  not ok - misc::failing 
    ---
    assertions: 1
    failures:
    - simple.c:4: |+
        Assertion failed: The expression 0 is false.
    ...

  $ signal.c.bin --tap
  TAP version 13
  1..3
  # Criterion v.* (re)
  
  # Running 3 tests from simple
  not ok - simple::wrong_signal 
    ---
    assertions: 0
    failures:
    ...
  not ok - simple::uncaught unexpected signal after signal.c:14
  ok - simple::caught 

  $ asserts.c.bin --tap
  TAP version 13
  1..8
  # Criterion v.* (re)
  
  # Running 8 tests from asserts
  ok - asserts::wstring 
  ok - asserts::string 
  ok - asserts::stream 
  not ok - asserts::old_school 
    ---
    assertions: 2
    failures:
    - asserts.c:19: |+
        Assertion failed: (null)
    - asserts.c:18: |+
        Assertion failed: You can fail an assertion with a message from anywhere
    ...
  ok - asserts::native 
  ok - asserts::float 
  not ok - asserts::base 
    ---
    assertions: 2
    failures:
    - asserts.c:13: |+
        Assertion failed: This assert runs
    - asserts.c:12: |+
        Assertion failed: assert is fatal, expect isn't
    ...
  ok - asserts::array 

  $ more-suites.c.bin --tap
  TAP version 13
  1..3
  # Criterion v.* (re)
  
  # Running 1 tests from suite2
  ok - suite2::test 
  
  # Running 1 tests from suite1
  ok - suite1::test 
  
  # Running 1 tests from disabled
  ok - disabled::test  # SKIP test was skipped

  $ long-messages.c.bin --tap
  TAP version 13
  1..1
  # Criterion v.* (re)
  
  # Running 1 tests from sample
  not ok - sample::long_msg 
    ---
    assertions: 1
    failures:
    - long-messages.c:4: |+
        Assertion failed: This is
        A long message
        Spawning multiple lines.
        Formatting is respected.
    ...

  $ description.c.bin --tap
  TAP version 13
  1..2
  # Criterion v.* (re)
  
  # Running 2 tests from misc
  ok - misc::skipped This one is skipped # SKIP test was skipped
  not ok - misc::failing Just a failing test
    ---
    assertions: 1
    failures:
    - description.c:4: |+
        Assertion failed: The expression 0 is false.
    ...

Testing --output=tap

  $ simple.c.bin --output=tap:-
  TAP version 13
  1..2
  # Criterion v.* (re)
  
  # Running 2 tests from misc
  ok - misc::passing 
  not ok - misc::failing 
    ---
    assertions: 1
    failures:
    - simple.c:4: |+
        Assertion failed: The expression 0 is false.
    ...

Testing CRITERION_OUTPUTS

  $ CRITERION_OUTPUTS=tap:- simple.c.bin
  TAP version 13
  1..2
  # Criterion v.* (re)
  
  # Running 2 tests from misc
  ok - misc::passing 
  not ok - misc::failing 
    ---
    assertions: 1
    failures:
    - simple.c:4: |+
        Assertion failed: The expression 0 is false.
    ...

  $ skip.c.bin --tap
  TAP version 13
  1..2
  # Criterion v.* (re)
  
  # Running 2 tests from misc
  ok - misc::skipping  # SKIP test was skipped
  ok - misc::message  # SKIP Skips may take printf-like messages
