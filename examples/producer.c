#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>

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

    if ((rc = nng_pub0_open(&sock)) != 0) {
        panic();
    }

    // NNG_EBUSY;

    nng_listener lisener1, lisener2;

    if ((rc = nng_listener_create(&lisener1, sock, "ipc://xxx.ipc")) != 0) {
        panic();
    }
    if ((rc = nng_listener_start(lisener1, 0)) != 0) {
        panic();
    }


    // if ((rc = nng_listener_create(&lisener2, sock, "tcp://localhost:3999")) != 0) {
    //     panic();
    // }
    // if ((rc = nng_listener_start(lisener2, 0)) != 0) {
    //     panic();
    // }


    int  i = rand();
    char buf[512] = { 'x' };
    // int  len = snprintf(buf, 512, "one MSG # %d\n", i++);
    while (1) {
        nng_send(sock, buf, 3, 0);
        n_msg++;
        // sleep(1);
    }


    return 0;
}
