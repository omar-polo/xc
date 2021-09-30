AC_INIT([xc], [0.1])
AC_CONFIG_LIBOBJ_DIR(compat)
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

AC_REPLACE_FUNCS([
	explicit_bzero \
	getprogname \
	readpassphrase \
	strtonum \
])

AC_MSG_CHECKING(for program_invocation_short_name)
AC_LINK_IFELSE([AC_LANG_SOURCE(
	[
		#include <errno.h>
		#include <stdio.h>
		#include <stdlib.h>
		int main(void) {
			const char *cp = program_invocation_short_name;
			printf("%s\n", cp);
			exit(0);
		}
	])],
	[
		AC_DEFINE([HAVE_PROGRAM_INVOCATION_SHORT_NAME], 1,
		    [program_invocation_short_name])
		AC_MSG_RESULT(yes)
	],
	AC_MSG_RESULT(no)
)

AC_CHECK_LIB([strophe], [xmpp_initialize], [],
	[AC_MSG_ERROR([requires libstrophe])])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
])

AC_OUTPUT
