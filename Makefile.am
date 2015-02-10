ACLOCAL_AMFLAGS = -I m4
SUBDIRS = dependencies/csptr samples

lib_LTLIBRARIES = libcriterion.la

WARNINGS = -Wall -Wextra \
	-Wno-unused-result -Wno-missing-field-initializers

libcriterion_la_CFLAGS = \
	$(WARNINGS) \
	-std=gnu99 \
	-fplan9-extensions \
	-I$(top_srcdir)/include/ \
	-I$(top_srcdir)/dependencies/csptr/include/ \
	$(COVERAGE_CFLAGS)

libcriterion_la_LDFLAGS = $(COVERAGE_LDFLAGS)

# dirty but unless someone has a better alternative...
libcriterion_la_LIBADD = dependencies/csptr/src/libcsptr_la-*.lo

EXTRA_DIST = LICENSE

subdirincludedir = $(includedir)/criterion/
subdirinclude_HEADERS = \
	include/criterion/assert.h \
	include/criterion/common.h \
	include/criterion/criterion.h \
	include/criterion/event.h \
	include/criterion/hooks.h \
	include/criterion/stats.h

libcriterion_la_SOURCES = \
	src/event.c \
	src/event.h \
	src/report.c \
	src/report.h \
	src/runner.c \
	src/runner.h \
	src/process.c \
	src/process.h \
	src/stats.c \
	src/stats.h
