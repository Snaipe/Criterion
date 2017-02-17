# use the provided wrapper script to output things properly
LOG_COMPILER = $(top_srcdir)/build-aux/criterion-tap-test

# use the TAP log driver
LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
            $(top_srcdir)/build-aux/tap-driver.sh

check_PROGRAMS = criterion_tests

criterion_tests_SOURCES = simple.c
criterion_tests_LDFLAGS = -lcriterion

TESTS = criterion_tests
EXTRA_DIST = $(TESTS)
