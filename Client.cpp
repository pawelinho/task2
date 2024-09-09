#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Exercise 6: Data Class for receiving objects
class Data {
public:
    int health;
    char name[50];
};

int main() {
    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char server_response[1024] = {0};

    // Create socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        return -1;
    }

    // Exercise 6: Receiving Data object
    // while (true) {
    //     Data data;
    //     read(client_sock, &data, sizeof(data));
    //     std::cout << "Health: " << data.health << ", Name: " << data.name << std::endl;
    // }

    // Exercise 1-4: Unlimited message sending and receiving confirmation
    while (true) {
        std::cout << "Enter a message: ";
        std::cin.getline(buffer, 1024);

        send(client_sock, buffer, strlen(buffer), 0);

        // Exercise 4: Shutdown condition
        if (strcmp(buffer, "SHUTDOWN") == 0) break;

        // Receive server confirmation (Exercise 2-3)
        read(client_sock, server_response, 1024);
        std::cout << "Server: " << server_response << std::endl;
    }

    close(client_sock);
    return 0;
}
