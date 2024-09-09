#ifndef COMMS_H // prevents uplicate header file inclusions
#define COMMS_H

#include <iostream>
#include <sys/socket.h> // socket related
#include <arpa/inet.h> // socket related, IP address manipulation
#include <unistd.h> // socket related
#include <stdexcept> // used for exception classes like runtime_error
#include <string>

using namespace std;

class Comms {
protected:
    int sock;  // nuber representing socket in system
    struct sockaddr_in address;  // Address structure

public:
    // Constructor initialising sock
    Comms();

    void create_socket();

    // Sending a message over the socket
    void send_message(const string& message);

    // Receiving a message from the socket
    string receive_message();

    // Closing the socket
    void close_socket();

    // destructor making use of the close_socket to ensure deletion
    // used as virtual to make sure its cleaned up properly
    virtual ~Comms();
};

#endif
