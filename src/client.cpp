#include "client.h"
#include "clientmanager.h"
#include "player.h"
#include "inventory.h"
#include "itemhandler.h"
#include <fcntl.h>
#include <iostream>
#include <cstdlib>

Client::Client(ClientManager* client_manager, int socket_descriptor, struct sockaddr_in address)
{
    this->client_manager = client_manager;
    this->socket_descriptor = socket_descriptor;
    int flags = fcntl(this->socket_descriptor, F_GETFL, 0);
    fcntl(this->socket_descriptor, F_SETFL, flags | O_NONBLOCK);

    this->player = std::make_unique<Player>();
    int random = rand() % 5000;
    this->player->name = "guest_" + std::to_string(random);
    this->address = address;
    this->dialogueId = -1;
    this->dialogueOption = -1;
    this->stage = 0;    
}

Client::~Client()
{

}


int Client::getSocketDescriptor()
{
    return this->socket_descriptor;
}

std::string Client::getOutputData()
{
    return this->output.str();
}

void Client::resetOutputData()
{
    this->output.str("");
}

void Client::start()
{
    output << "Welcome to Text Scape" << std::endl
           << "Your name is: " << this->player->name << " enjoy the game. " << std::endl
           << "Use :help for a list of commands" << std::endl;
    this->player->inventory->addItem(ITEM_BEGINNERS_WAND, 20);
    this->player->inventory->addItem(ITEM_BRONZE_DAGGER, 1);
    move(TILE_BAR, false);
}

std::string Client::input(INPUT_RULES rules)
{
    std::string line_read;
    char buf[1024];
    int total_read = read(this->socket_descriptor, &buf, sizeof(buf)-1);
    if (total_read > 0)
    {
        buf[total_read+1] = 0;
        line_read = std::string(buf);
        line_read = line_read.substr(0,line_read.find_first_of("\n")-1);
        return line_read;
    }

    if (!(rules & INPUT_NO_EXCEPTION))
        throw std::logic_error("No input");

    return "";

}

Player* Client::getPlayer()
{
    return this->player.get();
}

void Client::setDialogue(int dialogueId)
{
    this->dialogueId = dialogueId;
    this->stage = 0;
}

void Client::dialogue()
{
    std::string name = "";
    try
    {
     /*   NEW_DIALOUGE(ENTER_TILE)
        STAGE(TILE_BAR)
            output << "You are inside bettys bar, it is cold and dark they look like they will go out of business soon" << std::endl;
            output << ":bathroom" << std::endl;
            LEAVE_DIALOGUE();
        END_STAGE()*/
    } 
    catch(...)
    {

    }


}

void Client::speak(std::string message)
{
    std::vector<Client*> clients_at_old_pos = this->client_manager->getClientsAtPosition(this->player->position);
    for (Client* c : clients_at_old_pos)
    {
        c->output << this->player->name << "> " << message << std::endl;
    }
}

void Client::showPositionInfo(POSITION pos)
{
    output << "\033[31;1;4m" << std::endl;
    switch(pos)
    {
        case TILE_BAR:
        {
            output << "You are inside bettys bar, it is cold and dark they look like they will go out of business soon" << std::endl;
            output << ":bathroom - Enter bettys bathroom" << std::endl;
        }
        break;

        case TILE_BAR_BATHROOM:
        {
            output << "You enter bettys bar's bathroom" << std::endl;
        }
        break;
    }

    output << "\033[0m" << std::endl;
}

void Client::showInventoryInfo()
{
    output << "\033[1;34m";
    for (struct inventory_item item : this->player->inventory->items)
    {
        output << ItemHandler::items[item.itemId].name << " x " << item.quantity << std::endl;
    }
    output << "\033[0m" << std::endl;
}

void Client::move(POSITION pos, bool alert_leaving)
{
    if (alert_leaving)
    {
        // Let everyone know we have moved
        std::vector<Client*> clients_at_old_pos = this->client_manager->getClientsAtPosition(this->player->position);
        for (Client* c : clients_at_old_pos)
        {
            if (c == this)
                continue;

            c->output << "Player: " << this->player->name << " has left this place" << std::endl;
        }
    }
  
    this->showPositionInfo(pos);

    output << std::endl;
    std::vector<Client*> clients_at_pos = this->client_manager->getClientsAtPosition(pos);
    if (clients_at_pos.size() > 0)
    {
        output << "PLAYERS AROUND YOU" << std::endl;
        for (Client* c : clients_at_pos)
        {
            output << "Player: " << c->getPlayer()->name << std::endl;
            c->output << "Player: " << this->player->name << " has entered this place" << std::endl;
        }
    }
    else
    {
        output << "YOU ARE IN THIS PLACE ON YOUR OWN" << std::endl;
    }
    this->player->position = pos;

}

void Client::handleCommand(std::string command)
{
    if (command == "")
        return;

    if (command == "stats")
    {
       output << "Hitpoints: " << this->player->xp.hp << std::endl;
       output << "Attack: " << this->player->xp.atk << std::endl;
       output << "Defense: " << this->player->xp.def << std::endl;
       output << "Strength: " << this->player->xp.str << std::endl;
       output << "Magic: " << this->player->xp.mage << std::endl;
    }
    else if(command == "info")
    {
        showPositionInfo(this->player->position);
    }
    else if(command == "inventory")
    {
        output << "\033[1;32mYOUR INVENTORY\033[0m" << std::endl;
        showInventoryInfo();
    }
    
    switch(this->player->position)
    {
        case TILE_BAR:
        {
            if (command == "bathroom")
            {
                move(TILE_BAR_BATHROOM);
            }
        }
        break;
    }
}

void Client::process()
{
    if (this->dialogueId == -1)
    {
        std::string input_data = input(INPUT_NO_EXCEPTION);
        if (input_data != "")
        {
            if (input_data.find(":") == 0)
            {
                // This is a command
                std::string command = input_data.substr(1, input_data.size());
                handleCommand(command);
            }
            else
            {
                // No command must be speech let's say hello
                speak(input_data);
            }
        }

    }

}