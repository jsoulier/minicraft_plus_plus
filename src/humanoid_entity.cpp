#include "humanoid_entity.hpp"

MppHumanoidEntity::MppHumanoidEntity()
    : MppMobEntity()
{
}

int MppHumanoidEntity::GetPhysicsWidth() const
{
    return 12;
}

int MppHumanoidEntity::GetPhysicsHeight() const
{
    return 14;
}

int MppHumanoidEntity::GetPhysicsOffsetX() const
{
    return 2;
}

int MppHumanoidEntity::GetPhysicsOffsetY() const
{
    return 1;
}