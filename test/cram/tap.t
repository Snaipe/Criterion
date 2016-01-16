Testing multiple samples with --tap

  $ simple.c.bin --tap
  TAP version 13
  1..2
  # Criterion v2.2.0
  
  # Running 2 tests from misc
  ok - misc::passing 
  not ok - misc::failing 
    simple.c:4: Assertion failed: The expression 0 is false.

  $ signal.c.bin --tap
  TAP version 13
  1..3
  # Criterion v2.2.0
  
  # Running 3 tests from simple
  not ok - simple::wrong_signal 
  not ok - simple::uncaught unexpected signal after signal.c:12
  ok - simple::caught 

  $ asserts.c.bin --tap
  TAP version 13
  1..6
  # Criterion v2.2.0
  
  # Running 6 tests from asserts
  ok - asserts::string 
  not ok - asserts::old_school 
    asserts.c:18: Assertion failed: The conditions for this assertion were not met.
    asserts.c:17: Assertion failed: You can fail an assertion with a message from anywhere
  ok - asserts::native 
  ok - asserts::float 
  not ok - asserts::base 
    asserts.c:12: Assertion failed: This assert runs
    asserts.c:11: Assertion failed: assert is fatal, expect isn't
  ok - asserts::array 

  $ more-suites.c.bin --tap
  TAP version 13
  1..3
  # Criterion v2.2.0
  
  # Running 1 tests from suite2
  ok - suite2::test 
  
  # Running 1 tests from suite1
  ok - suite1::test 
  
  # Running 1 tests from disabled
  ok - disabled::test  # SKIP suite is disabled

  $ long-messages.c.bin --tap
  TAP version 13
  1..1
  # Criterion v2.2.0
  
  # Running 1 tests from sample
  not ok - sample::long_msg 
    long-messages.c:4: Assertion failed: This is
      A long message
      Spawning multiple lines.
      Formatting is respected.

  $ description.c.bin --tap
  TAP version 13
  1..2
  # Criterion v2.2.0
  
  # Running 2 tests from misc
  ok - misc::skipped This one is skipped # SKIP test is disabled
  not ok - misc::failing Just a failing test
    description.c:4: Assertion failed: The expression 0 is false.

Testing --output=tap

  $ simple.c.bin --output=tap:-
  TAP version 13
  1..2
  # Criterion v2.2.0
  
  # Running 2 tests from misc
  ok - misc::passing 
  not ok - misc::failing 
    simple.c:4: Assertion failed: The expression 0 is false.

Testing CRITERION_OUTPUTS

  $ CRITERION_OUTPUTS=tap:- simple.c.bin
  TAP version 13
  1..2
  # Criterion v2.2.0
  
  # Running 2 tests from misc
  ok - misc::passing 
  not ok - misc::failing 
    simple.c:4: Assertion failed: The expression 0 is false.
