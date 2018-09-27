#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <memory>

class Client;
class ClientManager;

typedef Client* NEW_CLIENT_HANDLER(ClientManager* c_manager, int socket_descriptor, struct sockaddr_in address);
class Server
{
public:
    Server();
    virtual ~Server();
    void listenOnPort(int port, NEW_CLIENT_HANDLER* c_handler);
    ClientManager* getClientManager();
private:
    std::unique_ptr<ClientManager> client_manager;
    static void client_manager_thread(Server* server);
};

#endif