#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 2048
#define USERNAME_LEN 32

int client_socket;

void *receive_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes <= 0) break;
        printf("%s\n", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    char username[USERNAME_LEN];
    char recipient[USERNAME_LEN];
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return 1;
    }

    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    send(client_socket, username, strlen(username), 0);

    char response[BUFFER_SIZE];
    recv(client_socket, response, BUFFER_SIZE, 0);
    printf("%s\n", response);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    while (1) {
        printf("To (username): ");
        fgets(recipient, sizeof(recipient), stdin);
        recipient[strcspn(recipient, "\n")] = 0;

        printf("Message: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;

        // Manually build safe formatted message
        buffer[0] = '\0'; // clear buffer
        strcpy(buffer, recipient);
        strcat(buffer, ":");
        strncat(buffer, message, BUFFER_SIZE - strlen(buffer) - 1); // safe append

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return 0;
}
