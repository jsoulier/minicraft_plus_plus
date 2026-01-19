#include "humanoid.hpp"

void MppHumanoidEntity::Update(uint64_t ticks) 
{
    MppMobEntity::Update(ticks);
}

void MppHumanoidEntity::Render() 
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
