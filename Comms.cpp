#include "comms.h"

Comms::Comms() : sock(-1) { // initialising with -1 to start with an invalid socket
    memset(&address, 0, sizeof(address)); //clearing the address as part of initialisation
}

void Comms::create_socket() {
    cout << "[INFO] Creating socket..." << endl;
    sock = socket(AF_INET, SOCK_STREAM, 0);  // Creates a TCP socket (AF_INET = IPv4), (Sock_Stream = TCP) (0 auto selects protocol)
    if (sock < 0) { // if sock is negative then something went wrong - throw error
        throw runtime_error("[ERROR] Socket creation failed");
    }
    // printing out the int if successfull
    cout << "[INFO] Socket created successfully. Socket: " << sock << endl;
}

void Comms::send_message(const string& message) {
    cout << "[INFO] Sending message: " << message << endl;
    ssize_t bytes_sent = send(sock, message.c_str(), message.length(), 0); // message.c_str() converts message into the necessary char format
    if (bytes_sent < 0) { // bytes < 0 = error 
        throw runtime_error("[ERROR] Send failed");
    }
}

string Comms::receive_message() {
    char buffer[1024] = {0}; // creating buffer
    cout << "[INFO] Waiting to receive message..." << endl;
    ssize_t bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0); // reading data from socket into buffer
    if (bytes_received == 0) {
        // Connection closed by client/server
        throw runtime_error("[INFO] Connection closed by client/server");
    } else if (bytes_received < 0) {
        throw runtime_error("[ERROR] Receive failed.");
    }
    string message(buffer, bytes_received); // message is returned as a string
    cout << "[INFO] Received message: " << message << endl;
    return message;
}

void Comms::close_socket() {
    if (sock != -1) { // only attempt if socket is valid
        cout << "[INFO] Closing socket: " << sock << endl;
        close(sock);
        sock = -1;
    }
}

Comms::~Comms() {
    close_socket(); // closing socket in destructor
    cout << "[INFO] Socket closed" << endl;
}
