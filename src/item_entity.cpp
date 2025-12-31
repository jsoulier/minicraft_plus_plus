#include <savepoint/visitor.hpp>

#include "item.hpp"
#include "item_entity.hpp"
#include "physics.hpp"

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
    // TODO: draw
}

void MppItemEntity::Visit(SavepointVisitor& visitor)
{
    visitor(Item);
}

const MppItem& MppItemEntity::GetItem() const
{
    return Item;
}

int MppItemEntity::GetSize() const
{
    return MppItem::kWidth;
}

MppPhysicsType MppItemEntity::GetPhysicsType() const
{
    return MppPhysicsTypeItemEntity;
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
