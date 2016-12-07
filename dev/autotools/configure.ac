AC_INIT([Criterion Autotools Tests], [1.0], [your@email.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_CHECK_LIB([criterion], [criterion_initialize], [], [
  AC_MSG_ERROR([unable to find Criterion])
], [])

AC_PROG_AWK
AC_PROG_CC

AC_CONFIG_FILES([Makefile])

AC_REQUIRE_AUX_FILE([tap-driver.sh])
AC_OUTPUT
