#pragma once

#include <vector>

#include "input.hpp"
#include "item.hpp"

class MppInventory : public MppInputHandler
{
public:

private:
    std::vector<MppItem> Items;
    int MaxItems;
};