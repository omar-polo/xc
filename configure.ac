AC_INIT([xc], [0.1])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

AC_CHECK_LIB([strophe], [xmpp_initialize], [],
	[AC_MSG_ERROR([requires libstrophe])])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
])

AC_OUTPUT
