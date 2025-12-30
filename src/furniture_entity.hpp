#pragma once

#include "entity.hpp"
#include "recipe_inventory.hpp"
#include "recipe_output_menu.hpp"

class MppRenderer;

class MppFurnitureEntity : public MppEntity
{
public:
    virtual void Draw(MppRenderer& renderer) = 0;

private:
    MppRecipeInventory Inventory;
    MppRecipeOutputMenu Output;
};