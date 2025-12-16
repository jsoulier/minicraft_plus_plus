#pragma once

#include "mob_entity.hpp"

class MppHumanoidEntity : public MppMobEntity
{
public:
    MppHumanoidEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    void Move(MppLevel& level, int dx, int dy, int ticks) override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;

protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    virtual int GetSpriteBorderColor() const = 0;
    virtual int GetSpriteSkinColor() const = 0;
    virtual int GetSpriteShirtColor() const = 0;
    virtual int GetSpritePantColor() const = 0;
    virtual int GetAnimationSpeed() const;

private:
    bool Flip;
};