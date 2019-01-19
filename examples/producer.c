#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>

int rc;

void
panic(void)
{
    printf("Error (%d): %s\n", rc, nng_strerror(rc));
    exit(-1);
}

int
main(int argc, char **argv)
{
    nng_socket p;

    if ((rc = nng_pub0_open(&p)) != 0) {
        panic();
    }

    // NNG_EBUSY;

    nng_listener lisener1, lisener2;
    if ((rc = nng_listener_create(&lisener1, p, "ipc://xxx.ipc")) != 0) {
        panic();
    }

    if ((rc = nng_listener_create(&lisener2, p, "tcp://localhost:3999")) != 0) {
        panic();
    }

    if ((rc = nng_listener_start(lisener2, 0)) != 0) {
        panic();
    }

    if ((rc = nng_listener_start(lisener1, 0)) != 0) {
        panic();
    }


    int i = rand();
    char buf[512];
    int  len = snprintf(buf, 512, "one MSG # %d\n", i++);
    while (1) {
        nng_send(p, buf, len, 0);
        // sleep(1);
    }


    return 0;
}
