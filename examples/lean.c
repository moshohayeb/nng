#include <stdio.h>

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>

int
main(int argc, char **argv)
{
    nng_socket server = NNG_SOCKET_INITIALIZER;

    nng_pair0_open(&server);
    nng_listen(server, "tcp://localhost:3333", NULL, 0);

    printf("Running as a server\n");
    size_t sz;
    char   buf[64] = { 0 };
    while (1) {
        int rc = nng_recv(server, buf, &sz, 0);
        if (rc != 0) {
            return -1;
        }

        buf[sz] = 0;
        printf("Got %zu bytes [%s]\n", sz, buf);
    }

    return 0;
}
