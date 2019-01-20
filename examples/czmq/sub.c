#include <signal.h>
#include <czmq.h>

uint64_t n_msg;
uint64_t n_msg0;

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
    signal(SIGALRM, print_stats);
    ualarm(999999, 999999);

    zsock_t *socket = zsock_new_sub("ipc://example.sock", "zTopic");

    char *topic;
    char *msg;

    while (1) {
        int rc = zsock_recv(socket, "ss", &topic, &msg);
        if (rc != 0) {
            exit(-1);
        }

        n_msg++;
        free(topic);
        free(msg);
    }

    zsock_destroy(&socket);
    return 0;
}
