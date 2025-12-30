#pragma once

#include "furniture_entity.hpp"
#include "item.hpp"
#include "recipe_menu.hpp"

class MppRenderer;

class MppRecipeEntity : public MppFurnitureEntity, public MppRecipeMenu
{
public:
    void Draw(MppRenderer& renderer);
    MppPhysicsType GetPhysicsType() const override;

private:
};