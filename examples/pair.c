#include <stdio.h>
#include <unistd.h>

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>

#define INPROC_URL "inproc://xyz"

nng_socket
client()
{
    nng_socket client = NNG_SOCKET_INITIALIZER;

    nng_pair0_open(&client);
    nng_dial(client, INPROC_URL, NULL, 0);

    return client;
}

nng_socket
server()
{
    nng_socket server = NNG_SOCKET_INITIALIZER;

    nng_pair0_open(&server);
    nng_listen(server, INPROC_URL, NULL, 0);

    return server;
}

int
main(int argc, char **argv)
{
    nng_socket s = server();
    nng_socket c = client();

    int rc = nng_send(c, "HELLO", 5, 0);

    usleep(100);

    char   buf[64] = { 0 };
    size_t sz;
    rc = nng_recv(s, buf, &sz, 0);
    if (rc != 0) {
        printf("herr..\n");
    }


    printf("GOT %lu [%s] \n", sz, buf);

    return 0;
}
