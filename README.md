# xc -- XMPP client

xc is an XMPP client.  not usable yet, of course.

## Compiling

xc depends on libstrophe, and uses autotools to build, so it's the
usual spell:

	./bootstrap
	./configure
	make
	make install # eventually

(note that `./bootstrap` is not needed when compiling from a
distributed tarball)

To compile with debug symbols turned on (the bare minimum during
development), use something like:

	./configure CFLAGS='-O0 -g -I/usr/local/include' LDFLAGS=-L/usr/local/lib

To use LSP, issue a

	make compile_flags.txt

to generate a compile_flags.txt for `clangd`, or

	bear make

for a `compile_commands.json`.
