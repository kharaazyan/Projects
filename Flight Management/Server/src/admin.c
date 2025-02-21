#include "../include/admin.h"

extern pthread_mutex_t mutex;
extern size_t ticket_price;

static void ticket_price_menu();

void* admin_menu(void* arg) {
    char *login = "kharaazyan\n";
    char *pass  = "hovik2003\n";
    char tmp_buff[SIZE] = {0};


    printf("Enter admin nickname: ");
    fgets(tmp_buff, SIZE, stdin);
    while (strcmp(login, tmp_buff) != 0) {
        printf("Wrong nickname!\nTry again: ");
        memset(tmp_buff, 0, SIZE);
        fgets(tmp_buff, SIZE, stdin);
    }
    printf("Enter admin password: ");
    memset(tmp_buff, 0, SIZE);
    fgets(tmp_buff, SIZE, stdin);
    while (strcmp(pass, tmp_buff) != 0) {
        printf("Wrong password!\nTry again: ");
        memset(tmp_buff, 0, SIZE);
        fgets(tmp_buff, SIZE, stdin);
    }

    system("clear");
    printf("Welcome, system administrator!\n");
    sleep(2);

    char choose = 'y';
    while (choose == 'y') {
        system("clear");
        printf("Admin Menu:\n");
        printf("1. Flight Management\n");
        printf("2. Ticket Price Management\n");
        printf("3. End the server work\n");
        printf("Please choose an action: ");
        
        int action;
        scanf("%d", &action);
        while (action < 1 || action > 3) {
            printf("Invalid choice!\nTry again: ");
            scanf("%d", &action);
        }
        
        switch (action) {
            case 1:
                flight_management_menu();
                break;
            case 2:
                ticket_price_menu();
                break;
            case 3:
                choose = 'n';
                break;
        }
    }
    raise(SIGTERM);
    return arg;
}

static void ticket_price_menu() {
    printf("Current ticket price: %ld AMD\n", ticket_price);
    printf("Enter the new ticket price: ");
    int new_price;
    scanf("%d", &new_price);
    while (new_price < 5000) {
        printf("You can't set less than 5,000 AMD.\nTry again: ");
        scanf("%d", &new_price);
    }
    
    pthread_mutex_lock(&mutex);
    ticket_price = new_price;
    pthread_mutex_unlock(&mutex);
    printf("Ticket price updated successfully!\n");
    sleep(2);
}
