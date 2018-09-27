#include "server.h"
#include "client.h"
#include "clientmanager.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdexcept>
#include <iostream>
#include <thread>

Server::Server()
{
}

Server::~Server()
{
}

void Server::client_manager_thread(Server* server)
{
    while(1)
    {
        // Process the client manager
        server->getClientManager()->process();
    }
}



void Server::listenOnPort(int port, NEW_CLIENT_HANDLER* c_handler)
{
    this->client_manager = std::make_unique<ClientManager>();
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::logic_error("Failed to create socket descriptor");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        throw std::logic_error("Failed to set port rules");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        throw std::logic_error("Failed to bind the socket to port: " + std::to_string(port));
    }

    // We allow for 3 pending connections waiting to be accepted or forcefully fail the connection
    if (listen(server_fd, 3) < 0)
    {
        throw std::logic_error("Failed to listen on port: " + std::to_string(port));
    }

    // Let's run the client manager thread
    std::thread cm_thread(Server::client_manager_thread, this);

    // We wont fail on accepting sockets as this could be used to create a denial of service attack causing our server to shutdown
    while (1)
    {
        // Accept is blocking
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        std::unique_ptr<Client> client = std::unique_ptr<Client>(c_handler(this->client_manager.get(), new_socket, address));
        this->client_manager->addClient(std::move(client));
    }

    // Finish the client manager thread
    cm_thread.join();

}

ClientManager* Server::getClientManager()
{
    return this->client_manager.get();
}