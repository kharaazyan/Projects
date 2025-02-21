#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

typedef struct {
    int client_fd;
    int other_client_fd;
    pthread_mutex_t* mutex;
} ClientData;

void* handle_client(void* arg);

#endif
