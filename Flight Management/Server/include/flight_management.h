#ifndef FLIGHT_MANAGEMENT_H
#define FLIGHT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../../common/include/common.h"

void flight_management_menu();

void send_all_flights(int fd);

void book_flight(int fd);

#endif /* FLIGHT_MANAGEMENT_H */
