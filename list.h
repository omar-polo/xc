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

#ifndef LIST_H
#define LIST_H

#include <sys/queue.h>

#include <stdint.h>
#include <stdio.h>

enum atom_type {
	ATOM_STRING,
	ATOM_SYMBOL,
	ATOM_KEYWORD,
	ATOM_NUMBER,
};

SIMPLEQ_HEAD(listhead, item);

struct item {
	int type;
	union {
		char	*str;
		int64_t	 num;
	} v;
	SIMPLEQ_ENTRY(item) items;
};

int		 parse_list(char *, struct listhead *);
void		 free_list(struct listhead *);
struct item	*plist_get(struct listhead *, const char *);
void		 print_list(struct listhead *, FILE *);

#endif
