#include "server.h"

Server::Server(int server_port) : Comms(), listen_sock(-1) {
    try {
        cout << "[INFO] Starting server..." << endl;
        // Creating a listening socket
        listen_sock = socket(AF_INET, SOCK_STREAM, 0); // Same socket type as Client
        if (listen_sock < 0) {
            throw runtime_error("[ERROR] Server socket creation failed! :()");
        }
        cout << "[INFO] Listening socket created! Socket: " << listen_sock << endl;

        // setting socket options to reuse the address to prevent restart errors
        int opt = 1;
        if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            throw runtime_error("[ERROR] setsockopt failed.");
        }
        cout << "[INFO] Socket options set succeffully" << endl;

        // Setting up the server address
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr)); // zeroing address
        server_addr.sin_family = AF_INET; // IPv4
        server_addr.sin_addr.s_addr = INADDR_ANY;  // binding to any available network interface
        server_addr.sin_port = htons(server_port); // htons used for byte order management

        // binding the server socket to the address and port
        cout << "[INFO] Binding to port " << server_port << "..." << endl;
        if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            throw runtime_error("[ERROR] Bind failed..");
        }
        cout << "[INFO] Bind successful" << endl;

        // listening for incoming connections (up to 5)
        if (listen(listen_sock, 5) < 0) {
            throw runtime_error("[ERROR] Listen failed");
        }
        cout << "[INFO] Server listening for incoming connections on port " << server_port << "..." << endl;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        if (listen_sock != -1) {
            close(listen_sock);
        }
        throw;  // re-throwing so it is sent up the stack
    }
}

void Server::run() {
    try {
        cout << "[INFO] Waiting for a client to connect..." << endl;
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_addr_size); // accept waits for client to connect and returns socket of connection
        if (client_sock < 0) {
            throw runtime_error("[ERROR] Client accept fail");
        }
        cout << "[INFO] Client connected. Socket: " << client_sock << endl;

        // Assigning client socket to 'sock' in Comms
        sock = client_sock;

        string client_ip = inet_ntoa(client_addr.sin_addr); // converting to a readbale format
        int client_port = ntohs(client_addr.sin_port);
        cout << "[INFO] Client IP: " << client_ip << ", Port: " << client_port << endl;

        // Communication loop
        string message;
        while (true) {
            message = receive_message();

            if (message == "QUIT") {
                cout << "[INFO] Client sent quit request. Closing connection..." << endl;
                break;
            }

            cout << "Client: " << message << endl;

            // Prompting the server to enter a response
            cout << "Enter message: ";
            getline(cin, message);

            if (message.empty()) {
                cout << "[ERROR] Message cant be empty. Please try again!" << endl;
                continue;
            }

            // Send the response to the client
            send_message(message);
        }

        // Close the client socket
        close_socket();
        cout << "[INFO] Client connection closed." << endl;

        // Close the listening socket
        if (listen_sock != -1) {
            cout << "[INFO] Closing listening socket" << endl;
            close(listen_sock);
            listen_sock = -1;
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        if (sock != -1) {
            close_socket();
        }
        if (listen_sock != -1) {
            close(listen_sock);
        }
    }
}
