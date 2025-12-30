#pragma once

#include <string_view>

#include "recipe_inventory.hpp"
#include "recipe_output_menu.hpp"

class MppRecipeMenu
{
public:

protected:
    virtual std::string_view GetName() const = 0;
    virtual MppItemRecipeType GetRecipeType() const = 0;

private:
    MppRecipeInventory Inventory;
    MppRecipeOutputMenu Output;
};