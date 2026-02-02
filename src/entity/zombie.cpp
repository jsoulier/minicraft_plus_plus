#include <savepoint/savepoint.hpp>

#include <memory>

#include "../color.hpp"
#include "controller/melee.hpp"
#include "zombie.hpp"

MppZombieEntity::MppZombieEntity()
    : MppHumanoidEntity()
{
}

std::shared_ptr<MppController> MppZombieEntity::GetController() 
{
    return std::make_shared<MppMeleeController>();
}

int MppZombieEntity::GetSpriteBorderColor() const 
{
    return 0;
}

int MppZombieEntity::GetSpriteSkinColor() const 
{
    return 40;
}

int MppZombieEntity::GetSpriteShirtColor() const 
{
    return 420;
}

int MppZombieEntity::GetSpritePantColor() const 
{
    return 420;
}

int MppZombieEntity::GetSpriteShoeColor() const 
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
