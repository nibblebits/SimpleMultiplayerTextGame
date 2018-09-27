
#include "server.h"
#include "client.h"
#include "clientmanager.h"
#include <iostream>

/**
 * Handles new clients accepted by the server
 */
Client* new_client_function(ClientManager* c_manager, int socket_descriptor, struct sockaddr_in address)
{
    return new Client(c_manager, socket_descriptor, address);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "You must provide a port to listen on" << std::endl;
        return 1;
    }
    int port = atoi(argv[1]);
    Server server;
    std::cout << "Listening on port: " << argv[1] << std::endl;
    server.listenOnPort(port, new_client_function);

    return 0;
}