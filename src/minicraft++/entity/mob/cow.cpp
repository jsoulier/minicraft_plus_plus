#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/mob/creature.hpp>
#include <minicraft++/entity/mob/cow.hpp>

MppCowEntity::MppCowEntity()
    : MppCreatureEntity()
{
}

int MppCowEntity::GetAnimationPose1X() const
{
    return 12;
}

int MppCowEntity::GetAnimationPose1Y() const
{
    return 7;
}

int MppCowEntity::GetColor1() const
{
    return 0;
}

int MppCowEntity::GetColor2() const
{
    return 210;
}

int MppCowEntity::GetColor3() const
{
    return 444;
}

int MppCowEntity::GetColor4() const
{
    return 320;
}

int MppCowEntity::GetColor5() const
{
    return 444;
}

int MppCowEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppCowEntity::GetPhysicsOffsetY() const
{
    return 0;
}

int MppCowEntity::GetPhysicsWidth() const
{
    return 16;
}

int MppCowEntity::GetPhysicsHeight() const
{
    return 16;
}

int MppCowEntity::GetMaxHealth() const
{
    return 1;
}

int MppCowEntity::GetMaxHunger() const
{
    return 1;
}

int MppCowEntity::GetMaxEnergy() const
{
    return 1;
}

