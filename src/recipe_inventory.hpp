#pragma once

#include <string_view>

#include "inventory.hpp"

class MppRecipeInventory : public MppInventory
{
public:
    MppRecipeInventory();
    void Action() override;
    
protected:
    std::string_view GetName() const override;
};