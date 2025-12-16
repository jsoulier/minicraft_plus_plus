#include "humanoid_entity.hpp"

MppHumanoidEntity::MppHumanoidEntity()
    : MppMobEntity()
{
}

float MppHumanoidEntity::GetPhysicsWidth() const
{
    return 12;
}

float MppHumanoidEntity::GetPhysicsHeight() const
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