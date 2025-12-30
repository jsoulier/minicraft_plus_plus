#include "item.hpp"
#include "crafting_table_entity.hpp"

std::string_view MppCraftingTableEntity::GetName() const
{
    return "CRAFTING TABLE";
}

MppItemRecipeType MppCraftingTableEntity::GetRecipeType() const
{
    return MppItemRecipeTypeCraftingBench;
}

int MppCraftingTableEntity::GetSpriteX() const
{
    return 0;
}

int MppCraftingTableEntity::GetSpriteY() const
{
    return 0;
}

int MppCraftingTableEntity::GetColor1() const
{
    return 0;
}

int MppCraftingTableEntity::GetColor2() const
{
    return 0;
}

int MppCraftingTableEntity::GetColor3() const
{
    return 0;
}

int MppCraftingTableEntity::GetColor4() const
{
    return 0;
}

int MppCraftingTableEntity::GetColor5() const
{
    return 0;
}
