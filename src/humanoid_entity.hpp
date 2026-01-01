#pragma once

#include <memory>

#include "mob_entity.hpp"

class MppFurnitureEntity;

class MppHumanoidEntity : public MppMobEntity
{
public:
    MppHumanoidEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    void Visit(SavepointVisitor& visitor) override;
    void Action(MppLevel& level, MppRenderer& renderer, int ticks) override;
    void Move(MppLevel& level, int dx, int dy, int ticks) override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;

protected:
    virtual int GetSpriteBorderColor() const = 0;
    virtual int GetSpriteSkinColor() const = 0;
    virtual int GetSpriteShirtColor() const = 0;
    virtual int GetSpritePantColor() const = 0;
    virtual int GetSpriteShoeColor() const = 0;
    virtual int GetAnimationSpeed() const;

private:
    std::shared_ptr<MppFurnitureEntity> Held;
    // TODO: ugly hack for avoiding recursive call for HeldCallback
    bool IsInHeldCallback;
    bool Flip;
};