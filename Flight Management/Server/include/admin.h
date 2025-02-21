#ifndef ADMIN_H
#define ADMIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "flight_management.h"
#include "../../common/include/common.h"

void* admin_menu(void* arg);

#endif /* ADMIN_H */
