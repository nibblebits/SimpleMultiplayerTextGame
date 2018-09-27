#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include <vector>
#include <memory>
#include <mutex>
#include "statics.h"
class Client;
class ClientManager
{
public:
    ClientManager();
    virtual ~ClientManager();
    void addClient(std::unique_ptr<Client> client);
    std::vector<Client*> getClientsAtPosition(POSITION pos);
    void process();
private:
    std::vector<std::unique_ptr<Client>> clients;
    std::mutex client_lock;
};
#endif