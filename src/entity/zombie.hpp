#pragma once

#include <memory>

#include "humanoid.hpp"

class MppZombieEntity final : public MppHumanoidEntity
{
    SAVEPOINT_DERIVED(MppZombieEntity)

public:
    MppZombieEntity();
    
protected:
    std::shared_ptr<MppController> GetController();
    int GetSpriteBorderColor() const override;
    int GetSpriteSkinColor() const override;
    int GetSpriteShirtColor() const override;
    int GetSpritePantColor() const override;
    int GetSpriteShoeColor() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
};