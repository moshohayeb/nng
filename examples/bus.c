#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nng/nng.h>
#include <nng/transport/ipc/ipc.h>

#include <nng/protocol/pubsub0/pub.h>

// #include <nng/protocol/bus0/bus.h>
// #include <nng/protocol/bus0/bus.h>
// #include <nng/protocol/reqrep0/rep.h>
// #include <nng/protocol/reqrep0/req.h>
// #include <nng/supplemental/util/platform.h>

void fatal(const char *why)
{
    fprintf(stderr, "%s\n", why);
    exit(-1);
}

int main(int argc, char** argv)
{
    nng_socket socket;
    int rc;

    rc = nng_pub0_open(&socket);
    if (rc != 0) {
        fatal("nng_bus0_open");
    }

    rc = nng_listen(socket, "tcp://localhost:9000", NULL, 0);
    if (rc != 0) {
        fatal("nng_bus0_open");
        exit(-1);
    }

    printf("fd=%d\n", socket.id);
    int i = 0;
    while (1) {
        char buf[1024];
        snprintf(buf, 1024, "Hello %5d", i++);
        nng_send(socket, buf, strlen(buf), 0);
        printf("MSG=%s\n", buf);
        sleep(1);
    }

    return 0;
}
