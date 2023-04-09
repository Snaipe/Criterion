Testing multiple samples with --json

  $ simple.c.bin --json
  {
    "id": "Criterion v.*", (re)
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
    "id": "Criterion v.*", (re)
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
    "id": "Criterion v.*", (re)
    "passed": 6,
    "failed": 2,
    "errored": 0,
    "skipped": 0,
    "test_suites": [
      {
        "name": "asserts",
        "passed": 6,
        "failed": 2,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "wstring",
            "assertions": 0,
            "status": "PASSED"
          },
          {
            "name": "string",
            "assertions": 0,
            "status": "PASSED"
          },
          {
            "name": "stream",
            "assertions": 0,
            "status": "PASSED"
          },
          {
            "name": "old_school",
            "assertions": 2,
            "status": "FAILED",
            "messages": [
              "asserts.c:19: (no message)",
              "asserts.c:18: You can fail an assertion with a message from anywhere"
            ]
          },
          {
            "name": "native",
            "assertions": 0,
            "status": "PASSED"
          },
          {
            "name": "float",
            "assertions": 0,
            "status": "PASSED"
          },
          {
            "name": "base",
            "assertions": 2,
            "status": "FAILED",
            "messages": [
              "asserts.c:13: This assert runs",
              "asserts.c:12: assert is fatal, expect isn't"
            ]
          },
          {
            "name": "array",
            "assertions": 0,
            "status": "PASSED"
          }
        ]
      }
    ]
  }

  $ more-suites.c.bin --json
  {
    "id": "Criterion v.*", (re)
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
    "id": "Criterion v.*", (re)
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
              "long-messages.c:4: This is\nA long message\nSpawning multiple lines.\n\nFormatting is respected."
            ]
          }
        ]
      }
    ]
  }

  $ description.c.bin --json
  {
    "id": "Criterion v.*", (re)
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
    "id": "Criterion v.*", (re)
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

  $ failmessages.c.bin --output=json:-
  {
    "id": "Criterion v.*", (re)
    "passed": 0,
    "failed": 6,
    "errored": 0,
    "skipped": 0,
    "test_suites": [
      {
        "name": "messages",
        "passed": 0,
        "failed": 5,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "zero",
            "assertions": 17,
            "status": "FAILED",
            "messages": [
              "failmessages.c:75: (no message)",
              "failmessages.c:70: (no message)",
              "failmessages.c:69: (no message)",
              "failmessages.c:68: (no message)",
              "failmessages.c:65: (no message)",
              "failmessages.c:64: (no message)",
              "failmessages.c:63: (no message)",
              "failmessages.c:62: (no message)",
              "failmessages.c:61: (no message)",
              "failmessages.c:60: (no message)",
              "failmessages.c:59: (no message)",
              "failmessages.c:58: (no message)",
              "failmessages.c:57: (no message)",
              "failmessages.c:56: (no message)",
              "failmessages.c:55: (no message)",
              "failmessages.c:54: (no message)",
              "failmessages.c:53: (no message)"
            ]
          },
          {
            "name": "report_escape",
            "assertions": 3,
            "status": "FAILED",
            "messages": [
              "failmessages.c:226: \"dquote\" \\and\\ 'squote'\t\r\n<script>\u0001.",
              "failmessages.c:225: (no message)",
              "failmessages.c:224: (no message)"
            ]
          },
          {
            "name": "eq",
            "assertions": 27,
            "status": "FAILED",
            "messages": [
              "failmessages.c:152: (no message)",
              "failmessages.c:132: (no message)",
              "failmessages.c:131: (no message)",
              "failmessages.c:126: (no message)",
              "failmessages.c:125: (no message)",
              "failmessages.c:112: (no message)",
              "failmessages.c:111: (no message)",
              "failmessages.c:103: (no message)",
              "failmessages.c:102: (no message)",
              "failmessages.c:101: (no message)",
              "failmessages.c:99: (no message)",
              "failmessages.c:98: (no message)",
              "failmessages.c:97: (no message)",
              "failmessages.c:95: (no message)",
              "failmessages.c:92: (no message)",
              "failmessages.c:91: (no message)",
              "failmessages.c:90: (no message)",
              "failmessages.c:89: (no message)",
              "failmessages.c:88: (no message)",
              "failmessages.c:87: (no message)",
              "failmessages.c:86: (no message)",
              "failmessages.c:85: (no message)",
              "failmessages.c:84: (no message)",
              "failmessages.c:83: (no message)",
              "failmessages.c:82: (no message)",
              "failmessages.c:81: (no message)",
              "failmessages.c:80: (no message)"
            ]
          },
          {
            "name": "default",
            "assertions": 2,
            "status": "FAILED",
            "messages": [
              "failmessages.c:220: foo bar",
              "failmessages.c:219: (no message)"
            ]
          },
          {
            "name": "cmp",
            "assertions": 19,
            "status": "FAILED",
            "messages": [
              "failmessages.c:206: (no message)",
              "failmessages.c:186: (no message)",
              "failmessages.c:185: (no message)",
              "failmessages.c:183: (no message)",
              "failmessages.c:182: (no message)",
              "failmessages.c:180: (no message)",
              "failmessages.c:177: (no message)",
              "failmessages.c:176: (no message)",
              "failmessages.c:175: (no message)",
              "failmessages.c:174: (no message)",
              "failmessages.c:173: (no message)",
              "failmessages.c:172: (no message)",
              "failmessages.c:171: (no message)",
              "failmessages.c:170: (no message)",
              "failmessages.c:169: (no message)",
              "failmessages.c:168: (no message)",
              "failmessages.c:167: (no message)",
              "failmessages.c:166: (no message)",
              "failmessages.c:165: (no message)"
            ]
          }
        ]
      },
      {
        "name": "message",
        "passed": 0,
        "failed": 1,
        "errored": 0,
        "skipped": 0,
        "tests": [
          {
            "name": "compo",
            "assertions": 3,
            "status": "FAILED",
            "messages": [
              "failmessages.c:215: (no message)",
              "failmessages.c:214: (no message)",
              "failmessages.c:213: (no message)"
            ]
          }
        ]
      }
    ]
  }

Testing CRITERION_OUTPUTS

  $ CRITERION_OUTPUTS=json:- simple.c.bin
  {
    "id": "Criterion v.*", (re)
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
    "id": "Criterion v.*", (re)
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
