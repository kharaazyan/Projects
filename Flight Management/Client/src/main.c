#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/client.h"
#include "../../common/include/common.h"

int main() {
    int sockfd = connect_to_server();
    if (sockfd < 0) {
        fprintf(stderr, "Could not connect to server.\n");
        return -1;
    }

    char choose = 'y';
    while (choose == 'y') {
        system("clear");
        printf("Client Menu:\n");
        printf("1. Show Available Flights\n");
        printf("2. Book a Flight\n");
        printf("3. Exit\n");
        printf("Please choose an action: ");
        
        int action;
        scanf("%d", &action);

        switch (action) {
            case 1:
                show_flights(sockfd);
                printf("\nPress Enter to continue...");
                while (getchar() != '\n');
                getchar();
                break;
            case 2:
                book_flight_client(sockfd);
                printf("\nPress Enter to continue...");
                while (getchar() != '\n');
                getchar();
                break;
            case 3:
                send(sockfd, "3", 2, 0);
                choose = 'n';
                break;
            default:
                printf("Invalid choice!\n");
                sleep(1);
                break;
        }
    }

    close(sockfd);
    printf("Client disconnected.\n");
    return 0;
}
