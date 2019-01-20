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

    zsock_t *socket = zsock_new_pub("ipc://example.sock");

    assert(socket);

    while (!zsys_interrupted) {
        // zsys_info("Publishing");
        zsock_send(socket, "ss", "zTopic", "MESSAGE PART");
        // zclock_sleep(1000);
        n_msg++;
    }

    zsock_destroy(&socket);
    return 0;
}
