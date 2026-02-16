#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/mob/mob.hpp>

class MppItem;

class MppHumanoidEntity : public MppMobEntity
{
public:
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Render() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    void DoAction() override;
    void Equip(int index) override;
    int GetActionOffset() const override;
    void PickupEntity(const std::shared_ptr<MppEntity>& entity);
    void DropEntity();
    std::shared_ptr<MppEntity> GetEntity() const;
    void SetRiding(bool riding);

protected:
    void Render(const MppItem& item) const;
    int GetMaxItems() const override;
    int GetAnimationPose1X() const override;
    int GetAnimationPose1Y() const override;
    int GetAnimationPose2X() const override;
    int GetAnimationPose2Y() const override;
    int GetAnimationPose3X() const override;
    int GetAnimationPose3Y() const override;
    int GetAnimationPose() const override;

protected:
    std::shared_ptr<MppEntity> Entity;
    bool Riding;
};
