#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/sub.h>

int rc;

uint64_t n_msg;
uint64_t n_msg0;

void
panic(void)
{
    printf("Error (%d): %s\n", rc, nng_strerror(rc));
    exit(-1);
}

void
print_stats(int x)
{
    uint64_t delta = n_msg - n_msg0;

    printf("msgs/sec %llu\n", delta);

    n_msg0 = n_msg;
}


int
main(int argc, char **argv)
{
    nng_socket sock;

    signal(SIGALRM, print_stats);
    ualarm(1000000, 1000000);

    if ((rc = nng_sub0_open(&sock)) != 0) {
        panic();
    }

    nng_dialer dialer;
    if ((rc = nng_dialer_create(&dialer, sock, "tcp://localhost:3999")) != 0) {
    // if ((rc = nng_dialer_create(&dialer, sock, "ipc://xxx.ipc")) != 0) {
        panic();
    }

    if ((rc = nng_setopt(sock, NNG_OPT_SUB_SUBSCRIBE, "one", 0)) != 0) {
        panic();
    }

    nng_dialer_start(dialer, 0);

    char   buf[1024];
    while (1) {
        size_t sz = 1024;
        if (nng_recv(sock, buf, &sz, 0) != 0) {
            continue;
        }

        n_msg++;
    }


    return 0;
}
