#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/mob/mob.hpp>

class MppItem;

enum MppHumanoidEntityLocomotionMode : uint8_t
{
    MppHumanoidEntityLocomotionModeDefault,
    MppHumanoidEntityLocomotionModeHorse,
    MppHumanoidEntityLocomotionModeMinecart,
};

class MppHumanoidEntity : public MppMobEntity
{
public:
    MppHumanoidEntity();
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Update(uint64_t ticks) override;
    virtual void Render() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    void OnMount(const std::shared_ptr<MppMobEntity>& vehicle) override;
    void OnUnmount() override;
    void DoAction() override;
    void Equip(int index) override;
    int GetActionOffset() const override;
    void PickupEntity(const std::shared_ptr<MppEntity>& entity);
    void DropEntity();
    std::shared_ptr<MppEntity> GetEntity() const;

protected:
    void Render(const MppItem& item) const;
    int GetMaxItems() const override;
    int GetAnimationPose1X() const override;
    int GetAnimationPose1Y() const override;
    int GetAnimationPose2X() const override;
    int GetAnimationPose2Y() const override;
    int GetAnimationPose3X() const override;
    int GetAnimationPose3Y() const override;
    int GetAnimationPose4X() const override;
    int GetAnimationPose4Y() const override;
    int GetAnimationPose() const override;

protected:
    std::shared_ptr<MppEntity> Entity;
    MppHumanoidEntityLocomotionMode LocomotionMode;
};
