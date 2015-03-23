BIN_TESTS = \
	signal \
	report \
	suites \
	fixtures \
	asserts \
	more-suites \
	long-messages \
	simple

TESTS_ENVIRONMENT = CRITERION_ALWAYS_SUCCEED=1

check_PROGRAMS := $(BIN_TESTS)
CFLAGS = -I$(top_srcdir)/include/ -std=c99 -Wall -Wextra -pedantic
LDADD = -L$(top_srcdir)/ -lcriterion

SCRIPT_TESTS = tests/tap_test.sh \
               tests/early_exit.sh \
               tests/verbose.sh \
               tests/list.sh \
               tests/pattern.sh \
               tests/fail_fast.sh \
               tests/help.sh

EXTRA_DIST = $(SCRIPT_TESTS)

tests/tap_test.sh: simple signal asserts
tests/early_exit.sh: simple
tests/verbose.sh: simple
tests/help.sh: simple

TESTS = $(BIN_TESTS) $(SCRIPT_TESTS)
