#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/color.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/controller/melee.hpp>
#include <minicraft++/entity/mob/spider.hpp>

MppSpiderEntity::MppSpiderEntity()
    : MppMobEntity()
{
}

void MppSpiderEntity::Render() const
{
    MppMobEntity::Render();
}

int MppSpiderEntity::GetSpritePose1X() const
{
    return 8;
}

int MppSpiderEntity::GetSpritePose1Y() const
{
    return 7;
}

int MppSpiderEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppSpiderEntity::GetPhysicsOffsetY() const
{
    return 0;
}

int MppSpiderEntity::GetPhysicsWidth() const
{
    return 16;
}

int MppSpiderEntity::GetPhysicsHeight() const
{
    return 16;
}

std::shared_ptr<MppController> MppSpiderEntity::GetController() 
{
    return std::make_shared<MppMeleeController>();
}

int MppSpiderEntity::GetMaxHealth() const
{
    return 1;
}

int MppSpiderEntity::GetMaxHunger() const
{
    return 1;
}

int MppSpiderEntity::GetMaxEnergy() const
{
    return 1;
}

int MppSpiderEntity::GetSpriteColor1() const
{
    return 0;
}

int MppSpiderEntity::GetSpriteColor2() const
{
    return 500;
}

int MppSpiderEntity::GetSpriteColor3() const
{
    return 222;
}

int MppSpiderEntity::GetSpriteColor4() const
{
    return 111;
}

int MppSpiderEntity::GetSpriteColor5() const
{
    return 222;
}

int MppSpiderEntity::GetMoveTickRate() const
{
    return 3;
}
