#include "server.h"

void* handle_client(void* arg) {
    ClientData* data = (ClientData*)arg;
    char buffer[SIZE];
    
    while (1) {
        int recv_size = recv(data->client_fd, buffer, SIZE, 0);
        if (recv_size <= 0) {
            if (recv_size == 0) {
                printf("Client disconnected!\n");
            } else {
                perror("recv");
            }
            close(data->client_fd);
            free(data);
            pthread_exit(NULL);
        } else {
            buffer[recv_size] = '\0';

            pthread_mutex_lock(data->mutex);
            send(data->other_client_fd, buffer, recv_size, 0);
            pthread_mutex_unlock(data->mutex);
        }
    }
}
