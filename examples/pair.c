#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <inttypes.h>

#include <time.h>

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>

#include "utils.h"
// #define URL "ipc://xyz"
// #define URL "inproc://xyz"

#define URL     "tcp://localhost:3333"
#define MSG_LEN 64

uint64_t n_msg;
uint64_t n_msg0;
uint64_t epoch_us0;

void
print_stats(int x)
{
    uint64_t epoch_us = z_us();
    uint64_t epoch_delta = epoch_us - epoch_us0;
    uint64_t msg_delta = (n_msg - n_msg0);
    double   msgs_per_micro_sec = msg_delta * 1.0 / epoch_delta;
    double   msgs_per_sec = msgs_per_micro_sec * 1000000;

    // printf("msgs/usec %f %llu %llu\n", msgs_per_micro_sec, epoch_delta, msg_delta);
    printf("msgs/sec %llu\n", (uint64_t) msgs_per_sec);

    n_msg0 = n_msg;
    epoch_us0 = epoch_us;
}


int
node1()
{
    nng_socket client  = NNG_SOCKET_INITIALIZER;
    char data[MSG_LEN] = { 'x' };

    data[MSG_LEN - 1] = '\0';

    nng_pair0_open(&client);
    nng_dial(client, URL, NULL, 0);

    printf("Running as a client\n");
    while (1) {
        int rc = nng_send(client, data, 64, 0);
        if (rc != 0) {
            printf("bad\n");
            return -1;
        }

        n_msg++;
    }

    return 0;
}

int
node0()
{
    nng_socket server = NNG_SOCKET_INITIALIZER;
    char   buf[64] = { 0 };
    size_t sz;

    nng_pair0_open(&server);
    nng_listen(server, URL, NULL, 0);

    printf("Running as a server\n");
    while (1) {
        int rc = nng_recv(server, buf, &sz, 0);
        if (rc != 0) {
            printf("bad\n");
            return -1;
        }

        n_msg++;
    }

    return 0;
}

int
main(int argc, char **argv)
{
    signal(SIGALRM, print_stats);
    // ualarm(250000, 250000);

    if (argc == 1) {
        return node0();
    } else {
        return node1();
    }

    int rc = argc == 1 ? node0() : node1();

    return rc;
}
