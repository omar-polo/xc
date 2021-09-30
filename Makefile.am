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

xc_SOURCES =		xc.c

.PHONY: compile_flags.txt
compile_flags.txt:
	printf "%s\n" ${CFLAGS} ${AM_CFLAGS} > $@
