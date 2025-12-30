#pragma once

#include <string_view>

#include "furniture_entity.hpp"
#include "item.hpp"
#include "recipe_inventory.hpp"
#include "recipe_output_menu.hpp"

class MppRenderer;

class MppRecipeEntity : public MppFurnitureEntity
{
public:
    void Draw(MppRenderer& renderer);
    MppPhysicsType GetPhysicsType() const override;

protected:
    virtual std::string_view GetName() const = 0;
    virtual MppItemRecipeType GetRecipeType() const = 0;

private:
    MppRecipeInventory Inventory;
    MppRecipeOutputMenu Output;
};