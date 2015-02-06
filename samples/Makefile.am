TESTS = signal simple
TESTS_ENVIRONMENT = CRITERION_ALWAYS_SUCCEED=1

check_PROGRAMS = signal simple
CFLAGS = -I$(top_srcdir)/include/
LDADD = -L$(top_srcdir)/ -lcriterion

signal_SOURCES = signal.c
simple_SOURCES = simple.c
