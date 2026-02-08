#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/color.hpp>
#include <minicraft++/entity/controller/melee.hpp>
#include <minicraft++/entity/mob/zombie.hpp>

MppZombieEntity::MppZombieEntity()
    : MppHumanoidEntity()
{
}

std::shared_ptr<MppController> MppZombieEntity::GetController() 
{
    return std::make_shared<MppMeleeController>();
}

int MppZombieEntity::GetSpriteColor1() const 
{
    return 0;
}

int MppZombieEntity::GetSpriteColor2() const 
{
    return 420;
}

int MppZombieEntity::GetSpriteColor3() const 
{
    return 420;
}

int MppZombieEntity::GetSpriteColor4() const 
{
    return 40;
}

int MppZombieEntity::GetSpriteColor5() const 
{
    return 210;
}

int MppZombieEntity::GetMaxHealth() const
{
    return 1;
}

int MppZombieEntity::GetMaxHunger() const
{
    return 1;
}

int MppZombieEntity::GetMaxEnergy() const
{
    return 1;
}
