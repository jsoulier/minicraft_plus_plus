#include <string_view>

#include "color.hpp"
#include "furnace_entity.hpp"
#include "item.hpp"

MppItemID MppFurnaceEntity::GetItemID() const
{
    return MppItemIDFurnace;
}

std::string_view MppFurnaceEntity::GetName() const
{
    return "FURNACE";
}

MppItemRecipeType MppFurnaceEntity::GetRecipeType() const
{
    return MppItemRecipeTypeFurnace;
}

int MppFurnaceEntity::GetSpriteX() const
{
    return 1;
}

int MppFurnaceEntity::GetSpriteY() const
{
    return 17;
}

int MppFurnaceEntity::GetColor1() const
{
    return kMppColorFurnace1;
}

int MppFurnaceEntity::GetColor2() const
{
    return kMppColorFurnace2;
}

int MppFurnaceEntity::GetColor3() const
{
    return kMppColorFurnace3;
}

int MppFurnaceEntity::GetColor4() const
{
    return kMppColorFurnace4;
}

int MppFurnaceEntity::GetColor5() const
{
    return kMppColorFurnace5;
}
