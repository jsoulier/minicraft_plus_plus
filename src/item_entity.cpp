#include <savepoint/visitor.hpp>

#include "item.hpp"
#include "item_entity.hpp"

MppItemEntity::MppItemEntity()
    : MppEntity()
    , Item{kMppItemInvalid}
{
}

MppItemEntity::MppItemEntity(const MppItem& item)
    : MppEntity()
    , Item{item}
{
}

void MppItemEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    MppEntity::Update(level, renderer, ticks);
}

void MppItemEntity::Visit(SavepointVisitor& visitor)
{
    visitor(Item);
}

int MppItemEntity::GetSize() const
{
    return MppItem::kWidth;
}

int MppItemEntity::GetPhysicsWidth() const
{
    return Item.GetPhysicsWidth();
}

int MppItemEntity::GetPhysicsHeight() const
{
    return Item.GetPhysicsHeight();
}

int MppItemEntity::GetPhysicsOffsetX() const
{
    return Item.GetPhysicsOffsetX();
}

int MppItemEntity::GetPhysicsOffsetY() const
{
    return Item.GetPhysicsOffsetY();
}
