#include "server.h"

int main() {
    int server_fd, client_fds[2];
    struct sockaddr_in addr;
    pthread_t threads[2];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);

    for (int i = 0; i < 2; i++) {
        client_fds[i] = accept(server_fd, NULL, NULL);
        if (client_fds[i] < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        printf("Client %d connected\n", i + 1);
    }

    for (int i = 0; i < 2; i++) {
        ClientData* data = malloc(sizeof(ClientData));
        data->client_fd = client_fds[i];
        data->other_client_fd = client_fds[1 - i];
        data->mutex = &mutex;
        pthread_create(&threads[i], NULL, handle_client, data);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    close(server_fd);
    return 0;
}
