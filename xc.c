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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strophe.h>
#include <unistd.h>

char	*jid;
char	*password;
char	*host;
int	 verbose;
int	 port;

static void
conn_handler(xmpp_conn_t *conn, xmpp_conn_event_t status, int error,
    xmpp_stream_error_t *stream_err, void *d)
{
	xmpp_ctx_t *ctx = d;
	int secured;

	if (status == XMPP_CONN_CONNECT) {
		warnx("connected!");
		secured = xmpp_conn_is_secured(conn);
		warnx("connection is %ssecured", secured ? "" : "NOT ");
		xmpp_disconnect(conn);
	} else {
		warnx("disconnected");
		xmpp_stop(ctx);
	}
}

static void __dead
usage(void)
{
	fprintf(stderr,
	    "Usage: %s [-v] [-h host] [-P port] [-p password] -u user\n",
	    getprogname());

	exit(1);
}

int
main(int argc, char **argv)
{
	xmpp_ctx_t	*ctx;
	xmpp_conn_t	*conn;
	xmpp_log_t	*log;
	int		 ch, ret = 1;
	char		 passbuf[1024], *at;

	while ((ch = getopt(argc, argv, "h:P:p:u:v")) != -1) {
		switch (ch) {
		case 'h':
			host = optarg;
			break;
		case 'P': {
			const char *errstr;

			port = strtonum(optarg, 1, INT16_MAX, &errstr);
			if (errstr != NULL)
				errx(1, "port is %s: %s", errstr, optarg);
			break;
		}
		case 'p':
			password = optarg;
			break;
		case 'u':
			jid = optarg;
			break;
		case 'v':
			verbose++;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (jid == NULL)
		usage();

	if (host == NULL) {
		if ((at = strchr(jid, '@')) == NULL)
			errx(1, "invalid jid and/or missing host");
		host = at+1;
	}

	if (password == NULL) {
		password = readpassphrase("Password:", passbuf,
		    sizeof(passbuf), RPP_REQUIRE_TTY);
		if (password == NULL)
			errx(1, "can't read passphrase");
	}

	log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG);
	if ((ctx = xmpp_ctx_new(NULL, log)) == NULL)
		err(1, "xmpp_ctx_new");

	if ((conn = xmpp_conn_new(ctx)) == NULL)
		err(1, "xmpp_conn_new");

	xmpp_conn_set_jid(conn, jid);
	xmpp_conn_set_pass(conn, password);

	explicit_bzero(passbuf, sizeof(passbuf));

	if (unveil("/etc/ssl", "r") == -1)
		err(1, "unveil");

	if (pledge("stdio rpath inet dns", NULL) == -1)
		err(1, "pledge");

	if (xmpp_connect_client(conn, host, port, conn_handler, ctx) ==
	    XMPP_EOK) {
		xmpp_run(ctx);
		ret = 0;
	} else
		warnx("failed to connect");

	xmpp_conn_release(conn);
	xmpp_ctx_free(ctx);
	xmpp_shutdown();

	return ret;
}
