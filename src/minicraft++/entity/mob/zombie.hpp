#pragma once

#include <memory>

#include <minicraft++/entity/mob/humanoid.hpp>

class MppZombieEntity final : public MppHumanoidEntity
{
    SAVEPOINT_POLY(MppZombieEntity)

public:
    MppZombieEntity();
    
protected:
    std::shared_ptr<MppController> GetDefaultController();
    int GetSpriteColor1() const override;
    int GetSpriteColor2() const override;
    int GetSpriteColor3() const override;
    int GetSpriteColor4() const override;
    int GetSpriteColor5() const override;
    int GetMoveTickRate() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
};