#ifndef ITEMHANDLER_H
#define ITEMHANDLER_H

#include <string>
#include <vector>
#include "statics.h"
#include "item.h"

class ItemHandler
{
public:
    ItemHandler();
    virtual ~ItemHandler();
    static struct item items[];
};

#endif