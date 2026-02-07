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
    return std::make_shared<MppCreatureController>();
}
