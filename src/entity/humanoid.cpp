#include <savepoint/visitor.hpp>

#include <cstdint>

#include "humanoid.hpp"

void MppHumanoidEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(HeldEntity);
}

void MppHumanoidEntity::Update(uint64_t ticks) 
{
    MppMobEntity::Update(ticks);
}

void MppHumanoidEntity::Render() const
{
    MppMobEntity::Render();
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
