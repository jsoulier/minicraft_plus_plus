#include <savepoint/savepoint.hpp>

#include <cmath>
#include <cstdint>

#include "../assert.hpp"
#include "../color.hpp"
#include "../console.hpp"
#include "../renderer.hpp"
#include "../sprite.hpp"
#include "../tile.hpp"
#include "../util.hpp"
#include "../world.hpp"
#include "controller/creature.hpp"
#include "creature.hpp"
#include "mob.hpp"

static constexpr int kTickRate = 10;

MppCreatureEntity::MppCreatureEntity()
    : MppMobEntity()
    , Animation{}
    , FleeTicks{}
{
}

void MppCreatureEntity::OnAddEntity()
{
    MppMobEntity::OnAddEntity();
    Animation.SetTickRate(kTickRate);
    Animation.SetPose(0, GetSpritePoseX(), GetSpritePoseY());
    // TODO: having to update here is error prone
    Animation.Update(0, FacingX, FacingY, 0);
}

void MppCreatureEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(FleeTicks);
}

void MppCreatureEntity::PostUpdate(uint64_t ticks)
{
    MppMobEntity::PostUpdate(ticks);
    FleeTicks--;
    Animation.Update(0, VelocityX, VelocityY, ticks);
}

void MppCreatureEntity::Render() const
{
    MppMobEntity::Render();
    MppRendererDraw(
        MppSprite{
            GetSpriteColor1(),
            GetSpriteColor2(),
            GetSpriteColor3(),
            GetSpriteColor4(),
            GetSpriteColor5(),
            Animation.GetX(),
            Animation.GetY(),
            GetSize(),
        },
        X,
        Y,
        Animation.GetFlip(),
        MppRendererLayerEntity);
}

void MppCreatureEntity::OnAction(MppEntity& instigator)
{
    MppMobEntity::OnAction(instigator);
    // TODO: set FleeTicks here
}

int MppCreatureEntity::GetSpeed() const
{
    if (FleeTicks <= 0)
    {
        return GetDefaultSpeed();
    }
    else
    {
        return GetFleeSpeed();
    }
}

int MppCreatureEntity::GetDefaultSpeed() const
{
    return 3;
}

int MppCreatureEntity::GetFleeSpeed() const
{
    return 1;
}

int MppCreatureEntity::GetIdleCooldown() const
{
    return 120;
}

int MppCreatureEntity::GetFleeCooldown() const
{
    return 300;
}

int MppCreatureEntity::GetFleeTicks() const
{
    return FleeTicks;
}

std::shared_ptr<MppController> MppCreatureEntity::GetController() 
{
    return std::make_shared<MppCreatureController>(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
}
