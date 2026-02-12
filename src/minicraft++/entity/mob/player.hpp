#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/entity/mob/humanoid.hpp>

class MppPlayerEntity final : public MppHumanoidEntity
{
    SAVEPOINT_POLY(MppPlayerEntity)

public:
    MppPlayerEntity();
    void OnAdd() override;
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    void Render() const override;

protected:
    std::shared_ptr<MppController> GetDefaultController();
    int GetSpriteColor1() const override;
    int GetSpriteColor2() const override;
    int GetSpriteColor3() const override;
    int GetSpriteColor4() const override;
    int GetSpriteColor5() const override;
    int GetMoveTickRate() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
};