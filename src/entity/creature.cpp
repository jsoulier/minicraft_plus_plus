#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "creature.hpp"
#include "mob.hpp"

MppCreatureEntity::MppCreatureEntity()
    : MppMobEntity()
{
}

void MppCreatureEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
}

void MppCreatureEntity::Update(uint64_t ticks)
{
    MppMobEntity::Update(ticks);
}

void MppCreatureEntity::Render() const
{
    MppMobEntity::Render();
}
