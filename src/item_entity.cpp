#include "item.hpp"
#include "item_entity.hpp"

void MppItemEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    MppEntity::Update(level, renderer, ticks);
}

int MppItemEntity::GetSize() const
{
    return MppItem::kWidth;
}

int MppItemEntity::GetPhysicsWidth() const
{
    return 0;
}

int MppItemEntity::GetPhysicsHeight() const
{
    return 0;
}

int MppItemEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppItemEntity::GetPhysicsOffsetY() const
{
    return 0;
}
