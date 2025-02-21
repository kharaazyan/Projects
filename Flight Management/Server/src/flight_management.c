#include "../include/flight_management.h"

extern FlightNode* first_flight; 
extern pthread_mutex_t mutex;
extern size_t id_counter;      
extern size_t ticket_price;

static void create_flight();
static void delete_flight();
static void show_flights_console();
static void press_any_key();

void flight_management_menu() {
    char choose = 'y';
    while (choose == 'y') {
        system("clear");
        printf("Flight Management:\n");
        printf("1. Show Available Flights\n");
        printf("2. Create New Flight\n");
        printf("3. Delete Flight\n");
        printf("4. Return to Main Menu\n");
        printf("Your choice: ");
        
        int action;
        scanf("%d", &action);
        while (action < 1 || action > 4) {
            printf("Invalid choice!\nTry again: ");
            scanf("%d", &action);
        }

        switch (action) {
            case 1:
                show_flights_console();
                break;
            case 2:
                create_flight();
                break;
            case 3:
                delete_flight();
                break;
            case 4:
                choose = 'n';
                break;
        }
    }
}

static void show_flights_console() {
    system("clear");
    pthread_mutex_lock(&mutex);
    FlightNode* tmp = first_flight;
    if (tmp == NULL) {
        printf("There are no flights.\n");
        pthread_mutex_unlock(&mutex);
        press_any_key();
        return;
    }
    while (tmp != NULL) {
        printf("Flight ID       : %d\n", tmp->flight.flight_id);
        printf("Source          : %s\n", tmp->flight.source);
        printf("Destination     : %s\n", tmp->flight.destination);
        printf("Available Seats : %d\n", tmp->flight.available_seats);
        printf("------------------------------------------\n");
        tmp = tmp->next;
    }
    pthread_mutex_unlock(&mutex);
    press_any_key();
}

static void create_flight() {
    FlightNode* node = (FlightNode*)malloc(sizeof(FlightNode));
    node->flight.flight_id = id_counter++;
    node->next = NULL;
    node->client_list = NULL;

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Source (e.g., New York): ");
    fgets(node->flight.source, SIZE, stdin);
    node->flight.source[strcspn(node->flight.source, "\n")] = 0; 

    printf("Enter Destination (e.g., Los Angeles): ");
    fgets(node->flight.destination, SIZE, stdin);
    node->flight.destination[strcspn(node->flight.destination, "\n")] = 0; 

    printf("Enter Available Seats (e.g., 150): ");
    scanf("%d", &node->flight.available_seats);

    pthread_mutex_lock(&mutex);
    if (first_flight == NULL) {
        first_flight = node;
    } else {
        FlightNode* tmp = first_flight;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = node;
    }
    pthread_mutex_unlock(&mutex);

    printf("Flight created!\n");
    sleep(1);
}

static void delete_flight() {
    pthread_mutex_lock(&mutex);
    if (first_flight == NULL) {
        printf("There are no flights.\n");
        pthread_mutex_unlock(&mutex);
        sleep(2);
        return;
    }

    printf("Enter the flight ID to delete: ");
    int delete_id;
    scanf("%d", &delete_id);

    FlightNode* tmp = first_flight;
    FlightNode* prev = NULL;
    while (tmp && tmp->flight.flight_id != delete_id) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (!tmp) {
        printf("Flight ID %d not found.\n", delete_id);
    } else {
        if (!prev) {
            first_flight = first_flight->next;
        } else {
            prev->next = tmp->next;
        }

        ClientNode* ctmp = tmp->client_list;
        while (ctmp) {
            ClientNode* cprev = ctmp;
            ctmp = ctmp->next;
            free(cprev);
        }

        free(tmp);
        printf("Flight ID %d deleted.\n", delete_id);
    }
    pthread_mutex_unlock(&mutex);
    sleep(2);
}

