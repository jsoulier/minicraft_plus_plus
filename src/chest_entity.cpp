#include "chest_entity.hpp"
#include "color.hpp"
#include "item.hpp"

MppItemID MppChestEntity::GetItemID() const
{
    return MppItemIDChest;
}

bool MppChestEntity::IsConvertibleToItem() const
{
    // TODO: check if inventory is empty
    return true;
}

int MppChestEntity::GetSpriteX() const
{
    return 2;
}

int MppChestEntity::GetSpriteY() const
{
    return 17;
}

int MppChestEntity::GetColor1() const
{
    return kMppColorChest1;
}

int MppChestEntity::GetColor2() const
{
    return kMppColorChest2;
}

int MppChestEntity::GetColor3() const
{
    return kMppColorChest3;
}

int MppChestEntity::GetColor4() const
{
    return kMppColorChest4;
}

int MppChestEntity::GetColor5() const
{
    return kMppColorChest5;
}
