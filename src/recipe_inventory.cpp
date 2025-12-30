#include <string_view>

#include "recipe_inventory.hpp"

MppRecipeInventory::MppRecipeInventory()
    : MppInventory(0)
{
}

void MppRecipeInventory::Action()
{
    // TODO: craft
}

std::string_view MppRecipeInventory::GetName() const
{
    // TODO: crafting table, furnace, etc
    return "TODO";
}
