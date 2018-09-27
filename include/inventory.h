#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>

struct inventory_item
{
    int itemId = -1;
    int quantity = -1;  
};

class Inventory
{
public:
   Inventory(int size);
   virtual ~Inventory();
   bool addItem(int item_id, int amount);
   void incrementQuantity(int item_id, int amount);
   struct inventory_item getItem(int item_id);
   
   int size;
   std::vector<struct inventory_item> items;
};
#endif;