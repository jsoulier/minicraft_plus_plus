#pragma once

#include <savepoint/base.hpp>

#include "recipe_entity.hpp"

class MppFurnaceEntity : public MppRecipeEntity
{
    SAVEPOINT_DERIVED(MppFurnaceEntity)

public:
    MppItemID GetItemID() const override;
    
protected:
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
