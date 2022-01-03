# use the TAP log driver
TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
            $(top_srcdir)/build-aux/tap-driver.sh

 # Setup Criterion TAP output using ENV
 AM_TESTS_ENVIRONMENT = \
    CRITERION_OUTPUTS='tap:-'; \
    export CRITERION_OUTPUTS;

check_PROGRAMS = criterion_tests

criterion_tests_SOURCES = simple.c
criterion_tests_CFLAGS =  $(CRITERION_CFLAGS)
criterion_tests_LDFLAGS =  $(CRITERION_LIBS)

TESTS = criterion_tests
