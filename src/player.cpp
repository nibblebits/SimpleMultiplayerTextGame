#include "player.h"
#include "inventory.h"
Player::Player()
{
    this->position = -1;
    this->inventory = std::make_unique<Inventory>(30);
}

Player::~Player()
{

}