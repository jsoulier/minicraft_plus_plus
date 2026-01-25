#include <savepoint/savepoint.hpp>

#include "../entity.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../log.hpp"
#include "../renderer.hpp"
#include "item.hpp"
#include "mob.hpp"

MppItemEntity::MppItemEntity()
    : Item{kMppItemInvalid}
{
}

MppItemEntity::MppItemEntity(const MppItem& item)
    : Item{item}
{
}

void MppItemEntity::Render() const
{
    MppEntity::Render();
    Item.Render(X, Y, MppRendererLayerEntity);
}

void MppItemEntity::OnCollision(MppEntity& instigator)
{
    MppEntity::OnCollision(instigator);
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    if (!mob)
    {
        return;
    }
    if (mob->GetInventory()->Add(Item))
    {
        Kill();
    }
}

void MppItemEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
    visitor(Item);
}

int MppItemEntity::GetPhysicsOffsetX() const
{
    return Item.GetPhysicsOffsetX();
}

int MppItemEntity::GetPhysicsOffsetY() const
{
    return Item.GetPhysicsOffsetY();
}

int MppItemEntity::GetPhysicsWidth() const
{
    return Item.GetPhysicsWidth();
}

int MppItemEntity::GetPhysicsHeight() const
{
    return Item.GetPhysicsHeight();
}

int MppItemEntity::GetSize() const
{
    return MppItem::kSize;
}