void send_all_flights(int fd) {
    pthread_mutex_lock(&mutex);
    if (first_flight == NULL) {
        char msg[] = "There are no flights!\t";
        send(fd, msg, strlen(msg), 0);
        pthread_mutex_unlock(&mutex);
        return;
    }
    
    FlightNode* tmp = first_flight;
    while (tmp != NULL) {
        char buff[BUFF] = {0};
        if (tmp->next == NULL) {
            snprintf(buff, sizeof(buff),
                     "Flight ID        : %d\n"
                     "Source           : %s\n"
                     "Destination      : %s\n"
                     "Available Seats  : %d\n\t",
                     tmp->flight.flight_id,
                     tmp->flight.source,
                     tmp->flight.destination,
                     tmp->flight.available_seats);
        } else {
            snprintf(buff, sizeof(buff),
                     "Flight ID        : %d\n"
                     "Source           : %s\n"
                     "Destination      : %s\n"
                     "Available Seats  : %d\n\n",
                     tmp->flight.flight_id,
                     tmp->flight.source,
                     tmp->flight.destination,
                     tmp->flight.available_seats);
        }
        send(fd, buff, strlen(buff), 0);
        tmp = tmp->next;
    }
    pthread_mutex_unlock(&mutex);
}

void book_flight(int fd) {
    char req[] = "Enter Flight ID to book: ";
    send(fd, req, strlen(req), 0);

    char buffer[BUFF] = {0};
    int recv_size = recv(fd, buffer, sizeof(buffer), 0);
    if (recv_size <= 0) {
        return;
    }
    int flight_id = atoi(buffer);

    memset(buffer, 0, sizeof(buffer));
    char req2[] = "Enter your name: ";
    send(fd, req2, strlen(req2), 0);
    recv_size = recv(fd, buffer, sizeof(buffer), 0);
    if (recv_size <= 0) {
        return;
    }
    char user_name[SIZE];
    strncpy(user_name, buffer, SIZE - 1);
    user_name[strcspn(user_name, "\n")] = 0;

    memset(buffer, 0, sizeof(buffer));
    char req3[] = "Enter your contact: ";
    send(fd, req3, strlen(req3), 0);
    recv_size = recv(fd, buffer, sizeof(buffer), 0);
    if (recv_size <= 0) {
        return;
    }
    char user_contact[SIZE];
    strncpy(user_contact, buffer, SIZE - 1);
    user_contact[strcspn(user_contact, "\n")] = 0; 

    pthread_mutex_lock(&mutex);
    FlightNode* tmp = first_flight;
    while (tmp && tmp->flight.flight_id != flight_id) {
        tmp = tmp->next;
    }

    if (!tmp) {
        char not_found[] = "Flight not found!\n";
        send(fd, not_found, strlen(not_found), 0);
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (tmp->flight.available_seats <= 0) {
        char no_seats[] = "No available seats on this flight.\n";
        send(fd, no_seats, strlen(no_seats), 0);
        pthread_mutex_unlock(&mutex);
        return;
    }

    tmp->flight.available_seats--;

    ClientNode* new_client_node = (ClientNode*)malloc(sizeof(ClientNode));
    new_client_node->next = NULL;
    strncpy(new_client_node->client.name, user_name, SIZE - 1);
    strncpy(new_client_node->client.contact, user_contact, SIZE - 1);
    new_client_node->client.client_id = 0;

    if (tmp->client_list == NULL) {
        tmp->client_list = new_client_node;
    } else {
        ClientNode* ctmp = tmp->client_list;
        while (ctmp->next) {
            ctmp = ctmp->next;
        }
        ctmp->next = new_client_node;
    }

    char success[100];
    snprintf(success, sizeof(success),
             "Booking success! Your ticket price is %ld AMD.\n", ticket_price);
    send(fd, success, strlen(success), 0);

    pthread_mutex_unlock(&mutex);
}

static void press_any_key() {
    printf("Press Enter to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar(); 
}
