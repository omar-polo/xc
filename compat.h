/*
 * Copyright (c) 2021 Omar Polo <op@omarpolo.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef COMPAT_H
#define COMPAT_H

#include "config.h"

#include <paths.h>
#include <stddef.h>

#ifndef _PATH_TTY
#define _PATH_TTY "/dev/tty"
#endif

#ifndef HAVE_GETPROGNAME
const char	*getprogname(void);
#endif

#ifdef HAVE_READPASSPHRASE
#include <readpassphrase.h>
#else
char		*readpassphrase(const char *, char *, size_t, int);
#define RPP_ECHO_OFF    0x00            /* Turn off echo (default). */
#define RPP_ECHO_ON     0x01            /* Leave echo on. */
#define RPP_REQUIRE_TTY 0x02            /* Fail if there is no tty. */
#define RPP_FORCELOWER  0x04            /* Force input to lower case. */
#define RPP_FORCEUPPER  0x08            /* Force input to upper case. */
#define RPP_SEVENBIT    0x10            /* Strip the high bit from input. */
#define RPP_STDIN       0x20            /* Read from stdin, not /dev/tty */
#endif

#ifndef HAVE_STRTONUM
long long	 strtonum(const char*, long long, long long, const char**);
#endif

#endif
