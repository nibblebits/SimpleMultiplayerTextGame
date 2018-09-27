#ifndef PLAYER_H
#define PLAYER_H

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <memory>
#include "statics.h"

struct xp
{
    int hp = 1;
    int atk = 1;
    int str = 1;
    int def = 1;
    int mage = 1;
};

class Inventory;
class Player
{
public:
    Player();
    virtual ~Player();

    std::string name;
    struct xp xp;
    POSITION position;
    std::unique_ptr<Inventory> inventory;
};

#endif