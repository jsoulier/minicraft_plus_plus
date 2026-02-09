#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/mob/mob.hpp>

class MppItem;

class MppHumanoidEntity : public MppMobEntity
{
public:
    virtual void OnAddEntity() override;
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Render() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetActionRange() const override;
    void HoldEntity(const std::shared_ptr<MppEntity>& entity);
    void DropHeldEntity();
    void MoveHeldEntityToInventory();
    std::shared_ptr<MppEntity> GetHeldEntity() const;
    void DoAction() override;

protected:
    void Render(const MppItem& item) const;
    int GetMaxItems() const override;
    int GetSpritePose1X() const override;
    int GetSpritePose1Y() const override;
    int GetSpritePose2X() const override;
    int GetSpritePose2Y() const override;
    int GetPose() const override;

protected:
    std::shared_ptr<MppEntity> HeldEntity;
};
