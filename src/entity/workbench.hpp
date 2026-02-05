#pragma once

#include <savepoint/savepoint.hpp>

#include "recipe.hpp"

class MppWorkbenchEntity final : public MppRecipeEntity
{
    SAVEPOINT_POLY(MppWorkbenchEntity)

public:
    MppWorkbenchEntity();
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
