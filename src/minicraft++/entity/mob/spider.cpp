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

int MppSpiderEntity::GetAnimationPose1X() const
{
    return 12;
}

int MppSpiderEntity::GetAnimationPose1Y() const
{
    return 10;
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

std::shared_ptr<MppController> MppSpiderEntity::GetDefaultController() 
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

int MppSpiderEntity::GetColor1() const
{
    return 0;
}

int MppSpiderEntity::GetColor2() const
{
    return 500;
}

int MppSpiderEntity::GetColor3() const
{
    return 222;
}

int MppSpiderEntity::GetColor4() const
{
    return 111;
}

int MppSpiderEntity::GetColor5() const
{
    return 222;
}

int MppSpiderEntity::GetMoveTickRate() const
{
    return 3;
}
