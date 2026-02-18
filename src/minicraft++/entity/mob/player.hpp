#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/entity/mob/humanoid.hpp>

class MppPlayerEntity final : public MppHumanoidEntity
{
    SAVEPOINT_POLY(MppPlayerEntity)

public:
    MppPlayerEntity();

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