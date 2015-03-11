TESTS = \
	signal \
	report \
	suites \
	fixtures \
	asserts \
	simple

TESTS += tests/tap_test.sh \
         tests/early_exit.sh \
         tests/verbose.sh \
         tests/help.sh

tests/tap_test.sh: simple
tests/early_exit.sh: simple
tests/verbose.sh: simple
tests/help.sh: simple

TESTS_ENVIRONMENT = CRITERION_ALWAYS_SUCCEED=1

check_PROGRAMS = $(TESTS)
CFLAGS = -I$(top_srcdir)/include/ -std=c99
LDADD = -L$(top_srcdir)/ -lcriterion
