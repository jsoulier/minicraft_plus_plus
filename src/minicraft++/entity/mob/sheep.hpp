#pragma once

#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/mob/creature.hpp>

class MppSheepEntity final : public MppCreatureEntity
{
    SAVEPOINT_POLY(MppSheepEntity)

public:
    MppSheepEntity();

protected:
    int GetAnimationPose1X() const override;
    int GetAnimationPose1Y() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
};