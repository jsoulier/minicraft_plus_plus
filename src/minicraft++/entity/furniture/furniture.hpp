#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/entity/entity.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/sprite.hpp>

class MppInventory;

class MppFurnitureEntity : public MppEntity
{
public:
    MppFurnitureEntity();
    virtual void OnCreate() override;
    virtual void Update(uint64_t ticks) override;
    virtual void Render() const override;
    virtual bool OnAction(std::shared_ptr<MppEntity>& instigator) override;
    virtual MppEntityCollision OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy) override;
    bool Drop(const std::shared_ptr<MppEntity>& instigator) override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSize() const override;

protected:
    virtual int GetAnimationTickRate() const;
    virtual int GetAnimationPose1X() const = 0;
    virtual int GetAnimationPose1Y() const = 0;
    virtual int GetAnimationPose2X() const;
    virtual int GetAnimationPose2Y() const;
    virtual int GetAnimationPose3X() const;
    virtual int GetAnimationPose3Y() const;
    virtual int GetAnimationPose4X() const;
    virtual int GetAnimationPose4Y() const;
    virtual int GetColor1() const = 0;
    virtual int GetColor2() const = 0;
    virtual int GetColor3() const = 0;
    virtual int GetColor4() const = 0;
    virtual int GetColor5() const = 0;

private:
    MppSpriteAnimation Animation;
};
