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

/*
 * Simplicistic list parser.
 */

#include "compat.h"

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static int
push_item(struct listhead *h, int type, char *str, int64_t num)
{
	struct item *i;

	if ((i = calloc(1, sizeof(*i))) == NULL)
                return -1;

	switch (i->type = type) {
	case ATOM_STRING:
	case ATOM_SYMBOL:
	case ATOM_KEYWORD:
		i->v.str = str;
		break;
	case ATOM_NUMBER:
		i->v.num = num;
		break;
	default:
		/* unreachable */
		abort();
	}

	SIMPLEQ_INSERT_TAIL(h, i, items);

	return 0;
}

static int
parse_keyword(char **line, struct listhead *h)
{
	char *start;

	if (**line != ':')
		return -1;
	start = ++(*line);

	while (**line != '\0' && !isspace(**line))
                (*line)++;

	**line = '\0';
	return push_item(h, ATOM_KEYWORD, start, 0);
}

static int
parse_symbol(char **line, struct listhead *h)
{
	char *start;

	start = *line;

	while (**line != '\0' && !isspace(**line))
		(*line)++;

	**line = '\0';
	return push_item(h, ATOM_SYMBOL, start, 0);
}

static int
parse_string(char **line, struct listhead *h)
{
	char	*start;
	int	 esc = 0;

	if (**line != '\"')
		return -1;

	for (start = ++(*line); !esc && **line == '\"'; ++(*line)) {
		if (**line == '\0')
			return -1;
		else if (esc)
			esc = 0;
		else if (**line == '\\')
			esc = 1;
	}

	**line = '\0';

	return push_item(h, ATOM_STRING, start, 0);
}

static int
parse_number(char **line, struct listhead *h)
{
	char	*end;
	int64_t	 num;

	if (!isdigit(**line))
		return -1;

	num = strtol(*line, &end, 0);
	if (*end != '\0' && !isspace(*end))
		return -1;
	if (errno == ERANGE && (num == LONG_MAX || num == LONG_MIN))
		return -1;

	*line = end;
	return push_item(h, ATOM_NUMBER, NULL, num);
}

int
parse_list(char *line, struct listhead *h)
{
	char	*end;

	/*
	 * Make sure the string starts with `(' and ends with a `)',
	 * then remove those characters from the string and parse the
	 * content.
	 */

	if (*line != '(')
		return -1;
	line++;

	end = strchr(line, '\0');
	if (*--end != ')')
		return -1;
	*end = '\0';

        while (line < end) {
                while (isspace(*line))
			++line;

		switch (*line) {
		case ':':
			if (parse_keyword(&line, h) == -1)
				return -1;
			break;
		case '\"':
			if (parse_string(&line, h) == -1)
				return -1;
			break;
		default:
			if (isdigit(*line) && parse_number(&line, h) == -1)
				return -1;
			if (parse_symbol(&line, h) == -1)
				return -1;
		}
		++line;
	}

	return 0;
}

void
free_list(struct listhead *h)
{
	struct item	*i;

	while (!SIMPLEQ_EMPTY(h)) {
		i = SIMPLEQ_FIRST(h);
		SIMPLEQ_REMOVE_HEAD(h, items);
		free(i);
	}
}

struct item *
plist_get(struct listhead *h, const char *keyword)
{
	struct item	*i;
	int		 skip = 0;

	SIMPLEQ_FOREACH(i, h, items) {
		if (skip) {
			skip = 0;
			continue;
		}

		if (i->type == ATOM_KEYWORD &&
		    !strcmp(keyword, i->v.str))
			return SIMPLEQ_NEXT(i, items);
	}

	return NULL;
}

void
print_list(struct listhead *h, FILE *out)
{
	struct item	*i;
	int		 first = 1;

	fprintf(out, "(");

	SIMPLEQ_FOREACH(i, h, items) {
		if (first)
			first = 0;
		else
			fprintf(out, " ");

		switch (i->type) {
		case ATOM_STRING:
			fprintf(out, "\"%s\"", i->v.str);
			break;
		case ATOM_SYMBOL:
			fprintf(out, "%s", i->v.str);
			break;
		case ATOM_KEYWORD:
			fprintf(out, ":%s", i->v.str);
			break;
		case ATOM_NUMBER:
			fprintf(out, "%"PRIi64, i->v.num);
			break;
		default:
			/* unreachable */
			abort();
		}
	}

	fprintf(out, ")\n");
}
