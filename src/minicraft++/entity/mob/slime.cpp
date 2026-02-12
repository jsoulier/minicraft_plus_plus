#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/color.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/controller/melee.hpp>
#include <minicraft++/entity/mob/slime.hpp>

MppSlimeEntity::MppSlimeEntity()
    : MppMobEntity()
{
}

void MppSlimeEntity::Render() const
{
    MppMobEntity::Render();
}

int MppSlimeEntity::GetSpritePose1X() const
{
    return 12;
}

int MppSlimeEntity::GetSpritePose1Y() const
{
    return 6;
}

int MppSlimeEntity::GetPhysicsOffsetX() const
{
    return 2;
}

int MppSlimeEntity::GetPhysicsOffsetY() const
{
    return 4;
}

int MppSlimeEntity::GetPhysicsWidth() const
{
    return 12;
}

int MppSlimeEntity::GetPhysicsHeight() const
{
    return 11;
}

std::shared_ptr<MppController> MppSlimeEntity::GetDefaultController() 
{
    return std::make_shared<MppMeleeController>();
}

int MppSlimeEntity::GetMaxHealth() const
{
    return 1;
}

int MppSlimeEntity::GetMaxHunger() const
{
    return 1;
}

int MppSlimeEntity::GetMaxEnergy() const
{
    return 1;
}

int MppSlimeEntity::GetSpriteColor1() const
{
    return 0;
}

int MppSlimeEntity::GetSpriteColor2() const
{
    return 40;
}

int MppSlimeEntity::GetSpriteColor3() const
{
    return 50;
}

int MppSlimeEntity::GetSpriteColor4() const
{
    return 0;
}

int MppSlimeEntity::GetSpriteColor5() const
{
    return 0;
}

int MppSlimeEntity::GetMoveTickRate() const
{
    return 5;
}

int MppSlimeEntity::GetSpeed() const
{
    return 1;
}
