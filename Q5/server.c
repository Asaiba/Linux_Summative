// server.c - Real-time Chat Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 4
#define BUFFER_SIZE 2048
#define USERNAME_LEN 32

typedef struct {
    int socket;
    char username[USERNAME_LEN];
    pthread_t thread;
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t lock;

void send_online_users(int client_socket) {
    char list[BUFFER_SIZE] = "Online users:\n";
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; ++i) {
        strcat(list, clients[i].username);
        strcat(list, "\n");
    }
    pthread_mutex_unlock(&lock);
    send(client_socket, list, strlen(list), 0);
}

int find_client_by_username(const char *username) {
    for (int i = 0; i < client_count; ++i) {
        if (strcmp(clients[i].username, username) == 0) return i;
    }
    return -1;
}

void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    char username[USERNAME_LEN];

    // Receive and store username
    recv(client_socket, username, USERNAME_LEN, 0);
    username[strcspn(username, "\n")] = 0; // remove newline

    pthread_mutex_lock(&lock);
    if (client_count >= MAX_CLIENTS) {
        pthread_mutex_unlock(&lock);
        send(client_socket, "Server full", 11, 0);
        close(client_socket);
        return NULL;
    }

    clients[client_count].socket = client_socket;
    strncpy(clients[client_count].username, username, USERNAME_LEN);
    client_count++;
    pthread_mutex_unlock(&lock);

    printf("[+] %s connected.\n", username);
    send(client_socket, "Authenticated\n", 15, 0);
    send_online_users(client_socket);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes <= 0) break;

        // Format: <recipient_username>:<message>
        char *recipient = strtok(buffer, ":");
        char *msg = strtok(NULL, "");

        if (recipient && msg) {
            pthread_mutex_lock(&lock);
            int target_index = find_client_by_username(recipient);
            pthread_mutex_unlock(&lock);
            if (target_index != -1) {
                char formatted_msg[BUFFER_SIZE];
                snprintf(formatted_msg, sizeof(formatted_msg),
                         "From %s:%s", username, msg);
                send(clients[target_index].socket, formatted_msg, strlen(formatted_msg), 0);
            } else {
                send(client_socket, "User not online\n", 17, 0);
            }
        }
    }

    // Remove disconnected client
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; ++i) {
        if (clients[i].socket == client_socket) {
            clients[i] = clients[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
    close(client_socket);
    printf("[-] %s disconnected.\n", username);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = { .sin_family = AF_INET,
                                       .sin_port = htons(9000),
                                       .sin_addr.s_addr = INADDR_ANY };

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);
    printf("[*] Server running on port 9000...\n");

    pthread_mutex_init(&lock, NULL);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, (void*)&client_socket);
        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}
