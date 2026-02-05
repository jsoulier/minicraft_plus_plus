#pragma once

#include <savepoint/savepoint.hpp>

#include "recipe.hpp"

class MppInventory;

class MppFurnaceEntity final : public MppRecipeEntity
{
    SAVEPOINT_POLY(MppFurnaceEntity)

public:
    MppFurnaceEntity();
    MppItemID GetItemID() const override;
    
protected:
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
};
