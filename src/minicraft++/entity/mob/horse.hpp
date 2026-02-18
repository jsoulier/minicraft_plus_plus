#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/entity/mob/creature.hpp>

class MppMobEntityMount;

class MppHorseEntity final : public MppCreatureEntity
{
    SAVEPOINT_POLY(MppHorseEntity)

public:
    MppHorseEntity();
    void OnAdd() override;
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    void Render() const override;
    bool OnInteraction(std::shared_ptr<MppEntity>& instigator) override;
    void DoAction() override;

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
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
    int GetSpeed() const override;
    int GetMoveTickRate() const override;

private:
    std::shared_ptr<MppMobEntityMount> Mount;
};
