#pragma once

#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/mob/creature.hpp>

class MppMountController;

class MppHorseEntity final : public MppCreatureEntity
{
    SAVEPOINT_POLY(MppHorseEntity)

public:
    MppHorseEntity();
    void Visit(SavepointVisitor& visitor) override;
    void OnAdd() override;
    void Update(uint64_t ticks) override;
    void Render() const override;
    void OnUnpossess() override;
    bool OnInteraction(MppEntity& instigator) override;
    void DoAction() override;

protected:
    int GetSpritePose1X() const override;
    int GetSpritePose1Y() const override;
    int GetSpriteColor1() const override;
    int GetSpriteColor2() const override;
    int GetSpriteColor3() const override;
    int GetSpriteColor4() const override;
    int GetSpriteColor5() const override;
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
    std::shared_ptr<MppMountController> MountController;
};
