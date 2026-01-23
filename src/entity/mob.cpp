#include <savepoint/visitor.hpp>

#include <memory>

#include "../color.hpp"
#include "../inventory.hpp"
#include "../renderer.hpp"
#include "mob.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
    , Inventory{std::make_shared<MppInventory>()}
    , VelocityX{0}
    , VelocityY{0}
    , FacingX{0}
    , FacingY{1}
{
}

void MppMobEntity::OnAddEntity()
{
    Inventory->SetMaxItems(GetMaxItems());
}

void MppMobEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
    visitor(Inventory);
    visitor(FacingX);
    visitor(FacingY);
}

void MppMobEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
    MppEntity::Move(VelocityX, VelocityY);
    if (VelocityX || VelocityY)
    {
        FacingX = VelocityX;
        FacingY = VelocityY;
    }
    VelocityX = 0;
    VelocityY = 0;
}

void MppMobEntity::Render() const
{
    MppEntity::Render();
    MppRendererDraw(MppSprite(kMppColorWhite), X, Y, false, MppRendererLayerEntity);
}

int MppMobEntity::GetSize() const
{
    return 16;
}

std::shared_ptr<MppInventory> MppMobEntity::GetInventory()
{
    return Inventory;
}

int MppMobEntity::GetMaxItems() const
{
    return 0;
}
