
#include "inventory.h"
Inventory::Inventory(int size)
{
    this->size = size;
}

Inventory::~Inventory()
{

}

bool Inventory::addItem(int item_id, int amount)
{
    struct inventory_item item = getItem(item_id);
    if (item.itemId == -1)
    {
        item.itemId = item_id;
        item.quantity = amount;
        items.push_back(item);
    }
    else
    {
        incrementQuantity(item_id, amount);
    }
}

void Inventory::incrementQuantity(int item_id, int amount)
{
    for (int i = 0; i < this->items.size(); i++)
    {
        if (this->items[i].itemId == item_id)
            this->items[i].quantity += amount;
    }
}

struct inventory_item Inventory::getItem(int item_id)
{
    for (int i = 0; i < this->items.size(); i++)
    {
        if (this->items[i].itemId == item_id)
            return this->items[i];
    }

    struct inventory_item item;
    return item;
}
