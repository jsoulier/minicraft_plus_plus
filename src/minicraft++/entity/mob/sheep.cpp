#include <savepoint/savepoint.hpp>

#include <minicraft++/color.hpp>
#include <minicraft++/entity/mob/creature.hpp>
#include <minicraft++/entity/mob/sheep.hpp>

MppSheepEntity::MppSheepEntity()
    : MppCreatureEntity()
{
}

int MppSheepEntity::GetSpritePose1X() const
{
    return 8;
}

int MppSheepEntity::GetSpritePose1Y() const
{
    return 6;
}

int MppSheepEntity::GetSpriteColor1() const
{
    return 0;
}

int MppSheepEntity::GetSpriteColor2() const
{
    return 555;
}

int MppSheepEntity::GetSpriteColor3() const
{
    return 0;
}

int MppSheepEntity::GetSpriteColor4() const
{
    return 532;
}

int MppSheepEntity::GetSpriteColor5() const
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
