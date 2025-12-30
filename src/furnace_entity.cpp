#include "item.hpp"
#include "furnace_entity.hpp"

std::string_view MppFurnaceEntity::GetName() const
{
    return "CRAFTING TABLE";
}

MppItemRecipeType MppFurnaceEntity::GetRecipeType() const
{
    return MppItemRecipeTypeFurnace;
}

int MppFurnaceEntity::GetSpriteX() const
{
    return 0;
}

int MppFurnaceEntity::GetSpriteY() const
{
    return 0;
}

int MppFurnaceEntity::GetColor1() const
{
    return 0;
}

int MppFurnaceEntity::GetColor2() const
{
    return 0;
}

int MppFurnaceEntity::GetColor3() const
{
    return 0;
}

int MppFurnaceEntity::GetColor4() const
{
    return 0;
}

int MppFurnaceEntity::GetColor5() const
{
    return 0;
}

