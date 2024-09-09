// client.cpp: Implements the Client class methods.

#include "client.h"
#include <thread>
#include <chrono>

Client::Client(const string& server_ip, int server_port) : Comms() {
    try {
        cout << "[INFO] Initializing client..." << endl;
        create_socket();  // Creating the client socket

        // Set up the server address structure
        address.sin_family = AF_INET;
        address.sin_port = htons(server_port);
        if (inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr) <= 0) { // inet_pton() converts the server IP string into in_addr used by sockets
            throw runtime_error("[ERROR] Invalid server IP address");
        }

        cout << "[INFO] Connecting to server @ " << server_ip << ":" << server_port << "..." << endl;
        if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
            throw runtime_error("[ERROR] Connection to server failed");
        }
        cout << "[INFO] Connected to the server (: yay!" << endl;
    } catch (const exception& e) {
        cout << e.what() << endl; // logging out exceptions for debugging and user info
        close_socket(); 
        throw;  // Re-throwing
    }
}

void Client::run() {
    try {
        string message;
        while (true) {
            cout << "Enter message (type QUIT to exit): ";
            getline(cin, message); // reading input

            if (message.empty()) {
                cout << "[ERROR] Message cannot be empty. Please try again!" << endl;
                continue;
            }

            // Send the message to the server
            send_message(message);

            // If the message is "QUIT" exit the loop
            if (message == "QUIT") {
                cout << "[INFO] QUIT command! Exiting client" << endl;
                break;
            }

            // Receive the server's response
            string response = receive_message();
            cout << "Server: " << response << endl;
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
}
