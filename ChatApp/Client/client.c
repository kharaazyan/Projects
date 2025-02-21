#include "client.h"

void* receive_messages(void* arg) {
    ClientData* data = (ClientData*)arg;
    char buffer[SIZE];

    while (1) {
        int recv_size = recv(data->client_fd, buffer, SIZE, 0);
        if (recv_size <= 0) {
            if (recv_size == 0) {
                printf("\nServer disconnected!\n");
            } else {
                perror("recv");
            }
            close(data->client_fd);
            exit(EXIT_FAILURE);
        }

        buffer[recv_size] = '\0';
        pthread_mutex_lock(data->mutex);
        printf("\r\033[K> %s> ", buffer);  
        fflush(stdout);
        pthread_mutex_unlock(data->mutex);
    }
}

void* send_messages(void* arg) {
    ClientData* data = (ClientData*)arg;
    char buffer[SIZE];

    while (1) {
        pthread_mutex_lock(data->mutex);
        printf("> ");
        fflush(stdout);
        pthread_mutex_unlock(data->mutex);

        if (fgets(buffer, SIZE, stdin) != NULL) {
            if (send(data->client_fd, buffer, strlen(buffer), 0) <= 0) {
                perror("send");
                close(data->client_fd);
                exit(EXIT_FAILURE);
            }
        }
    }
}
