#include <savepoint/savepoint.hpp>

#include "../color.hpp"
#include "creature.hpp"
#include "sheep.hpp"

MppSheepEntity::MppSheepEntity()
    : MppCreatureEntity()
{
}

int MppSheepEntity::GetSpritePoseX() const
{
    return 8;
}

int MppSheepEntity::GetSpritePoseY() const
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
