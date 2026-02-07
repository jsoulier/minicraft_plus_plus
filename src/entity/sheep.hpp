#pragma once

#include <savepoint/savepoint.hpp>

#include "creature.hpp"

class MppSheepEntity final : public MppCreatureEntity
{
    SAVEPOINT_POLY(MppSheepEntity)

public:
    MppSheepEntity();

protected:
    int GetSpritePose1X() const override;
    int GetSpritePose1Y() const override;
    int GetSpriteColor1() const override;
    int GetSpriteColor2() const override;
    int GetSpriteColor3() const override;
    int GetSpriteColor4() const override;
    int GetSpriteColor5() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
};