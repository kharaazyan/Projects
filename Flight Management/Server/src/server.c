
#include "../include/server.h"

int server_fd = 0;                
pthread_mutex_t mutex;           
pthread_t admin_thread;         
FlightNode* first_flight = NULL;
size_t id_counter = 1;           
size_t ticket_price = 18128;   

int start_server() {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addr_size = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }

    if (bind(server_fd, (struct sockaddr*)&addr, addr_size) < 0) {
        perror("bind");
        return -1;
    }

    if (listen(server_fd, 128) < 0) {
        perror("listen");
        return -1;
    }

    printf("Server started on port %d\n", PORT);

    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&admin_thread, NULL, admin_menu, NULL) != 0) {
        perror("pthread_create admin");
        return -1;
    }

    return 0;
}

void* client_handler(void* arg) {
    Info* info = (Info*)arg;
    int fd = info->fd;
    free(info); 

    char request[5] = {0};
    char keep_going = 'y';
    while (keep_going == 'y') {
        int recv_size = recv(fd, request, sizeof(request), 0);
        if (recv_size <= 0) {
            close(fd);
            pthread_exit(NULL);
        }
        
        int option = atoi(request);
        switch (option) {
            case 1: 
                send_all_flights(fd);
                break;
            case 2: 
                book_flight(fd);
                break;
            case 3:
                keep_going = 'n';
                break;
            default:
                break;
        }
        memset(request, 0, sizeof(request));
    }

    close(fd);
    pthread_exit(NULL);
}

void clean_exit(int signum) {
    pthread_cancel(admin_thread);

    FlightNode* tmp = first_flight;
    while (tmp) {
        ClientNode* ctmp = tmp->client_list;
        while (ctmp) {
            ClientNode* cprev = ctmp;
            ctmp = ctmp->next;
            free(cprev);
        }

        FlightNode* prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
    first_flight = NULL;

    pthread_mutex_destroy(&mutex);

    close(server_fd);

    printf("Server stopped by signal: %d\n", signum);
    exit(0);
}
