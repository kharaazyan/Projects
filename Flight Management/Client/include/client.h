#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../../common/include/common.h"

int connect_to_server();

void show_flights(int sockfd);

void book_flight_client(int sockfd);

#endif /* CLIENT_H */
