// pub sub performance test -- adapted from pubsub demo in nanomsg,
// and updated for NNG.
//
// Copyright 2018 Garrett D'Amore <garrett@damore.org>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <nng/supplemental/util/platform.h>

/*  The server runs forever. */

static void
die(const char *label, int err)
{
	fprintf(stderr, "%s: %s\n", label, nng_strerror(err));
	exit(1);
}

void
server(const char *url)
{
	nng_socket fd;
	int        rv;

	// Create the socket.
	if ((rv = nng_pub0_open(&fd)) < 0) {
		die("nng_pub0_open", rv);
	}

	if ((rv = nng_listen(fd, url, NULL, 0)) < 0) {
		die("nng_listen", rv);
	}

	// Now we can just publish results.  Note that there is no explicit
	// accept required.  We just start writing the information.

	uint32_t x = 0;
	for (;;) {
		nng_msg *msg;

		if ((rv = nng_msg_alloc(&msg, 0)) != 0) {
			die("nng_msg_alloc", rv);
		}
		x++;
		if ((rv = nng_msg_append(msg, &x, sizeof(x))) != 0) {
			die("nng_msg_append", rv);
		}
		if ((rv = nng_sendmsg(fd, msg, 0)) != 0) {
			nng_msg_free(msg);
		}
	}
}

void
client(const char *url)
{
	nng_socket fd;
	nng_time   start;
	nng_time   end;
	int        rv;
	uint32_t   count;
	uint32_t   missed;
	uint32_t   last;

	if ((rv = nng_sub0_open(&fd)) != 0) {
		die("nng_sub0_open", rv);
	};

	if ((rv = nng_dial(fd, url, NULL, 0)) != 0) {
		die("nng_dial", rv);
	}

	if ((rv = nng_setopt(fd, NNG_OPT_SUB_SUBSCRIBE, "", 0)) != 0) {
		die("nng_setopt", rv);
	}

	count  = 0;
	missed = 0;
	last   = 0;
	for (;;) {
		nng_msg *msg;
		uint32_t id;

		if ((rv = nng_recvmsg(fd, &msg, 0)) != 0) {
			die("nng_recvmsg", rv);
		}
		if (nng_msg_len(msg) != sizeof(id)) {
			die("nng_msg_len", NNG_EMSGSIZE);
		}
		memcpy(&id, nng_msg_body(msg), sizeof(id));
		nng_msg_free(msg);
		count++;
		if (last == 0) {
			start = nng_clock();
		} else if ((last + 1) != id) {
			missed += id - (last + 1);
		}
		last = id;
		if (count > 400000) {
			break;
		}
	}

	end = nng_clock();
	printf("RECEIVED %u messages (missed %u)\n", count, missed);
	if (count > 1) {
		uint64_t delta = end - start;
		double   rate  = (count - 1); // first message doesn't count
		rate /= delta;
		rate *= 1000;
		printf("RATE: %f msgs/sec\n", rate);
	}
}

int
main(int argc, char **argv)
{
	int rc;
	if ((argc == 3) && (strcmp(argv[2], "-s") == 0)) {
		server(argv[1]);
		exit(EXIT_SUCCESS);
	}
	if (argc == 2) {
		client(argv[1]);
		exit(EXIT_SUCCESS);
	}
	fprintf(stderr, "Usage: %s <url> [-s]\n", argv[0]);
	exit(EXIT_FAILURE);
}
