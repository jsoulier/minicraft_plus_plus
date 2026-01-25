#include <memory>

#include "../color.hpp"
#include "../item.hpp"
#include "furnace.hpp"

MppFurnaceEntity::MppFurnaceEntity()
    : MppRecipeEntity()
{
}

MppItemID MppFurnaceEntity::GetItemID() const
{
    return MppItemIDFurnace;
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
