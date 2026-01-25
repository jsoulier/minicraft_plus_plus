#include <savepoint/savepoint.hpp>

#include "../color.hpp"
#include "creature.hpp"
#include "sheep.hpp"

int MppSheepEntity::GetSpriteX() const
{
    return 8;
}

int MppSheepEntity::GetSpriteY() const
{
    return 6;
}

int MppSheepEntity::GetColor1() const
{
    return kMppColorSheep1;
}

int MppSheepEntity::GetColor2() const
{
    return kMppColorSheep2;
}

int MppSheepEntity::GetColor3() const
{
    return kMppColorSheep3;
}

int MppSheepEntity::GetColor4() const
{
    return kMppColorSheep4;
}

int MppSheepEntity::GetColor5() const
{
    return kMppColorSheep5;
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

