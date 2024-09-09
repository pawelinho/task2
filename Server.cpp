#include <iostream>
#include <cstring>
#include <pthread.h>  // For multithreading
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Exercise 6 Data Class
class Data {
public:
    int health;
    char name[50];
};

// Exercise 5: Thread function for handling client connection (messages)
void* handle_client(void* socket_desc) {
    int client_sock = *(int*)socket_desc;
    char buffer[1024] = {0};
    const char* confirmation = "Message Received";

    while (true) {
        read(client_sock, buffer, 1024);
        std::cout << "Message from client: " << buffer << std::endl;

        if (strcmp(buffer, "SHUTDOWN") == 0) break;

        // Exercise 2: Send confirmation
        send(client_sock, confirmation, strlen(confirmation), 0);
    }

    close(client_sock);
    return 0;
}

// Exercise 6: Thread function to send Data object
void* send_data(void* socket_desc) {
    int client_sock = *(int*)socket_desc;
    Data data = { 100, "Player1" };

    while (true) {
        data.health++;  // Increment health each second
        send(client_sock, &data, sizeof(data), 0);
        sleep(1);  // Send data every second
    }

    close(client_sock);
    return 0;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return -1;
    }

    std::cout << "Waiting for connections..." << std::endl;

    // Exercise 1-5: Accept and handle multiple clients with threads
    while (true) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);

        // Exercise 5: Create thread for each client connection (message handling)
        // pthread_t client_thread;
        // pthread_create(&client_thread, NULL, handle_client, (void*)&client_sock);
        // pthread_detach(client_thread);  // Detach to avoid memory leaks

        // Exercise 6: Create thread to send Data object
        pthread_t data_thread;
        pthread_create(&data_thread, NULL, send_data, (void*)&client_sock);
        pthread_detach(data_thread);
    }

    close(server_sock);
    return 0;
}
