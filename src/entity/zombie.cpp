#include <savepoint/savepoint.hpp>

#include <memory>

#include "../color.hpp"
#include "hostile_controller.hpp"
#include "zombie.hpp"

MppZombieEntity::MppZombieEntity()
    : MppHumanoidEntity()
{
}

std::shared_ptr<MppMobController> MppZombieEntity::GetController() 
{
    return std::make_shared<MppHostileMobController>(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
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
