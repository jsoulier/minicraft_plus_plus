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
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
    int GetMoveTickRate() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
};