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

#include "compat.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "list.h"

#define nitems(a) (sizeof(a)/sizeof(a[0]))

struct check {
	const char	*src;
	const char	*keyword;
	int		 should_fail;
	int		 expected_type;
	const char	*expected_str;
	int64_t		 expected_num;
};

#define CHECKS(_src, _key, _t, _val) {					\
		.src = _src,						\
		.keyword = _key,					\
		.expected_type = _t,					\
		.expected_str = _val,					\
	}

#define CKSYMBOL(_src, _key, _val) CHECKS(_src, _key, ATOM_SYMBOL, _val)
#define CKSTRING(_src, _key, _val) CHECKS(_src, _key, ATOM_STRING, _val)
#define CKKEYWORD(_src, _key, _val) CHECKS(_src, _key, ATOM_KEYWORD, _val)
#define CKNIL(_src, _key) CHECKS(_src, _key, ATOM_STRING, NULL)

#define CKNUM(_src, _key, _val) {					\
		.src = _src,						\
		.keyword = _key,					\
		.expected_type = ATOM_NUMBER,				\
		.expected_num = _val,					\
	}

#define SHOULD_FAIL(_src) {.src = _src, .should_fail = 1}

/* test suite */
struct check suite[] = {
	SHOULD_FAIL(""),
	SHOULD_FAIL("("),
	SHOULD_FAIL("(hello"),
	SHOULD_FAIL("(\"hello"),
	SHOULD_FAIL("(\"hello)\""),

	CKNIL("(:foo bar :baz 5)", "hello"),
	CKNIL("(:foo bar :baz)", "baz"),

	CKSYMBOL("(:hello there :how are :you)", "hello", "there"),

	CKNUM("(:foo \"hello\" :bar 7)", "bar", 7),
};

static inline const char *
typename(int t)
{
	switch (t) {
	case ATOM_STRING:
		return "string";
	case ATOM_SYMBOL:
		return "symbol";
	case ATOM_KEYWORD:
		return "keyword";
	case ATOM_NUMBER:
		return "number";
	default:
		return "<unknown>";
	}
}

int
main(void)
{
	struct listhead	 h;
	struct item	*item;
	size_t		 i;
	char		*dup;

	SIMPLEQ_INIT(&h);

	for (i = 0; i < nitems(suite); ++i) {
		if ((dup = strdup(suite[i].src)) == NULL)
			err(1, "strdup");

		if (parse_list(dup, &h) == -1 && !suite[i].should_fail)
			errx(1, "can't parse: %s", suite[i].src);

		if (suite[i].should_fail)
			goto next;

		item = plist_get(&h, suite[i].keyword);
		if (item == NULL &&
		    suite[i].expected_type != ATOM_STRING &&
		    suite[i].expected_str != NULL)
			errx(1, "nil for key :%s where not expected in %s",
			    suite[i].keyword, suite[i].src);

		if (item == NULL)
			goto next;

		if (item->type != suite[i].expected_type)
			errx(1, "wanted type %s; got %s; keyword=%s src: %s",
			    typename(suite[i].expected_type),
			    typename(item->type),
			    suite[i].keyword, suite[i].src);

		switch (item->type) {
		case ATOM_STRING:
		case ATOM_SYMBOL:
		case ATOM_KEYWORD:
			if (strcmp(item->v.str, suite[i].expected_str))
				errx(1, "for keyword %s there is %s, not %s."
				    "  src: %s",
				    suite[i].keyword, item->v.str,
				    suite[i].expected_str,
				    suite[i].src);
			break;
		case ATOM_NUMBER:
			if (item->v.num != suite[i].expected_num)
				errx(1, "for keyword %s there is %"PRIi64", not "
				    "%"PRIi64".  src: %s",
				    suite[i].keyword, item->v.num,
				    suite[i].expected_num,
				    suite[i].src);
			break;
		default:
			errx(1, "unknown type %d for src: %s",
			    item->type, suite[i].src);
		}

	next:
		free_list(&h);
		free(dup);
	}

	return 0;
}
