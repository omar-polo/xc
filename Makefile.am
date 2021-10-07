AM_CFLAGS =		-Wall \
			-Wcast-qual \
			-Wextra \
			-Wmissing-declarations \
			-Wmissing-prototypes \
			-Wno-pointer-sign \
			-Wno-unused-parameter \
			-Wpointer-arith \
			-Wshadow \
			-Wsign-compare \
			-Wstrict-prototypes \
			-Wwrite-strings

bin_PROGRAMS =		xc

EXTRA_xc_SOURCES =	compat/queue.h

xc_SOURCES =		compat.h \
			list.c list.h \
			xc.c

check_PROGRAMS =	list_test

list_test_SOURCES =	list_test.c list.c list.h compat.h

TESTS =			$(check_PROGRAMS)

LDADD =			$(LIBOBJS)

.PHONY: compile_flags.txt
compile_flags.txt:
	printf "%s\n" ${CFLAGS} ${AM_CFLAGS} > $@
