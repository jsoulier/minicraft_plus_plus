#include "../color.hpp"
#include "../renderer.hpp"
#include "mob.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
    , VelocityX{0}
    , VelocityY{0}
    , FacingX{0}
    , FacingY{1}
{
}

void MppMobEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
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
