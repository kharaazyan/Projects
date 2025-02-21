#include "../include/client.h"

int connect_to_server() {
    struct sockaddr_in addr;
    struct in_addr ip_addr;
    socklen_t size = sizeof(addr);

    inet_pton(AF_INET, IP, &ip_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = ip_addr.s_addr;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&addr, size) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    printf("Connection established to server %s:%d\n", IP, PORT);
    sleep(1);
    return sockfd;
}

void show_flights(int sockfd) {
    send(sockfd, "1", 2, 0);

    char buff[BUFF] = {0};
    int done = 0;

    while (!done) {
        int bytes_received = recv(sockfd, buff, sizeof(buff), 0);
        if (bytes_received <= 0) {
            perror("recv");
            exit(1);
        }

        for (int i = 0; i < bytes_received; i++) {
            if (buff[i] == '\t') {
                buff[i] = '\0';
                done = 1;
                break;
            }
        }
        printf("%s", buff);

        if (!done) {
            printf("\nPress Enter to show more...");
            while (getchar() != '\n');
        }
        memset(buff, 0, sizeof(buff));
    }

    printf("\nEnd of flight list.\n");
}

void book_flight_client(int sockfd) {
    send(sockfd, "2", 2, 0);


    char buffer[BUFF] = {0};

    int bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        perror("recv");
        return;
    }
    printf("%s", buffer);
    memset(buffer, 0, sizeof(buffer));

    char flight_id_str[32];
    scanf("%s", flight_id_str);
    send(sockfd, flight_id_str, strlen(flight_id_str), 0);

    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        perror("recv");
        return;
    }
    printf("%s", buffer);
    memset(buffer, 0, sizeof(buffer));

    char user_name[SIZE];
    fgets(user_name, SIZE, stdin);
    user_name[strcspn(user_name, "\n")] = 0;
    send(sockfd, user_name, strlen(user_name), 0);

    bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        perror("recv");
        return;
    }
    printf("%s", buffer);
    memset(buffer, 0, sizeof(buffer));

    char contact[SIZE];
    fgets(contact, SIZE, stdin);
    contact[strcspn(contact, "\n")] = 0;
    send(sockfd, contact, strlen(contact), 0);

    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        perror("recv");
        return;
    }
    printf("%s", buffer);
}
