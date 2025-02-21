#include "client.h"

int main() {
    int client_fd;
    struct sockaddr_in addr;
    pthread_t thread_recv, thread_send;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server\n");

    ClientData data = {client_fd, &mutex};

    pthread_create(&thread_recv, NULL, receive_messages, &data);
    pthread_create(&thread_send, NULL, send_messages, &data);

    pthread_join(thread_recv, NULL);
    pthread_join(thread_send, NULL);

    close(client_fd);
    return 0;
}
