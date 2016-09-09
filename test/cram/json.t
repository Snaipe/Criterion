Testing multiple samples with --json

  $ simple.c.bin --json
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 1,
    "failed": 1,
    "errored": 0,
    "skipped": 0,
    "test_suites": [
      {
        "name": "misc",
        "passed": 1,
        "failed": 1,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "passing",
            "assertions": 1,
            "status": "PASSED"
          },
          {
            "name": "failing",
            "assertions": 1,
            "status": "FAILED",
            "messages": [
              "simple.c:4: The expression 0 is false."
            ]
          }
        ]
      }
    ]
  }

  $ signal.c.bin --json
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 1,
    "failed": 2,
    "errored": 1,
    "skipped": 0,
    "test_suites": [
      {
        "name": "simple",
        "passed": 1,
        "failed": 2,
        "errored": 1,
        "skipped": 0,
        "tests": [
          {
            "name": "wrong_signal",
            "assertions": 0,
            "status": "FAILED",
            "messages": [
  
            ]
          },
          {
            "name": "uncaught",
            "assertions": 0,
            "status": "ERRORED",
            "messages": ["The test crashed."]
          },
          {
            "name": "caught",
            "assertions": 0,
            "status": "PASSED"
          }
        ]
      }
    ]
  }

  $ asserts.c.bin --json
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 5,
    "failed": 2,
    "errored": 0,
    "skipped": 0,
    "test_suites": [
      {
        "name": "asserts",
        "passed": 5,
        "failed": 2,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "wstring",
            "assertions": 10,
            "status": "PASSED"
          },
          {
            "name": "string",
            "assertions": 10,
            "status": "PASSED"
          },
          {
            "name": "old_school",
            "assertions": 2,
            "status": "FAILED",
            "messages": [
              "asserts.c:18: The conditions for this assertion were not met.",
              "asserts.c:17: You can fail an assertion with a message from anywhere"
            ]
          },
          {
            "name": "native",
            "assertions": 8,
            "status": "PASSED"
          },
          {
            "name": "float",
            "assertions": 2,
            "status": "PASSED"
          },
          {
            "name": "base",
            "assertions": 6,
            "status": "FAILED",
            "messages": [
              "asserts.c:12: This assert runs",
              "asserts.c:11: assert is fatal, expect isn't"
            ]
          },
          {
            "name": "array",
            "assertions": 3,
            "status": "PASSED"
          }
        ]
      }
    ]
  }

  $ more-suites.c.bin --json
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 2,
    "failed": 0,
    "errored": 0,
    "skipped": 1,
    "test_suites": [
      {
        "name": "suite2",
        "passed": 1,
        "failed": 0,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "test",
            "assertions": 1,
            "status": "PASSED"
          }
        ]
      },
      {
        "name": "suite1",
        "passed": 1,
        "failed": 0,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "test",
            "assertions": 1,
            "status": "PASSED"
          }
        ]
      },
      {
        "name": "disabled",
        "passed": 0,
        "failed": 0,
        "errored": 0,
        "skipped": 1,
        "tests": [
          {
            "name": "test",
            "assertions": 0,
            "status": "SKIPPED",
            "messages": ["The test was skipped."]
          }
        ]
      }
    ]
  }

  $ long-messages.c.bin --json
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 0,
    "failed": 1,
    "errored": 0,
    "skipped": 0,
    "test_suites": [
      {
        "name": "sample",
        "passed": 0,
        "failed": 1,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "long_msg",
            "assertions": 1,
            "status": "FAILED",
            "messages": [
              "long-messages.c:4: This is",
              "  A long message",
              "  Spawning multiple lines.",
              "  Formatting is respected."
            ]
          }
        ]
      }
    ]
  }

  $ description.c.bin --json
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 0,
    "failed": 1,
    "errored": 0,
    "skipped": 1,
    "test_suites": [
      {
        "name": "misc",
        "passed": 0,
        "failed": 1,
        "errored": 0,
        "skipped": 1,
        "tests": [
          {
            "name": "skipped",
            "assertions": 0,
            "status": "SKIPPED",
            "messages": ["The test was skipped."]
          },
          {
            "name": "failing",
            "assertions": 1,
            "status": "FAILED",
            "messages": [
              "description.c:4: The expression 0 is false."
            ]
          }
        ]
      }
    ]
  }

Testing --output=json

  $ simple.c.bin --output=json:-
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 1,
    "failed": 1,
    "errored": 0,
    "skipped": 0,
    "test_suites": [
      {
        "name": "misc",
        "passed": 1,
        "failed": 1,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "passing",
            "assertions": 1,
            "status": "PASSED"
          },
          {
            "name": "failing",
            "assertions": 1,
            "status": "FAILED",
            "messages": [
              "simple.c:4: The expression 0 is false."
            ]
          }
        ]
      }
    ]
  }

Testing CRITERION_OUTPUTS

  $ CRITERION_OUTPUTS=json:- simple.c.bin
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 1,
    "failed": 1,
    "errored": 0,
    "skipped": 0,
    "test_suites": [
      {
        "name": "misc",
        "passed": 1,
        "failed": 1,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "passing",
            "assertions": 1,
            "status": "PASSED"
          },
          {
            "name": "failing",
            "assertions": 1,
            "status": "FAILED",
            "messages": [
              "simple.c:4: The expression 0 is false."
            ]
          }
        ]
      }
    ]
  }

  $ skip.c.bin --json
  {
    "id": "Criterion v2.3.0-dev",
    "passed": 0,
    "failed": 0,
    "errored": 0,
    "skipped": 2,
    "test_suites": [
      {
        "name": "misc",
        "passed": 0,
        "failed": 0,
        "errored": 0,
        "skipped": 2,
        "tests": [
          {
            "name": "skipping",
            "assertions": 0,
            "status": "SKIPPED",
            "messages": ["The test was skipped."]
          },
          {
            "name": "message",
            "assertions": 0,
            "status": "SKIPPED",
            "messages": ["Skips may take printf-like messages"]
          }
        ]
      }
    ]
  }
