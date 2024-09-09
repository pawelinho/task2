#include <iostream>
#include "server.h"
#include "client.h"

using namespace std;

// Using the def below to test server and non server mode
// #define SERVER_MODE

// To run in server mode I used this compilation code:
// g++ main.cpp comms.cpp client.cpp server.cpp -o app_server -lpthread -DSERVER_MODE
// and this to compile as client:
// g++ main.cpp comms.cpp client.cpp server.cpp -o app_client -lpthread
// and then  ran ./app_client and ./app_server in separate terminals

int main(int argc, char* argv[]) {
    try {
        #ifdef SERVER_MODE
        cout << "[INFO] Starting in server mode..." << endl;
        int server_port = 8080;  // Default port
        Server server(server_port);
        server.run();
        #else
        cout << "[INFO] Starting in client mode..." << endl;
        string server_ip = "127.0.0.1";
        int server_port = 8080;  // Default port
        Client client(server_ip, server_port);
        client.run();
        #endif
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        return 1;
    }

    return 0;
}
