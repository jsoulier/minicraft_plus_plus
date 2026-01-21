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

MppMobEntity::~MppMobEntity()
{
    // TODO: why does it crash without the empty destructor?
    int i = 0;
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

void MppMobEntity::Render()
{
    MppEntity::Render();
    MppRendererDraw(MppSprite(kMppColorWhite), X, Y, false, MppRendererLayerDebug);
}

int MppMobEntity::GetSize() const
{
    return 16;
}

std::shared_ptr<MppInventory> MppMobEntity::GetInventory()
{
    return Inventory;
}
