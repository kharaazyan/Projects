#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "admin.h"
#include "flight_management.h"
#include "../../common/include/common.h"

int start_server();

void* client_handler(void* arg);

void clean_exit(int signum);

#endif /* SERVER_H */
