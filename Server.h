#ifndef SERVER_H
#define SERVER_H

#include "comms.h"

class Server : public Comms { // inheriting from comms
private:
    int listen_sock; //listening sockeet

public:
    // Constructor initialised with port
    Server(int server_port);

    // Runing the server
    void run();
};

#endif
