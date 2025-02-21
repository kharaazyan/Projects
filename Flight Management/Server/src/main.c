#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "../include/server.h"
#include "../../common/include/common.h"

extern void clean_exit(int signum);
extern int server_fd;

int main() {
    signal(SIGINT, clean_exit);
    signal(SIGTERM, clean_exit);

    if (start_server() != 0) {
        fprintf(stderr, "Failed to start the server.\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        Info* info = (Info*)malloc(sizeof(Info));
        info->fd = client_fd;

        if (pthread_create(&info->thread, NULL, client_handler, (void*)info) != 0) {
            perror("pthread_create client");
            free(info);
            close(client_fd);
        }
    }

    return 0;
}
