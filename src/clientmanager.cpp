#include "clientmanager.h"
#include "client.h"
#include "player.h"
ClientManager::ClientManager()
{

}

ClientManager::~ClientManager()
{

}

void ClientManager::addClient(std::unique_ptr<Client> client)
{
    this->client_lock.lock();
    client->start();
    this->clients.push_back(std::move(client));
    this->client_lock.unlock();
}

std::vector<Client*> ClientManager::getClientsAtPosition(POSITION pos)
{
    std::vector<Client*> clients;
    for (int i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i]->getPlayer()->position == pos)
            clients.push_back(this->clients[i].get());
    }

    return clients;
}

void ClientManager::process()
{
    this->client_lock.lock();
    for (int i = 0; i < this->clients.size(); i++)
    {
        this->clients[i]->process();
        this->clients[i]->dialogue();
        std::string output_data = this->clients[i]->getOutputData();
        send(this->clients[i]->getSocketDescriptor(), output_data.c_str(), output_data.size(), 0);
        this->clients[i]->resetOutputData();
    }
    this->client_lock.unlock();
}
