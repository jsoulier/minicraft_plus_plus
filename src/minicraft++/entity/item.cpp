#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/item.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>

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

bool MppItemEntity::OnCollision(MppEntity& instigator)
{
    MppEntity::OnCollision(instigator);
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    if (mob && mob->GetInventory()->Add(Item))
    {
        Kill();
    }
    return false;
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
