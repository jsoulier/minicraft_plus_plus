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
    return kMppColorZombieBorder;
}

int MppZombieEntity::GetSpriteSkinColor() const 
{
    return kMppColorZombieSkin;
}

int MppZombieEntity::GetSpriteShirtColor() const 
{
    return kMppColorZombieShirt;
}

int MppZombieEntity::GetSpritePantColor() const 
{
    return kMppColorZombiePants;
}

int MppZombieEntity::GetSpriteShoeColor() const 
{
    return kMppColorZombieShoes;
}
