#include "chest_entity.hpp"

MppItemID MppChestEntity::GetItemID() const
{
    return MppItemIDChest;
}

bool MppChestEntity::IsConvertableToItem() const
{
    // TODO: check if inventory is empty
    return true;
}

int MppChestEntity::GetSpriteX() const
{
    return 0;
}

int MppChestEntity::GetSpriteY() const
{
    return 0;
}

int MppChestEntity::GetColor1() const
{
    return 0;
}

int MppChestEntity::GetColor2() const
{
    return 0;
}

int MppChestEntity::GetColor3() const
{
    return 0;
}

int MppChestEntity::GetColor4() const
{
    return 0;
}

int MppChestEntity::GetColor5() const
{
    return 0;
}
