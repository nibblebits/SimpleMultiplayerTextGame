#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <memory>
#include "statics.h"

#define NEW_DIALOUGE(DIALOGUE_ID) if (this->dialogueId == DIALOGUE_ID) { \
                                  switch(this->stage) {
#define LEAVE_DIALOGUE() this->stage = -1; this->dialogueId = -1; break;
#define END_DIALOGUE() default: this->stage = -1; this->dialogueId = -1; } }
#define STAGE(number) case number: {
#define END_STAGE() this->stage +=1; break; }

class Player;
class ClientManager;
class Client
{
public:
    Client(ClientManager* client_manager, int socket_descriptor, struct sockaddr_in address);
    virtual ~Client();

    int getSocketDescriptor();
    std::string getOutputData();
    void resetOutputData();
    std::string input(INPUT_RULES rules = 0);
    Player* getPlayer();
    void showInventoryInfo();
    void showPositionInfo(POSITION pos);
    void speak(std::string message);
    void move(POSITION pos, bool alert_leaving=true);
    void handleCommand(std::string command);

    void setDialogue(int dialogueId);
    virtual void start();
    virtual void dialogue();
    virtual void process();
private:
    ClientManager* client_manager;
    int dialogueId;
    int stage = 0;
    int dialogueOption;
    std::unique_ptr<Player> player;
    int socket_descriptor;
    struct sockaddr_in address;
    std::stringstream output;
};

#endif