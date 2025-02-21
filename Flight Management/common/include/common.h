#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>

#define PORT 8080
#define BUFF 512
#define SIZE 50
#define IP "127.0.0.1"

typedef struct Flight {
    int flight_id;
    char source[SIZE];
    char destination[SIZE];
    int available_seats;
} Flight;

typedef struct Client {
    int client_id;
    char name[SIZE];
    char contact[SIZE];
} Client;

typedef struct ClientNode {
    Client client;
    struct ClientNode* next;
} ClientNode;

typedef struct FlightNode { 
    Flight flight;
    struct FlightNode* next;
    ClientNode* client_list;
} FlightNode;

typedef struct Info {
    pthread_t thread;
    int fd;
} Info;

#endif /* COMMON_H */
