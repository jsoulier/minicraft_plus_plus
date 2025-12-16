#pragma once

#include "humanoid_entity.hpp"

class MppPlayerEntity : public MppHumanoidEntity
{
    SAVEPOINT_DERIVED(MppPlayerEntity)

public:
    MppPlayerEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;

protected:
    int GetSpriteBorderColor() const override;
    int GetSpriteShirtColor() const override;
    int GetSpritePantColor() const override;
    int GetSpriteSkinColor() const override;
};