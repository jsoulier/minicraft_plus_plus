#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/entity/mob/mob.hpp>

class MppMountEntityProxy;

class MppMinecartEntity final : public MppMobEntity
{
    SAVEPOINT_POLY(MppMinecartEntity)

public:
    MppMinecartEntity();
    void OnAdd() override;
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    void Render() const override;
    bool OnAction(std::shared_ptr<MppEntity>& instigator) override;
    bool OnInteraction(std::shared_ptr<MppEntity>& instigator) override;
    virtual MppEntityCollision OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy);
    void DoAction() override;
    MppItemID GetItemID() const override;
    bool Drop(const std::shared_ptr<MppEntity>& instigator) override;

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
    int GetSpeed() const override;

private:
    std::shared_ptr<MppMountEntityProxy> Mount;
};