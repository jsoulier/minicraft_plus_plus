#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/mob/creature.hpp>
#include <minicraft++/entity/mob/sheep.hpp>

MppSheepEntity::MppSheepEntity()
    : MppCreatureEntity()
{
}

int MppSheepEntity::GetAnimationPose1X() const
{
    return 12;
}

int MppSheepEntity::GetAnimationPose1Y() const
{
    return 9;
}

int MppSheepEntity::GetColor1() const
{
    return 0;
}

int MppSheepEntity::GetColor2() const
{
    return 555;
}

int MppSheepEntity::GetColor3() const
{
    return 0;
}

int MppSheepEntity::GetColor4() const
{
    return 532;
}

int MppSheepEntity::GetColor5() const
{
    return 0;
}

int MppSheepEntity::GetPhysicsOffsetX() const
{
    return 1;
}

int MppSheepEntity::GetPhysicsOffsetY() const
{
    return 1;
}

int MppSheepEntity::GetPhysicsWidth() const
{
    return 14;
}

int MppSheepEntity::GetPhysicsHeight() const
{
    return 14;
}

int MppSheepEntity::GetMaxHealth() const
{
    return 1;
}

int MppSheepEntity::GetMaxHunger() const
{
    return 1;
}

int MppSheepEntity::GetMaxEnergy() const
{
    return 1;
}
