#include <savepoint/savepoint.hpp>

#include <cmath>
#include <cstdint>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/controller/creature.hpp>
#include <minicraft++/entity/mob/creature.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

MppCreatureEntity::MppCreatureEntity()
    : MppMobEntity()
    , FleeTicks{}
{
}

void MppCreatureEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(FleeTicks);
}

void MppCreatureEntity::Update(uint64_t ticks)
{
    MppMobEntity::Update(ticks);
    FleeTicks--;
}

void MppCreatureEntity::OnAction(MppEntity& instigator)
{
    MppMobEntity::OnAction(instigator);
    // TODO: set FleeTicks here
}

int MppCreatureEntity::GetMoveTickRate() const
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
    return std::make_shared<MppCreatureController>();
}
