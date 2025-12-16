#include "item.hpp"
#include "item_entity.hpp"

void MppItemEntity::Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks)
{
    MppEntity::Update(level, renderer, dt, ticks);
}

int MppItemEntity::GetSize() const
{
    return MppItem::kWidth;
}

float MppItemEntity::GetPhysicsWidth() const
{
    return 0;
}

float MppItemEntity::GetPhysicsHeight() const
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
