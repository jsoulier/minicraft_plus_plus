#pragma once

#include <savepoint/base.hpp>

#include "recipe_entity.hpp"

class MppCraftingTableEntity : public MppRecipeEntity
{
    SAVEPOINT_DERIVED(MppCraftingTableEntity)

public:
    std::string_view GetName() const override;
    MppItemRecipeType GetRecipeType() const override;
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
};