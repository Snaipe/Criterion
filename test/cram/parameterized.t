Using parameterized tests

  $ parameterized.c.bin
  [----] parameterized.c:79: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::cleanup
  [----] parameterized.c:79: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::cleanup
  [----] parameterized.c:79: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::cleanup
  [----] parameterized.c:35: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::multiple
  [----] parameterized.c:35: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::multiple
  [----] parameterized.c:35: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::multiple
  [----] parameterized.c:14: Assertion Failed
  [----]   
  [----]   Parameter: 1
  [----]   
  [FAIL] params::simple
  [----] parameterized.c:14: Assertion Failed
  [----]   
  [----]   Parameter: 2
  [----]   
  [FAIL] params::simple
  [----] parameterized.c:14: Assertion Failed
  [----]   
  [----]   Parameter: 3
  [----]   
  [FAIL] params::simple
  [----] parameterized.c:113: Assertion Failed
  [----]   
  [----]   string: Hello
  [----]   
  [FAIL] params::string
  [----] parameterized.c:113: Assertion Failed
  [----]   
  [----]   string: World
  [----]   
  [FAIL] params::string
  [====] Synthesis: Tested: 11 | Passing: 0 | Failing: 11 | Crashing: 0 

  $ parameterized.cc.bin
  [----] parameterized.cc:72: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::cleanup
  [----] parameterized.cc:72: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::cleanup
  [----] parameterized.cc:72: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::cleanup
  [----] parameterized.cc:34: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::multiple
  [----] parameterized.cc:34: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::multiple
  [----] parameterized.cc:34: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::multiple
  [----] parameterized.cc:13: Assertion Failed
  [----]   
  [----]   Parameter: 1
  [----]   
  [FAIL] params::simple
  [----] parameterized.cc:13: Assertion Failed
  [----]   
  [----]   Parameter: 2
  [----]   
  [FAIL] params::simple
  [----] parameterized.cc:13: Assertion Failed
  [----]   
  [----]   Parameter: 3
  [----]   
  [FAIL] params::simple
  [----] parameterized.cc:92: Assertion Failed
  [----]   
  [----]   string: Hello
  [----]   
  [FAIL] params::string
  [----] parameterized.cc:92: Assertion Failed
  [----]   
  [----]   string: World
  [----]   
  [FAIL] params::string
  [====] Synthesis: Tested: 11 | Passing: 0 | Failing: 11 | Crashing: 0 

Using parameterized tests (verbose)

  $ parameterized.c.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 4 tests from params:
  [RUN ] params::cleanup
  [----] parameterized.c:79: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::cleanup
  [RUN ] params::cleanup
  [----] parameterized.c:79: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::cleanup
  [RUN ] params::cleanup
  [----] parameterized.c:79: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::cleanup
  [RUN ] params::multiple
  [----] parameterized.c:35: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::multiple
  [RUN ] params::multiple
  [----] parameterized.c:35: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::multiple
  [RUN ] params::multiple
  [----] parameterized.c:35: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::multiple
  [RUN ] params::simple
  [----] parameterized.c:14: Assertion Failed
  [----]   
  [----]   Parameter: 1
  [----]   
  [FAIL] params::simple
  [RUN ] params::simple
  [----] parameterized.c:14: Assertion Failed
  [----]   
  [----]   Parameter: 2
  [----]   
  [FAIL] params::simple
  [RUN ] params::simple
  [----] parameterized.c:14: Assertion Failed
  [----]   
  [----]   Parameter: 3
  [----]   
  [FAIL] params::simple
  [RUN ] params::string
  [----] parameterized.c:113: Assertion Failed
  [----]   
  [----]   string: Hello
  [----]   
  [FAIL] params::string
  [RUN ] params::string
  [----] parameterized.c:113: Assertion Failed
  [----]   
  [----]   string: World
  [----]   
  [FAIL] params::string
  [====] Synthesis: Tested: 11 | Passing: 0 | Failing: 11 | Crashing: 0 

  $ parameterized.cc.bin --verbose
  [----] Criterion v2.3.3
  [====] Running 4 tests from params:
  [RUN ] params::cleanup
  [----] parameterized.cc:72: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::cleanup
  [RUN ] params::cleanup
  [----] parameterized.cc:72: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::cleanup
  [RUN ] params::cleanup
  [----] parameterized.cc:72: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::cleanup
  [RUN ] params::multiple
  [----] parameterized.cc:34: Assertion Failed
  [----]   
  [----]   Parameters: (1, 2.000000)
  [----]   
  [FAIL] params::multiple
  [RUN ] params::multiple
  [----] parameterized.cc:34: Assertion Failed
  [----]   
  [----]   Parameters: (3, 4.000000)
  [----]   
  [FAIL] params::multiple
  [RUN ] params::multiple
  [----] parameterized.cc:34: Assertion Failed
  [----]   
  [----]   Parameters: (5, 6.000000)
  [----]   
  [FAIL] params::multiple
  [RUN ] params::simple
  [----] parameterized.cc:13: Assertion Failed
  [----]   
  [----]   Parameter: 1
  [----]   
  [FAIL] params::simple
  [RUN ] params::simple
  [----] parameterized.cc:13: Assertion Failed
  [----]   
  [----]   Parameter: 2
  [----]   
  [FAIL] params::simple
  [RUN ] params::simple
  [----] parameterized.cc:13: Assertion Failed
  [----]   
  [----]   Parameter: 3
  [----]   
  [FAIL] params::simple
  [RUN ] params::string
  [----] parameterized.cc:92: Assertion Failed
  [----]   
  [----]   string: Hello
  [----]   
  [FAIL] params::string
  [RUN ] params::string
  [----] parameterized.cc:92: Assertion Failed
  [----]   
  [----]   string: World
  [----]   
  [FAIL] params::string
  [====] Synthesis: Tested: 11 | Passing: 0 | Failing: 11 | Crashing: 0 

Parameterized tests should be listed as one test

  $ parameterized.c.bin --list
  params: 4 tests
  |-- cleanup
  |-- multiple
  |-- simple
  `-- string
