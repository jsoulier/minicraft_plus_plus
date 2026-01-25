#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../color.hpp"
#include "../renderer.hpp"
#include "../sprite.hpp"
#include "humanoid.hpp"

static constexpr int kTickRate = 10;

void MppHumanoidEntity::OnAddEntity()
{
    MppMobEntity::OnAddEntity();
    Animation.SetTickRate(kTickRate);
    Animation.SetPose(0, GetSpritePose1X(), GetSpritePose1Y());
    Animation.SetPose(1, GetSpritePose2X(), GetSpritePose2Y());
    // TODO: having to tick here is error prone
    Animation.Update(0, FacingX, FacingY, 0);
}

void MppHumanoidEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(HeldEntity);
}

void MppHumanoidEntity::Update(uint64_t ticks) 
{
    // TODO: pose
    Animation.Update(0, VelocityX, VelocityY, ticks);
    MppMobEntity::Update(ticks);
}

void MppHumanoidEntity::Render() const
{
    MppMobEntity::Render();
    MppRendererDraw(
        MppSprite{
            GetSpriteBorderColor(),
            GetSpriteShirtColor(),
            GetSpritePantColor(),
            GetSpriteSkinColor(),
            GetSpriteShoeColor(),
            Animation.GetX(),
            Animation.GetY(),
            GetSize(),
        },
        X,
        Y,
        Animation.GetFlip(),
        MppRendererLayerEntity);
}

int MppHumanoidEntity::GetPhysicsOffsetX() const
{
    return 2;
}

int MppHumanoidEntity::GetPhysicsOffsetY() const
{
    return 1;
}

int MppHumanoidEntity::GetPhysicsWidth() const
{
    return 12;
}

int MppHumanoidEntity::GetPhysicsHeight() const
{
    return 14;
}

int MppHumanoidEntity::GetMaxItems() const
{
    return 32;
}

int MppHumanoidEntity::GetSpritePose1X() const
{
    return 0;
}

int MppHumanoidEntity::GetSpritePose1Y() const
{
    return 6;
}

int MppHumanoidEntity::GetSpritePose2X() const
{
    return 4;
}

int MppHumanoidEntity::GetSpritePose2Y() const
{
    return 6;
}
