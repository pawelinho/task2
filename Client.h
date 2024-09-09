#ifndef CLIENT_H
#define CLIENT_H

#include "comms.h"

class Client : public Comms { // inherited from Comms
public:
    // the client is initialised by creating a socket and connecting to the server
    Client(const string& server_ip, int server_port);

    // Run the client
    void run();
};

#endif
