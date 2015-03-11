TESTS = \
	signal \
	report \
	suites \
	fixtures \
	asserts \
	simple

TESTS_ENVIRONMENT = CRITERION_ALWAYS_SUCCEED=1

check_PROGRAMS = $(TESTS)
CFLAGS = -I$(top_srcdir)/include/ -std=c99
LDADD = -L$(top_srcdir)/ -lcriterion
