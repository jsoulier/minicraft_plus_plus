#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include "../sprite.hpp"
#include "mob.hpp"

class MppItem;

class MppHumanoidEntity : public MppMobEntity
{
public:
    virtual void OnAddEntity() override;
    virtual void Visit(SavepointVisitor& visitor);
    virtual void PostUpdate(uint64_t ticks) override;
    virtual void Render() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetActionRange() const override;
    void Pickup(const std::shared_ptr<MppEntity>& entity);
    bool IsHoldingEntity() const;
    void DropHeldEntity();

protected:
    void Render(const MppItem& item) const;
    int GetMaxItems() const override;
    int GetSpritePose1X() const;
    int GetSpritePose1Y() const;
    int GetSpritePose2X() const;
    int GetSpritePose2Y() const;
    virtual int GetSpriteBorderColor() const = 0;
    virtual int GetSpriteSkinColor() const = 0;
    virtual int GetSpriteShirtColor() const = 0;
    virtual int GetSpritePantColor() const = 0;
    virtual int GetSpriteShoeColor() const = 0;

protected:
    std::shared_ptr<MppEntity> HeldEntity;
    MppSpriteAnimation Animation;
};
